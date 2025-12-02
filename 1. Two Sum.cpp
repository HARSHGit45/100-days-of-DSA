
package com.imobile3.pos.data.module.batch.report

import android.database.Cursor
import android.util.SparseArray
import com.imobile3.pos.data.module.batch.BatchManager
import com.imobile3.pos.data.module.order.cart.tender.TenderManager
import com.imobile3.pos.data.module.order.cart.transaction.TransactionRepository
import com.imobile3.pos.library.constants.enums.CvmResult
import com.imobile3.pos.library.domainobjects.*
import com.imobile3.pos.library.webservices.enums.*
import org.junit.Assert.*
import org.junit.Before
import org.junit.Test
import org.mockito.ArgumentMatchers.any
import org.mockito.ArgumentMatchers.eq
import org.mockito.Mockito
import org.mockito.kotlin.whenever
import java.lang.reflect.Method
import java.math.BigDecimal

class GetBatchSummaryListWithReportsUseCaseTest {

    private lateinit var transactionRepository: TransactionRepository
    private lateinit var batchManager: BatchManager
    private lateinit var tenderManager: TenderManager
    private lateinit var useCase: GetBatchSummaryListWithReportsUseCase

    @Before
    fun setup() {
        transactionRepository = Mockito.mock(TransactionRepository::class.java)
        batchManager = Mockito.mock(BatchManager::class.java)
        tenderManager = Mockito.mock(TenderManager::class.java)
        useCase = GetBatchSummaryListWithReportsUseCase(
            transactionRepository,
            batchManager,
            tenderManager
        )
    }

    // ---------------------------
    // removeZeroTotalDeposits()
    // ---------------------------
    @Test
    fun `removeZeroTotalDeposits removes zero total deposits`() {
        val method: Method = GetBatchSummaryListWithReportsUseCase::class.java
            .getDeclaredMethod("removeZeroTotalDeposits", BatchReport::class.java)
        method.isAccessible = true

        val report = BatchReport()

        val zero = BatchDepositDto().apply {
            paymentType = PaymentType.Cash
            totalAmount = BigDecimal.ZERO
        }
        val nonZero = BatchDepositDto().apply {
            paymentType = PaymentType.Cash
            totalAmount = BigDecimal("12.34")
        }

        report.addDeposit(zero)
        report.addDeposit(nonZero)

        // pre-check
        assertEquals(2, report.deposits.size)

        method.invoke(useCase, report)

        // only non-zero remains
        assertEquals(1, report.deposits.size)
        assertEquals(BigDecimal("12.34"), report.deposits[0].totalAmount)
    }

    // ---------------------------
    // shouldUpdateTenderCount()
    // ---------------------------
    @Test
    fun `shouldUpdateTenderCount returns true for valid credit card authorized with cvm not pin`() {
        val method = GetBatchSummaryListWithReportsUseCase::class.java.getDeclaredMethod(
            "shouldUpdateTenderCount",
            TenderType::class.java,
            TenderStatusType::class.java,
            TxDbTender::class.java
        )
        method.isAccessible = true

        val tender = Mockito.mock(TxDbTender::class.java)
        whenever(tender.paymentType).thenReturn(PaymentType.CreditCard)
        whenever(tender.tenderCreditStatusType).thenReturn(TenderCreditStatusType.Authorized)
        whenever(tender.cvmResult).thenReturn(CvmResult.Signature)

        val res = method.invoke(
            useCase,
            TenderType.Normal,
            TenderStatusType.Completed,
            tender
        ) as Boolean

        assertTrue(res)
    }

    @Test
    fun `shouldUpdateTenderCount returns false for non-credit payment`() {
        val method = GetBatchSummaryListWithReportsUseCase::class.java.getDeclaredMethod(
            "shouldUpdateTenderCount",
            TenderType::class.java,
            TenderStatusType::class.java,
            TxDbTender::class.java
        )
        method.isAccessible = true

        val tender = Mockito.mock(TxDbTender::class.java)
        whenever(tender.paymentType).thenReturn(PaymentType.Cash)

        val res = method.invoke(
            useCase,
            TenderType.Normal,
            TenderStatusType.Completed,
            tender
        ) as Boolean

        assertFalse(res)
    }

    // ---------------------------
    // getCustomTenderDeposits()
    // ---------------------------
    @Test
    fun `getCustomTenderDeposits adds new custom deposit when none exists`() {
        val method = GetBatchSummaryListWithReportsUseCase::class.java.getDeclaredMethod(
            "getCustomTenderDeposits",
            BatchReport::class.java,
            TxDbTender::class.java,
            String::class.java
        )
        method.isAccessible = true

        val report = BatchReport()

        val tender = Mockito.mock(TxDbTender::class.java)
        whenever(tender.paymentType).thenReturn(PaymentType.CustomTender)
        whenever(tender.orderAmount).thenReturn(BigDecimal("5.00"))
        whenever(tender.tipAmount).thenReturn(BigDecimal("1.00"))
        whenever(tender.changeAmount).thenReturn(BigDecimal.ZERO)
        whenever(tender.totalAmount).thenReturn(BigDecimal("6.00"))
        whenever(tender.amountReceived).thenReturn(BigDecimal("6.00"))

        val fields = Mockito.mock(CustomTenderFieldsDto::class.java)
        whenever(fields.customTenderId).thenReturn("CT-101")
        whenever(tender.customFields).thenReturn(fields)

        // simulate no matching existing deposit
        whenever(batchManager.getCustomTenderFieldsDto(any())).thenReturn(null)

        method.invoke(useCase, report, tender, "tag")

        assertEquals(1, report.deposits.size)
        val added = report.deposits[0]
        assertEquals(PaymentType.CustomTender, added.paymentType)
        assertEquals(BigDecimal("5.00"), added.orderAmount)
        assertEquals(BigDecimal("1.00"), added.tipAmount)
        assertEquals(BigDecimal("6.00"), added.totalAmount)
        assertEquals(BigDecimal("6.00"), added.amountReceived)
        // custom fields json should be set (string) - toJson implementation unknown; just ensure not null when set by code
        assertNotNull(added.customTenderFields)
    }

    @Test
    fun `getCustomTenderDeposits merges into existing deposit when customTenderId matches`() {
        val method = GetBatchSummaryListWithReportsUseCase::class.java.getDeclaredMethod(
            "getCustomTenderDeposits",
            BatchReport::class.java,
            TxDbTender::class.java,
            String::class.java
        )
        method.isAccessible = true

        val report = BatchReport()

        // create existing deposit with custom id CT-201
        val existing = BatchDepositDto().apply {
            paymentType = PaymentType.CustomTender
            orderAmount = BigDecimal("3.00")
            tipAmount = BigDecimal("0.50")
            changeAmount = BigDecimal.ZERO
            totalAmount = BigDecimal("3.50")
            amountReceived = BigDecimal("3.50")
        }
        report.addDeposit(existing)

        val existingFields = CustomTenderFieldsDto().apply { customTenderId = "CT-201" }
        whenever(batchManager.getCustomTenderFieldsDto(existing)).thenReturn(existingFields)

        val tender = Mockito.mock(TxDbTender::class.java)
        whenever(tender.paymentType).thenReturn(PaymentType.CustomTender)
        whenever(tender.orderAmount).thenReturn(BigDecimal("2.00"))
        whenever(tender.tipAmount).thenReturn(BigDecimal("0.25"))
        whenever(tender.changeAmount).thenReturn(BigDecimal.ZERO)
        whenever(tender.totalAmount).thenReturn(BigDecimal("2.25"))
        whenever(tender.amountReceived).thenReturn(BigDecimal("2.25"))
        whenever(tender.customFields).thenReturn(existingFields)

        method.invoke(useCase, report, tender, "tag")

        // deposit should be merged: orderAmount 3 + 2 = 5, tip 0.5 + 0.25 = 0.75, total 3.5 + 2.25 = 5.75
        assertEquals(1, report.deposits.size)
        val merged = report.deposits[0]
        assertEquals(BigDecimal("5.00"), merged.orderAmount)
        assertEquals(BigDecimal("0.75"), merged.tipAmount)
        assertEquals(BigDecimal("5.75"), merged.totalAmount)
    }

    // ---------------------------
    // generateUserTipTotal()
    // ---------------------------
    @Test
    fun `generateUserTipTotal creates and accumulates per-user tip totals`() {
        val method = GetBatchSummaryListWithReportsUseCase::class.java.getDeclaredMethod(
            "generateUserTipTotal",
            TxDbTransaction::class.java,
            BatchReport::class.java
        )
        method.isAccessible = true

        val report = BatchReport()

        val tx1 = Mockito.mock(TxDbTransaction::class.java)
        whenever(tx1.userId).thenReturn(77)
        whenever(tx1.totalTips).thenReturn(BigDecimal("2.50"))

        method.invoke(useCase, tx1, report)
        assertEquals(1, report.userTipTotalMap.size())
        assertEquals(BigDecimal("2.50"), report.userTipTotalMap[77])

        val tx2 = Mockito.mock(TxDbTransaction::class.java)
        whenever(tx2.userId).thenReturn(77)
        whenever(tx2.totalTips).thenReturn(BigDecimal("1.25"))

        method.invoke(useCase, tx2, report)
        assertEquals(1, report.userTipTotalMap.size())
        assertEquals(BigDecimal("3.75"), report.userTipTotalMap[77])
    }

    // ---------------------------
    // generateDeposits() - exercise all branch filters:
    //  - invalid tender status (skip)
    //  - non-auth & cancelled (skip)
    //  - non-auth & voided (skip)
    //  - failedToVoid (skip)
    //  - valid tender increments tipAdjustTenderCount and adds deposit
    // ---------------------------
    @Test
    fun `generateDeposits filters and adds appropriately`() {
        val method = GetBatchSummaryListWithReportsUseCase::class.java.getDeclaredMethod(
            "generateDeposits",
            String::class.java,
            List::class.java,
            BatchReport::class.java
        )
        method.isAccessible = true

        // Use MockedStatic to control static TenderManager helper methods
        // (isTenderStatusTypeValid, isCancelled, isVoided, isFailedToVoid)
        val mockedStatic = Mockito.mockStatic(TenderManager::class.java)
        try {
            // Prepare a report that will collect deposits and count tipAdjustTenderCount
            val report = BatchReport()

            // 1) Tender with invalid status -> should be skipped immediately
            val tInvalid = Mockito.mock(TxDbTender::class.java)
            whenever(tInvalid.tenderStatusType).thenReturn(TenderStatusType.Pending)
            // static will return false for this status
            mockedStatic.`when`<Boolean> { TenderManager.isTenderStatusTypeValid(TenderStatusType.Pending) }
                .thenReturn(false)

            // 2) Non-auth tender that is cancelled -> skip
            val tNonAuthCancelled = Mockito.mock(TxDbTender::class.java)
            whenever(tNonAuthCancelled.tenderStatusType).thenReturn(TenderStatusType.Completed)
            whenever(tNonAuthCancelled.paymentType).thenReturn(PaymentType.Cash)
            whenever(tenderManager.isNonAuthTender(tNonAuthCancelled)).thenReturn(true)
            mockedStatic.`when`<Boolean> { TenderManager.isTenderStatusTypeValid(TenderStatusType.Completed) }
                .thenReturn(true)
            mockedStatic.`when`<Boolean> { TenderManager.isCancelled(tNonAuthCancelled) }
                .thenReturn(true)

            // 3) Non-auth tender that is voided -> skip
            val tNonAuthVoided = Mockito.mock(TxDbTender::class.java)
            whenever(tNonAuthVoided.tenderStatusType).thenReturn(TenderStatusType.Completed)
            whenever(tNonAuthVoided.paymentType).thenReturn(PaymentType.Cash)
            whenever(tenderManager.isNonAuthTender(tNonAuthVoided)).thenReturn(true)
            mockedStatic.`when`<Boolean> { TenderManager.isVoided(tNonAuthVoided) }.thenReturn(true)

            // 4) Tender failed to void -> skip
            val tFailedVoid = Mockito.mock(TxDbTender::class.java)
            whenever(tFailedVoid.tenderStatusType).thenReturn(TenderStatusType.Completed)
            mockedStatic.`when`<Boolean> { TenderManager.isFailedToVoid(tFailedVoid) }.thenReturn(true)

            // 5) Valid credit card tender -> should be added and tipAdjustTenderCount incremented
            val tValid = Mockito.mock(TxDbTender::class.java)
            whenever(tValid.tenderStatusType).thenReturn(TenderStatusType.Completed)
            whenever(tValid.tenderType).thenReturn(TenderType.Normal)
            whenever(tValid.paymentType).thenReturn(PaymentType.CreditCard)
            whenever(tValid.tenderCreditStatusType).thenReturn(TenderCreditStatusType.Authorized)
            whenever(tValid.cvmResult).thenReturn(CvmResult.Signature)
            whenever(tValid.orderAmount).thenReturn(BigDecimal("10.00"))
            whenever(tValid.tipAmount).thenReturn(BigDecimal("2.00"))
            whenever(tValid.changeAmount).thenReturn(BigDecimal.ZERO)
            whenever(tValid.totalAmount).thenReturn(BigDecimal("12.00"))
            whenever(tValid.amountReceived).thenReturn(BigDecimal("12.00"))
            // ensure static checks for valid status return true
            mockedStatic.`when`<Boolean> { TenderManager.isTenderStatusTypeValid(TenderStatusType.Completed) }
                .thenReturn(true)
            // ensure non-auth checks return false for this tender
            whenever(tenderManager.isNonAuthTender(tValid)).thenReturn(false)
            // static checks for cancelled/void/failedToVoid should be false for tValid
            mockedStatic.`when`<Boolean> { TenderManager.isCancelled(tValid) }.thenReturn(false)
            mockedStatic.`when`<Boolean> { TenderManager.isVoided(tValid) }.thenReturn(false)
            mockedStatic.`when`<Boolean> { TenderManager.isFailedToVoid(tValid) }.thenReturn(false)

            // For custom tender matching when adding deposit: return null (new deposit)
            whenever(batchManager.getCustomTenderFieldsDto(any())).thenReturn(null)

            // now call generateDeposits with a list of tenders that contains all cases
            val tenders = listOf(tInvalid, tNonAuthCancelled, tNonAuthVoided, tFailedVoid, tValid)

            method.invoke(useCase, "tag", tenders, report)

            // Only tValid should have resulted in a deposit being added
            // and tipAdjustTenderCount should be incremented once (for tValid)
            assertEquals(1, report.deposits.size)
            val dep = report.deposits[0]
            assertEquals(PaymentType.CreditCard, dep.paymentType)
            assertEquals(BigDecimal("10.00"), dep.orderAmount)
            assertEquals(BigDecimal("2.00"), dep.tipAmount)
            assertEquals(BigDecimal("12.00"), dep.totalAmount)
            assertEquals(1, report.tipAdjustTenderCount)
        } finally {
            mockedStatic.close()
        }
    }

    // ---------------------------
    // getBatchReport exception propagation (private)
    // ---------------------------
    @Test(expected = RuntimeException::class)
    fun `getBatchReport rethrows exceptions from repository`() {
        // call private getBatchReport and ensure exception is rethrown
        val method = GetBatchSummaryListWithReportsUseCase::class.java.getDeclaredMethod(
            "getBatchReport",
            String::class.java,
            Long::class.javaPrimitiveType
        )
        method.isAccessible = true

        // make repository throw when fetching transactions
        whenever(transactionRepository.getTransactionsFromBatchNumber(999L))
            .thenThrow(RuntimeException("db fail"))

        // invoking should rethrow
        method.invoke(useCase, "tag", 999L)
    }

    // ---------------------------
    // execute() mapping behavior
    // ---------------------------
    @Test
    fun `execute returns batch summary responses for given batch numbers when no transactions`() {
        // If repository returns null cursor for a batch, getBatchReport returns an empty report.
        whenever(transactionRepository.getTransactionsFromBatchNumber(1L)).thenReturn(null)
        whenever(transactionRepository.getTransactionsFromBatchNumber(2L)).thenReturn(null)

        val results = useCase.execute("tag", listOf(1L, 2L))

        assertEquals(2, results.size)
        assertEquals(1L, results[0].batchNumber)
        assertEquals(2L, results[1].batchNumber)
        assertNotNull(results[0].batchReport)
        assertNotNull(results[1].batchReport)
    }
}






body {
  font-family: Arial, sans-serif;
  margin: 20px;
  background: #f8f9fa;
}

.tabs {
  display: flex;
  gap: 10px;
  margin-bottom: 15px;
}

.tabs button {
  padding: 10px 18px;
  border: none;
  background: #ddd;
  border-radius: 6px;
  cursor: pointer;
}

.tabs button.active {
  background: #007bff;
  color: white;
}

#searchBox {
  width: 300px;
  padding: 8px;
  margin-bottom: 12px;
}

#backBtn {
  margin-bottom: 12px;
  padding: 8px 14px;
  cursor: pointer;
}

table {
  width: 100%;
  border-collapse: collapse;
  background: white;
}

table th, table td {
  padding: 10px;
  border: 1px solid #ccc;
  text-align: left;
}

table tr:hover {
  background: #f1f1f1;
  cursor: pointer;
}

.progress-up {
  color: green;
  font-weight: bold;
}

.progress-down {
  color: red;
  font-weight: bold;
}





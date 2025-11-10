class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        int n=nums.size();


        for(int i=0;i<n;i++)
        {
            for(int j=i+1;j<n;j++)
            {
                if(nums[i]+nums[j]==target)
                {
                    return {i,j};
                }
            }
        }

        return {};
        
    }

};


package com.imobile3.pos.data.module.batch.report

import android.content.Context
import android.database.Cursor
import com.imobile3.pos.library.database.transaction.accessObjects.TransactionsDao
import com.imobile3.pos.library.domainobjects.BatchReport
import com.imobile3.pos.library.domainobjects.TxDbTender
import com.imobile3.pos.library.domainobjects.TxDbTransaction
import com.imobile3.pos.library.webservices.enums.*
import com.imobile3.pos.library.webservices.transferobjects.BatchDepositDto
import io.mockk.every
import io.mockk.just
import io.mockk.mockk
import io.mockk.runs
import org.junit.jupiter.api.*
import org.junit.jupiter.api.extension.ExtendWith
import org.mockito.Mock
import org.mockito.junit.jupiter.MockitoExtension
import java.math.BigDecimal

@ExtendWith(MockitoExtension::class)
class BatchReportLoaderTest {

    @Mock
    private lateinit var mockContext: Context

    @Mock
    private lateinit var mockDao: TransactionsDao

    private lateinit var mockCursor: Cursor
    private lateinit var loader: BatchReportLoader

    @BeforeEach
    fun setup() {
        mockCursor = mockk(relaxed = true) // relaxed avoids NPE on close(), moveToNext(), etc.
        loader = BatchReportLoader(mockContext, listOf(1001L))

        // Inject the mock DAO into loader (private field)
        val daoField = loader.javaClass.getDeclaredField("mDao")
        daoField.isAccessible = true
        daoField.set(loader, mockDao)
    }

    // ---------------------------------------------------------------------
    // 1. loadInBackground()
    // ---------------------------------------------------------------------
    @Test
    fun `loadInBackground returns one response per batch`() {
        val report = BatchReport().apply { setTipAdjustTenderCount(3) }

        // mock private method call by replacing it through reflection
        val getBatchMethod = loader.javaClass.getDeclaredMethod("getBatchReport", Long::class.java)
        getBatchMethod.isAccessible = true
        getBatchMethod.invoke(loader, 1001L)

        // Instead of deep testing internals, stub the DAO so no crash
        every { mockDao.getTransactions(any()) } returns mockCursor
        every { mockCursor.moveToFirst() } returns false

        val responses = loader.loadInBackground()

        Assertions.assertEquals(1, responses.size)
        Assertions.assertEquals(1001L, responses.first().batchNumber)
        Assertions.assertNotNull(responses.first().batchReport)
    }

    // ---------------------------------------------------------------------
    // 2. getBatchReport() — handles cancelled and valid transactions
    // ---------------------------------------------------------------------
    @Test
    fun `getBatchReport skips cancelled and processes valid transactions`() {
        val txCancelled = TxDbTransaction().apply {
            transactionStatusType = TransactionStatusType.Cancelled
        }
        val txValid = TxDbTransaction().apply {
            transactionStatusType = TransactionStatusType.Completed
            transactionNumber = 2222L
            userId = 10
            totalTips = BigDecimal("5.00")
        }

        // Tenders for valid transaction
        val tender = TxDbTender().apply {
            tenderStatusType = TenderStatusType.Completed
            tenderType = TenderType.Normal
            paymentType = PaymentType.Cash
            tenderCreditStatusType = TenderCreditStatusType.Authorized
            totalAmount = BigDecimal("20.00")
            amountReceived = BigDecimal("20.00")
        }

        // Cursor mocking
        every { mockDao.getTransactions(1001L) } returns mockCursor
        every { mockCursor.moveToFirst() } returns true
        every { mockCursor.moveToNext() } returnsMany listOf(true, false)
        every { mockDao.getTxDbTransactionFromCursor(mockCursor) } returnsMany listOf(txCancelled, txValid)
        every { mockDao.getTendersByTransactionNumber(2222L) } returns listOf(tender)
        every { mockCursor.close() } just runs

        // Invoke private getBatchReport(long)
        val method = loader.javaClass.getDeclaredMethod("getBatchReport", Long::class.java)
        method.isAccessible = true
        val report = method.invoke(loader, 1001L) as BatchReport

        Assertions.assertEquals(1, report.deposits.size)
        Assertions.assertEquals(PaymentType.Cash, report.deposits.first().paymentType)
        Assertions.assertEquals(BigDecimal("20.00"), report.deposits.first().totalAmount)
        Assertions.assertEquals(BigDecimal("5.00"), report.userTipTotalMap.get(10))
    }

    // ---------------------------------------------------------------------
    // 3. generateDeposits() — combines same tender type amounts
    // ---------------------------------------------------------------------
    @Test
    fun `generateDeposits adds and combines deposits`() {
        val tender1 = TxDbTender().apply {
            tenderStatusType = TenderStatusType.Completed
            tenderType = TenderType.Normal
            paymentType = PaymentType.CreditCard
            tenderCreditStatusType = TenderCreditStatusType.Authorized
            totalAmount = BigDecimal("50.00")
            amountReceived = BigDecimal("50.00")
        }
        val tender2 = TxDbTender().apply {
            tenderStatusType = TenderStatusType.Completed
            tenderType = TenderType.Normal
            paymentType = PaymentType.CreditCard
            tenderCreditStatusType = TenderCreditStatusType.Authorized
            totalAmount = BigDecimal("25.00")
            amountReceived = BigDecimal("25.00")
        }

        val report = BatchReport()
        val reportField = loader.javaClass.getDeclaredField("mReport")
        reportField.isAccessible = true
        reportField.set(loader, report)

        val method = loader.javaClass.getDeclaredMethod("generateDeposits", List::class.java)
        method.isAccessible = true
        method.invoke(loader, listOf(tender1, tender2))

        val deposits = report.deposits
        Assertions.assertEquals(1, deposits.size)
        Assertions.assertEquals(BigDecimal("75.00"), deposits.first().totalAmount)
        Assertions.assertEquals(PaymentType.CreditCard, deposits.first().paymentType)
    }

    // ---------------------------------------------------------------------
    // 4. removeZeroTotalDeposits()
    // ---------------------------------------------------------------------
    @Test
    fun `removeZeroTotalDeposits filters out zero deposits`() {
        val d1 = BatchDepositDto().apply { totalAmount = BigDecimal.ZERO }
        val d2 = BatchDepositDto().apply { totalAmount = BigDecimal("10.00") }

        val report = BatchReport().apply { setDeposits(listOf(d1, d2)) }
        val reportField = loader.javaClass.getDeclaredField("mReport")
        reportField.isAccessible = true
        reportField.set(loader, report)

        val method = loader.javaClass.getDeclaredMethod("removeZeroTotalDeposits")
        method.isAccessible = true
        method.invoke(loader)

        Assertions.assertEquals(1, report.deposits.size)
        Assertions.assertEquals(BigDecimal("10.00"), report.deposits.first().totalAmount)
    }

    // ---------------------------------------------------------------------
    // 5. generateUserTipTotal()
    // ---------------------------------------------------------------------
    @Test
    fun `generateUserTipTotal aggregates per user`() {
        val tx1 = TxDbTransaction().apply {
            userId = 1
            totalTips = BigDecimal("3.00")
        }
        val tx2 = TxDbTransaction().apply {
            userId = 1
            totalTips = BigDecimal("2.00")
        }

        val report = BatchReport()
        val reportField = loader.javaClass.getDeclaredField("mReport")
        reportField.isAccessible = true
        reportField.set(loader, report)

        val method = loader.javaClass.getDeclaredMethod("generateUserTipTotal", TxDbTransaction::class.java)
        method.isAccessible = true
        method.invoke(loader, tx1)
        method.invoke(loader, tx2)

        val total = report.userTipTotalMap.get(1)
        Assertions.assertEquals(BigDecimal("5.00"), total)
    }

    // ---------------------------------------------------------------------
    // 6. getBatchReport handles exception gracefully
    // ---------------------------------------------------------------------
    @Test
    fun `getBatchReport handles DAO exception gracefully`() {
        every { mockDao.getTransactions(any()) } throws RuntimeException("DB failure")

        val method = loader.javaClass.getDeclaredMethod("getBatchReport", Long::class.java)
        method.isAccessible = true

        Assertions.assertDoesNotThrow {
            val report = method.invoke(loader, 1001L) as BatchReport
            Assertions.assertNotNull(report)
        }
    }
}

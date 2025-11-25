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

};package com.imobile3.pos.data.module.batch.close

import com.imobile3.pos.app.constants.intdefs.BatchCloseState
import com.imobile3.pos.library.domainobjects.BatchReport
import com.imobile3.pos.library.domainobjects.BatchReportDetailed
import com.imobile3.pos.library.domainobjects.TxDbBatch
import com.imobile3.pos.library.domainobjects.TxDbBatchTransfer
import org.junit.Assert.*
import org.junit.Test
import org.mockito.Mockito
import java.util.Date

class BatchCloseResponseTest {

    @Test
    fun testDefaultValues() {
        val response = BatchCloseResponse()

        // Defaults
        assertEquals(0, response.state)
        assertNull(response.attemptedDateTime)
        assertNull(response.batch)
        assertNull(response.batchReport)
        assertNull(response.batchReportDetailed)
        assertNull(response.batchTransfers)
    }

    @Test
    fun testStateSetterGetter() {
        val response = BatchCloseResponse()

        response.state = BatchCloseState.SUCCESS
        assertEquals(BatchCloseState.SUCCESS, response.state)

        response.state = BatchCloseState.FAILED
        assertEquals(BatchCloseState.FAILED, response.state)
    }

    @Test
    fun testAttemptedDateTimeSetterGetter() {
        val response = BatchCloseResponse()

        val date = Date()
        response.attemptedDateTime = date
        assertEquals(date, response.attemptedDateTime)

        // Set to null to test nullability
        response.attemptedDateTime = null
        assertNull(response.attemptedDateTime)
    }

    @Test
    fun testBatchSetterGetter() {
        val response = BatchCloseResponse()

        val batch = Mockito.mock(TxDbBatch::class.java)
        response.batch = batch
        assertEquals(batch, response.batch)

        response.batch = null
        assertNull(response.batch)
    }

    @Test
    fun testBatchReportSetterGetter() {
        val response = BatchCloseResponse()

        val report = Mockito.mock(BatchReport::class.java)
        response.batchReport = report
        assertEquals(report, response.batchReport)

        response.batchReport = null
        assertNull(response.batchReport)
    }

    @Test
    fun testBatchReportDetailedSetterGetter() {
        val response = BatchCloseResponse()

        val detailedReport = Mockito.mock(BatchReportDetailed::class.java)
        response.batchReportDetailed = detailedReport
        assertEquals(detailedReport, response.batchReportDetailed)

        response.batchReportDetailed = null
        assertNull(response.batchReportDetailed)
    }

    @Test
    fun testBatchTransfersSetterGetter() {
        val response = BatchCloseResponse()

        val item1 = Mockito.mock(TxDbBatchTransfer::class.java)
        val item2 = Mockito.mock(TxDbBatchTransfer::class.java)
        val list = listOf(item1, item2)

        response.batchTransfers = list
        assertEquals(2, response.batchTransfers?.size)
        assertEquals(item1, response.batchTransfers?.get(0))
        assertEquals(item2, response.batchTransfers?.get(1))

        response.batchTransfers = null
        assertNull(response.batchTransfers)
    }
}

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


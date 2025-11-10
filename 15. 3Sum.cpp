class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& arr) {

        int t=0;

        sort(arr.begin(),arr.end());

        set<vector<int>> s;


        for(int i=0;i<arr.size();i++)
        {
            int j = i+1;
            int k = arr.size()-1;

            while(j<k){

            int sum = arr[i]+arr[j]+arr[k];


            if(sum==t){
                s.insert({arr[i],arr[j],arr[k]});
                j++;
                k--;
            }
            else if(sum<t){
                j++;
            }
            else{
                k--;
            }

            }

            
        }

        vector<vector<int>> ans;

        for(auto i: s){
            ans.push_back(i);
        }

        return ans;
        
        
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
import io.mockk.*
import org.junit.jupiter.api.*
import org.junit.jupiter.api.extension.ExtendWith
import org.mockito.Mock
import org.mockito.junit.jupiter.MockitoExtension
import java.math.BigDecimal
import kotlin.reflect.full.declaredMemberFunctions
import kotlin.reflect.jvm.isAccessible

@ExtendWith(MockitoExtension::class)
class BatchReportLoaderTest {

    @Mock
    private lateinit var mockContext: Context

    @Mock
    private lateinit var mockDao: TransactionsDao

    @Mock
    private lateinit var mockCursor: Cursor

    private lateinit var loader: BatchReportLoader

    @BeforeEach
    fun setup() {
        loader = spyk(BatchReportLoader(mockContext, listOf(1001L)))

        // Inject mock DAO into loader (since it's private)
        loader.javaClass.getDeclaredField("mDao").apply {
            isAccessible = true
            set(loader, mockDao)
        }
    }

    /** Utility to call private functions cleanly */
    private inline fun <reified T> callPrivate(funcName: String, vararg args: Any?): T {
        val fn = loader::class.declaredMemberFunctions.first { it.name == funcName }
        fn.isAccessible = true
        return fn.call(loader, *args) as T
    }

    // --------------------------------------------------------------------------
    // 1. loadInBackground()
    // --------------------------------------------------------------------------

    @Test
    fun `loadInBackground returns correct BatchSummaryResponses`() {
        val mockReport = BatchReport().apply { setTipAdjustTenderCount(2) }
        every { loader["getBatchReport"](1001L) } returns mockReport

        val result = loader.loadInBackground()

        Assertions.assertEquals(1, result.size)
        val response = result.first()
        Assertions.assertEquals(1001L, response.batchNumber)
        Assertions.assertEquals(2, response.batchReport.tipAdjustTenderCount)
    }

    // --------------------------------------------------------------------------
    // 2. getBatchReport()
    // --------------------------------------------------------------------------

    @Test
    fun `getBatchReport skips cancelled and processes valid transactions`() {
        // Mock transactions
        val txCancelled = mockk<TxDbTransaction>()
        val txCompleted = mockk<TxDbTransaction>()

        every { txCancelled.transactionStatusType } returns TransactionStatusType.Cancelled
        every { txCompleted.transactionStatusType } returns TransactionStatusType.Completed
        every { txCompleted.transactionNumber } returns 5555L
        every { txCompleted.totalTips } returns BigDecimal("5.00")
        every { txCompleted.userId } returns 101

        // Mock tenders for valid transaction
        val tender = TxDbTender().apply {
            tenderStatusType = TenderStatusType.Completed
            tenderType = TenderType.Normal
            paymentType = PaymentType.Cash
            tenderCreditStatusType = TenderCreditStatusType.Authorized
            totalAmount = BigDecimal("20.00")
            amountReceived = BigDecimal("20.00")
            changeAmount = BigDecimal.ZERO
        }

        every { mockDao.getTransactions(1001L) } returns mockCursor
        every { mockCursor.moveToFirst() } returns true
        every { mockCursor.moveToNext() } returnsMany listOf(true, false)
        every { mockDao.getTxDbTransactionFromCursor(mockCursor) } returnsMany listOf(txCancelled, txCompleted)
        every { mockDao.getTendersByTransactionNumber(5555L) } returns listOf(tender)

        val report = callPrivate<BatchReport>("getBatchReport", 1001L)

        Assertions.assertEquals(1, report.deposits.size, "Only 1 valid deposit expected")
        val deposit = report.deposits.first()
        Assertions.assertEquals(PaymentType.Cash, deposit.paymentType)
        Assertions.assertEquals(BigDecimal("20.00"), deposit.totalAmount)
        Assertions.assertEquals(BigDecimal("5.00"), report.userTipTotalMap.get(101))
    }

    // --------------------------------------------------------------------------
    // 3. generateDeposits()
    // --------------------------------------------------------------------------

    @Test
    fun `generateDeposits combines amounts for same payment type`() {
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
            totalAmount = BigDecimal("30.00")
            amountReceived = BigDecimal("30.00")
        }

        val report = BatchReport()
        loader.javaClass.getDeclaredField("mReport").apply {
            isAccessible = true
            set(loader, report)
        }

        callPrivate<Unit>("generateDeposits", listOf(tender1, tender2))

        val deposits = report.deposits
        Assertions.assertEquals(1, deposits.size)
        Assertions.assertEquals(BigDecimal("80.00"), deposits.first().totalAmount)
        Assertions.assertEquals(PaymentType.CreditCard, deposits.first().paymentType)
    }

    // --------------------------------------------------------------------------
    // 4. removeZeroTotalDeposits()
    // --------------------------------------------------------------------------

    @Test
    fun `removeZeroTotalDeposits removes deposits with zero total`() {
        val d1 = BatchDepositDto().apply { totalAmount = BigDecimal.ZERO }
        val d2 = BatchDepositDto().apply { totalAmount = BigDecimal("25.00") }

        val report = BatchReport().apply { setDeposits(listOf(d1, d2)) }
        loader.javaClass.getDeclaredField("mReport").apply {
            isAccessible = true
            set(loader, report)
        }

        callPrivate<Unit>("removeZeroTotalDeposits")

        Assertions.assertEquals(1, report.deposits.size)
        Assertions.assertEquals(BigDecimal("25.00"), report.deposits.first().totalAmount)
    }

    // --------------------------------------------------------------------------
    // 5. generateUserTipTotal()
    // --------------------------------------------------------------------------

    @Test
    fun `generateUserTipTotal aggregates tips for same user`() {
        val tx1 = TxDbTransaction().apply {
            userId = 42
            totalTips = BigDecimal("5.00")
        }
        val tx2 = TxDbTransaction().apply {
            userId = 42
            totalTips = BigDecimal("7.50")
        }

        val report = BatchReport()
        loader.javaClass.getDeclaredField("mReport").apply {
            isAccessible = true
            set(loader, report)
        }

        callPrivate<Unit>("generateUserTipTotal", tx1)
        callPrivate<Unit>("generateUserTipTotal", tx2)

        val total = report.userTipTotalMap.get(42)
        Assertions.assertEquals(BigDecimal("12.50"), total)
    }

    // --------------------------------------------------------------------------
    // 6. Error Handling (Optional)
    // --------------------------------------------------------------------------

    @Test
    fun `getBatchReport handles exception gracefully`() {
        every { mockDao.getTransactions(any()) } throws RuntimeException("DB failed")

        Assertions.assertDoesNotThrow {
            callPrivate<BatchReport>("getBatchReport", 1001L)
        }
    }
}

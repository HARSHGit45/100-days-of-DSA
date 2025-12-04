
package com.imobile3.pos.data.module.batch.close

import com.imobile3.pos.data.IdManager
import com.imobile3.pos.data.di.AppEntryPoint
import com.imobile3.pos.data.module.account.repository.UserHelper
import com.imobile3.pos.data.module.batch.BatchHelper
import com.imobile3.pos.library.domainobjects.TxDbBatchTransfer
import com.imobile3.pos.library.webservices.enums.TransferType
import com.imobile3.pos.library.webservices.transferobjects.BatchDepositDto
import org.junit.After
import org.junit.Assert.*
import org.junit.Before
import org.junit.Test
import org.junit.runner.RunWith
import org.mockito.Mockito.*
import org.mockito.kotlin.whenever
import org.mockito.junit.MockitoJUnitRunner
import org.mockito.MockedStatic
import java.lang.reflect.Field
import java.math.BigDecimal
import java.util.Date

@RunWith(MockitoJUnitRunner::class)
class BatchTotalsTest {

    // static mocks holders
    private var appEntryPointStatic: MockedStatic<AppEntryPoint>? = null
    private var userHelperStatic: MockedStatic<UserHelper>? = null
    private var batchHelperStatic: MockedStatic<BatchHelper>? = null

    private lateinit var mockIdManager: IdManager

    @Before
    fun setUp() {
        // mock AppEntryPoint.get().getIdManager()
        appEntryPointStatic = mockStatic(AppEntryPoint::class.java)
        userHelperStatic = mockStatic(UserHelper::class.java)
        batchHelperStatic = mockStatic(BatchHelper::class.java)

        mockIdManager = mock(IdManager::class.java)
        val mockAppEntry = mock(AppEntryPoint::class.java)
        whenever(mockAppEntry.getIdManager()).thenReturn(mockIdManager)
        appEntryPointStatic!!.`when`<AppEntryPoint> { AppEntryPoint.get() }.thenReturn(mockAppEntry)

        whenever(UserHelper.getUserId()).thenReturn("test-user")
        whenever(BatchHelper.getCurrentBatchNumber()).thenReturn("BATCH-123")
    }

    @After
    fun tearDown() {
        appEntryPointStatic?.close()
        userHelperStatic?.close()
        batchHelperStatic?.close()
    }

    // utility: read private field by name
    private fun getBigDecimalField(obj: Any, name: String): BigDecimal? {
        val f: Field = obj.javaClass.getDeclaredField(name)
        f.isAccessible = true
        return f.get(obj) as? BigDecimal
    }

    private fun getBooleanField(obj: Any, name: String): Boolean {
        val f: Field = obj.javaClass.getDeclaredField(name)
        f.isAccessible = true
        return f.getBoolean(obj)
    }

    private fun getNullableBigDecimalField(obj: Any, name: String): BigDecimal? {
        val f: Field = obj.javaClass.getDeclaredField(name)
        f.isAccessible = true
        return f.get(obj) as? BigDecimal
    }

    /**
     * Test populate(...) with one deposit of each supported type.
     * Verifies flags and totals are set as expected.
     */
    @Test
    fun `populate should sum expected and received totals and set flags for each payment type`() {
        // create mocks for deposits
        val creditDeposit = mock(BatchDepositDto::class.java)
        whenever(creditDeposit.paymentType).thenReturn(BatchDepositDto.PaymentType.CreditCard)
        whenever(creditDeposit.totalAmount).thenReturn(BigDecimal("100.00"))
        whenever(creditDeposit.amountReceived).thenReturn(BigDecimal("101.50"))

        val cashDeposit = mock(BatchDepositDto::class.java)
        whenever(cashDeposit.paymentType).thenReturn(BatchDepositDto.PaymentType.Cash)
        whenever(cashDeposit.totalAmount).thenReturn(BigDecimal("50.00"))
        // for cash the class code uses getOrderAmount() for received:
        whenever(cashDeposit.orderAmount).thenReturn(BigDecimal("49.00"))
        whenever(cashDeposit.tipAmount).thenReturn(BigDecimal("5.00"))

        val debitDeposit = mock(BatchDepositDto::class.java)
        whenever(debitDeposit.paymentType).thenReturn(BatchDepositDto.PaymentType.DebitCard)
        whenever(debitDeposit.totalAmount).thenReturn(BigDecimal("20.00"))
        whenever(debitDeposit.amountReceived).thenReturn(BigDecimal("20.00"))

        val checkDeposit = mock(BatchDepositDto::class.java)
        whenever(checkDeposit.paymentType).thenReturn(BatchDepositDto.PaymentType.Check)
        whenever(checkDeposit.totalAmount).thenReturn(BigDecimal("10.00"))
        whenever(checkDeposit.amountReceived).thenReturn(BigDecimal("10.00"))

        val giftCertDeposit = mock(BatchDepositDto::class.java)
        whenever(giftCertDeposit.paymentType).thenReturn(BatchDepositDto.PaymentType.GiftCertificate)
        whenever(giftCertDeposit.totalAmount).thenReturn(BigDecimal("5.00"))
        whenever(giftCertDeposit.amountReceived).thenReturn(BigDecimal("4.00"))

        val giftCardDeposit = mock(BatchDepositDto::class.java)
        whenever(giftCardDeposit.paymentType).thenReturn(BatchDepositDto.PaymentType.GiftCard)
        whenever(giftCardDeposit.totalAmount).thenReturn(BigDecimal("6.00"))
        whenever(giftCardDeposit.amountReceived).thenReturn(BigDecimal("6.00"))

        val customDeposit = mock(BatchDepositDto::class.java)
        whenever(customDeposit.paymentType).thenReturn(BatchDepositDto.PaymentType.CustomTender)
        whenever(customDeposit.totalAmount).thenReturn(BigDecimal("2.00"))
        whenever(customDeposit.amountReceived).thenReturn(BigDecimal("1.50"))

        val ebtDeposit = mock(BatchDepositDto::class.java)
        whenever(ebtDeposit.paymentType).thenReturn(BatchDepositDto.PaymentType.EbtCard)
        whenever(ebtDeposit.totalAmount).thenReturn(BigDecimal("7.00"))
        whenever(ebtDeposit.amountReceived).thenReturn(BigDecimal("7.00"))

        // transfers: include one SafeDrop (should be ignored) and one other (should be added to cash expected)
        val safeDropTransfer = mock(TxDbBatchTransfer::class.java)
        whenever(safeDropTransfer.transferTypeId).thenReturn(TransferType.SafeDrop)
        whenever(safeDropTransfer.amount).thenReturn(BigDecimal("1000.00"))

        val otherTransfer = mock(TxDbBatchTransfer::class.java)
        whenever(otherTransfer.transferTypeId).thenReturn(TransferType.Other)
        whenever(otherTransfer.amount).thenReturn(BigDecimal("30.00"))

        val deposits = listOf(
            creditDeposit,
            cashDeposit,
            debitDeposit,
            checkDeposit,
            giftCertDeposit,
            giftCardDeposit,
            customDeposit,
            ebtDeposit
        )

        val transfers = listOf(safeDropTransfer, otherTransfer)

        val totals = BatchTotals.populate(deposits, transfers)

        // Verify flags
        assertTrue(getBooleanField(totals, "mNeedsCreditDeposit"))
        assertTrue(getBooleanField(totals, "mNeedsCashDeposit"))
        assertTrue(getBooleanField(totals, "mNeedsDebitDeposit"))
        assertTrue(getBooleanField(totals, "mNeedsCheckDeposit"))
        assertTrue(getBooleanField(totals, "mNeedsGiftCertificateDeposit"))
        assertTrue(getBooleanField(totals, "mNeedsGiftCardDeposit"))
        assertTrue(getBooleanField(totals, "mNeedsCustomTendersDeposit"))
        assertTrue(getBooleanField(totals, "mNeedsEbtDeposit"))

        // Verify numeric totals (expected)
        // credit expected = 100.00, received = 101.50
        assertEquals(BigDecimal("100.00"), getBigDecimalField(totals, "mCreditExpectedTotal"))
        assertEquals(BigDecimal("101.50"), getBigDecimalField(totals, "mCreditReceivedTotal"))

        // cash expected = 50.00 + otherTransfer(30.00) = 80.00; received = orderAmount 49.00; tips = 5.00
        assertEquals(BigDecimal("80.00"), getBigDecimalField(totals, "mCashExpectedTotal"))
        assertEquals(BigDecimal("49.00"), getBigDecimalField(totals, "mCashReceivedTotal"))
        assertEquals(BigDecimal("5.00"), getBigDecimalField(totals, "mCashTips"))

        // debit
        assertEquals(BigDecimal("20.00"), getBigDecimalField(totals, "mDebitExpectedTotal"))
        assertEquals(BigDecimal("20.00"), getBigDecimalField(totals, "mDebitReceivedTotal"))

        // check
        assertEquals(BigDecimal("10.00"), getBigDecimalField(totals, "mCheckExpectedTotal"))
        assertEquals(BigDecimal("10.00"), getBigDecimalField(totals, "mCheckReceivedTotal"))

        // gift certificate
        assertEquals(BigDecimal("5.00"), getBigDecimalField(totals, "mGiftCertificateExpectedTotal"))
        assertEquals(BigDecimal("4.00"), getBigDecimalField(totals, "mGiftCertificateReceivedTotal"))

        // gift card
        assertEquals(BigDecimal("6.00"), getBigDecimalField(totals, "mGiftCardExpectedTotal"))
        assertEquals(BigDecimal("6.00"), getBigDecimalField(totals, "mGiftCardReceivedTotal"))

        // custom
        assertEquals(BigDecimal("2.00"), getBigDecimalField(totals, "mCustomTenderExpectedTotal"))
        assertEquals(BigDecimal("1.50"), getBigDecimalField(totals, "mCustomTenderReceivedTotal"))

        // ebt
        assertEquals(BigDecimal("7.00"), getBigDecimalField(totals, "mEbtExpectedTotal"))
        assertEquals(BigDecimal("7.00"), getBigDecimalField(totals, "mEbtReceivedTotal"))
    }

    /**
     * Test calculateOverShortTotals produces correct over/short values only when needs flag is true.
     */
    @Test
    fun `calculateOverShortTotals should compute over short only for needed deposit types`() {
        // create a simple BatchTotals and set private fields using reflection
        val totals = BatchTotals()

        // set needs flags and values via reflection
        val creditExpectedField = totals.javaClass.getDeclaredField("mCreditExpectedTotal")
        creditExpectedField.isAccessible = true
        creditExpectedField.set(totals, BigDecimal("100.00"))

        val creditReceivedField = totals.javaClass.getDeclaredField("mCreditReceivedTotal")
        creditReceivedField.isAccessible = true
        creditReceivedField.set(totals, BigDecimal("110.00"))

        // set boolean flag
        val needsCredit = totals.javaClass.getDeclaredField("mNeedsCreditDeposit")
        needsCredit.isAccessible = true
        needsCredit.setBoolean(totals, true)

        // cash: expected 80, received 70 -> overShort -10
        val cashExpected = totals.javaClass.getDeclaredField("mCashExpectedTotal")
        cashExpected.isAccessible = true
        cashExpected.set(totals, BigDecimal("80.00"))

        val cashReceived = totals.javaClass.getDeclaredField("mCashReceivedTotal")
        cashReceived.isAccessible = true
        cashReceived.set(totals, BigDecimal("70.00"))

        val needsCash = totals.javaClass.getDeclaredField("mNeedsCashDeposit")
        needsCash.isAccessible = true
        needsCash.setBoolean(totals, true)

        // debit NOT needed -> overShort should remain null
        val debitExpected = totals.javaClass.getDeclaredField("mDebitExpectedTotal")
        debitExpected.isAccessible = true
        debitExpected.set(totals, BigDecimal("20.00"))
        val debitReceived = totals.javaClass.getDeclaredField("mDebitReceivedTotal")
        debitReceived.isAccessible = true
        debitReceived.set(totals, BigDecimal("25.00"))
        val needsDebit = totals.javaClass.getDeclaredField("mNeedsDebitDeposit")
        needsDebit.isAccessible = true
        needsDebit.setBoolean(totals, false)

        // run calculation
        totals.calculateOverShortTotals()

        // check credit overShort = 110 - 100 = 10
        val creditOverShort = getNullableBigDecimalField(totals, "mCreditOverShort")
        assertNotNull("creditOverShort should be set", creditOverShort)
        assertEquals(BigDecimal("10.00"), creditOverShort)

        // cash overShort = 70 - 80 = -10
        val cashOverShort = getNullableBigDecimalField(totals, "mCashOverShort")
        assertNotNull("cashOverShort should be set", cashOverShort)
        assertEquals(BigDecimal("-10.00"), cashOverShort)

        // debit overShort should be null because needsDebit false
        val debitOverShort = getNullableBigDecimalField(totals, "mDebitOverShort")
        assertNull("debitOverShort should be null when not needed", debitOverShort)
    }

    /**
     * Test finalizeBatchDeposits handles empty input and skips deposits with null payment type.
     *
     * NOTE: finalizeBatchDeposits creates new BatchDepositDto(deposit) internally (copy constructor).
     * Intercepting that constructor requires more invasive bytecode/construction mocking (PowerMock,
     * or constructor mocking). To remain within Mockito + JUnit, here we:
     *  - verify that passing an empty list returns an empty list
     *  - verify that deposits with null paymentType are skipped (by providing a mocked deposit with null
     *    paymentType and asserting returned list is empty).
     *
     * For full verification of setDepositAmount/setOverShortAmount on the created objects you'd need
     * access to a testable BatchDepositDto copy constructor or to use constructor mocking tools.
     */
    @Test
    fun `finalizeBatchDeposits should return empty when input empty or when payment type null`() {
        val totals = BatchTotals()

        // case: empty input
        val resultEmpty = totals.finalizeBatchDeposits(emptyList())
        assertTrue(resultEmpty.isEmpty())

        // case: deposit with null payment type should be skipped
        val depositWithNullType = mock(BatchDepositDto::class.java)
        whenever(depositWithNullType.paymentType).thenReturn(null)

        val result = totals.finalizeBatchDeposits(listOf(depositWithNullType))
        assertTrue(result.isEmpty())
    }

    /**
     * Additional integration style test: use populate -> calculateOverShortTotals -> finalizeBatchDeposits
     * with an empty list for finalize (can't fully assert modified copied DTOs as explained).
     */
    @Test
    fun `populate then calculate then finalize (integration smoke)`() {
        val cashDeposit = mock(BatchDepositDto::class.java)
        whenever(cashDeposit.paymentType).thenReturn(BatchDepositDto.PaymentType.Cash)
        whenever(cashDeposit.totalAmount).thenReturn(BigDecimal("15.00"))
        whenever(cashDeposit.orderAmount).thenReturn(BigDecimal("15.00"))
        whenever(cashDeposit.tipAmount).thenReturn(BigDecimal("0.00"))

        val deposits = listOf(cashDeposit)
        val transfers = emptyList<TxDbBatchTransfer>()

        val totals = BatchTotals.populate(deposits, transfers)

        totals.calculateOverShortTotals()

        // finalize with empty list just to make sure finalize does not throw
        val finalized = totals.finalizeBatchDeposits(emptyList())
        assertNotNull(finalized)
        assertTrue(finalized.isEmpty())
    }
}




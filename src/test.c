#include "snns_slice.h"

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Various test fixtures
#ifndef SNNS_SLICE_TESTALLOCATION_SIZE
#define SNNS_SLICE_TESTALLOCATION_SIZE 20
#else
dont compile there is something wrong
#endif

static bool snns_Slice_testBool = false;

static bool snns_Slice_testAllocation_QQ_is_allocated = false;
static char snns_Slice_testAllocation_QQ[SNNS_SLICE_TESTALLOCATION_SIZE];
static bool snns_Slice_testAllocation_WW_is_allocated = false;
static char snns_Slice_testAllocation_WW[SNNS_SLICE_TESTALLOCATION_SIZE];
static bool snns_Slice_testAllocation_XX_is_allocated = false;
static char snns_Slice_testAllocation_XX[SNNS_SLICE_TESTALLOCATION_SIZE];
static bool snns_Slice_testAllocation_YY_is_allocated = false;
static char snns_Slice_testAllocation_YY[SNNS_SLICE_TESTALLOCATION_SIZE];
static bool snns_Slice_testAllocation_ZZ_is_allocated = false;
static char snns_Slice_testAllocation_ZZ[SNNS_SLICE_TESTALLOCATION_SIZE];

static void snns_Slice_test_fixtures_doClean_allocations(void)
{
    for (size_t i = 0; i < SNNS_SLICE_TESTALLOCATION_SIZE; ++i)
    {
        snns_Slice_testAllocation_QQ[i] = 'q';
        snns_Slice_testAllocation_WW[i] = 'w';
        snns_Slice_testAllocation_XX[i] = 'x';
        snns_Slice_testAllocation_YY[i] = 'y';
        snns_Slice_testAllocation_ZZ[i] = 'z';
    }
    snns_Slice_testAllocation_QQ_is_allocated = false;
    snns_Slice_testAllocation_WW_is_allocated = false;
    snns_Slice_testAllocation_XX_is_allocated = false;
    snns_Slice_testAllocation_YY_is_allocated = false;
    snns_Slice_testAllocation_ZZ_is_allocated = false;
}

static void snns_Slice_test_fixtures_assert_areClean_allocations(void)
{
    for (size_t i = 0; i < SNNS_SLICE_TESTALLOCATION_SIZE; ++i)
    {
        assert(snns_Slice_testAllocation_QQ[i] == 'q');
        assert(snns_Slice_testAllocation_WW[i] == 'w');
        assert(snns_Slice_testAllocation_XX[i] == 'x');
        assert(snns_Slice_testAllocation_YY[i] == 'y');
        assert(snns_Slice_testAllocation_ZZ[i] == 'z');
    }
    assert(snns_Slice_testAllocation_QQ_is_allocated == false);
    assert(snns_Slice_testAllocation_WW_is_allocated == false);
    assert(snns_Slice_testAllocation_XX_is_allocated == false);
    assert(snns_Slice_testAllocation_YY_is_allocated == false);
    assert(snns_Slice_testAllocation_ZZ_is_allocated == false);
}

static void snns_Slice_test_fixtures_doClean(void)
{
    snns_Slice_test_fixtures_doClean_allocations();

    snns_Slice_testBool = false;

    snns_Slice_memory.malloc = &malloc;
    snns_Slice_memory.calloc = &calloc;
    snns_Slice_memory.realloc = &realloc;
    snns_Slice_memory.free = &free;
}

static void snns_Slice_test_fixtures_assert_areClean(void)
{
    snns_Slice_test_fixtures_assert_areClean_allocations();
    assert(snns_Slice_testBool == false);
    assert(snns_Slice_memory.malloc == &malloc);
    assert(snns_Slice_memory.calloc == &calloc);
    assert(snns_Slice_memory.realloc == &realloc);
    assert(snns_Slice_memory.free == &free);
}

// Dependencies
static void *snns_Slice_testingMalloc(size_t s)
{
    if (SNNS_SLICE_TESTALLOCATION_SIZE < s)
    {
        return NULL;
    }
    else if (!snns_Slice_testAllocation_QQ_is_allocated)
    {
        snns_Slice_testAllocation_QQ_is_allocated = true;
        return (void *)snns_Slice_testAllocation_QQ;
    }
    else if (!snns_Slice_testAllocation_WW_is_allocated)
    {
        snns_Slice_testAllocation_WW_is_allocated = true;
        return (void *)snns_Slice_testAllocation_WW;
    }
    else if (!snns_Slice_testAllocation_XX_is_allocated)
    {
        snns_Slice_testAllocation_XX_is_allocated = true;
        return (void *)snns_Slice_testAllocation_XX;
    }
    else if (!snns_Slice_testAllocation_YY_is_allocated)
    {
        snns_Slice_testAllocation_YY_is_allocated = true;
        return (void *)snns_Slice_testAllocation_YY;
    }
    else if (!snns_Slice_testAllocation_ZZ_is_allocated)
    {
        snns_Slice_testAllocation_ZZ_is_allocated = true;
        return (void *)snns_Slice_testAllocation_ZZ;
    }
    else
    {
        return NULL;
    }

    assert("how did you get here" == NULL);
}

static void snns_Slice_testingFree(void *v)
{
    assert(v == NULL ||
           v == snns_Slice_testAllocation_QQ ||
           v == snns_Slice_testAllocation_WW ||
           v == snns_Slice_testAllocation_XX ||
           v == snns_Slice_testAllocation_YY ||
           v == snns_Slice_testAllocation_ZZ);
    if (v == NULL)
    {
        return;
    }
    else if (v == snns_Slice_testAllocation_QQ)
    {
        snns_Slice_testAllocation_QQ_is_allocated = false;
    }
    else if (v == snns_Slice_testAllocation_WW)
    {
        snns_Slice_testAllocation_WW_is_allocated = false;
    }
    else if (v == snns_Slice_testAllocation_XX)
    {
        snns_Slice_testAllocation_XX_is_allocated = false;
    }
    else if (v == snns_Slice_testAllocation_YY)
    {
        snns_Slice_testAllocation_YY_is_allocated = false;
    }
    else if (v == snns_Slice_testAllocation_ZZ)
    {
        snns_Slice_testAllocation_ZZ_is_allocated = false;
    }

    assert("Why are you freeing this pointer" == NULL);
}

static void *snns_Slice_testingRealloc(void *v, size_t s)
{
    assert(v == NULL ||
           v == snns_Slice_testAllocation_QQ ||
           v == snns_Slice_testAllocation_WW ||
           v == snns_Slice_testAllocation_XX ||
           v == snns_Slice_testAllocation_YY ||
           v == snns_Slice_testAllocation_ZZ);

    if (SNNS_SLICE_TESTALLOCATION_SIZE < s)
    {
        return NULL;
    }
    else if (!snns_Slice_testAllocation_QQ_is_allocated)
    {
        snns_Slice_testAllocation_QQ_is_allocated = true;
        memcpy((void *)snns_Slice_testAllocation_QQ, v, 20);
        snns_Slice_testingFree(v);
        return (void *)snns_Slice_testAllocation_QQ;
    }
    else if (!snns_Slice_testAllocation_WW_is_allocated)
    {
        snns_Slice_testAllocation_WW_is_allocated = true;
        memcpy((void *)snns_Slice_testAllocation_WW, v, 20);
        snns_Slice_testingFree(v);
        return (void *)snns_Slice_testAllocation_WW;
    }
    else if (!snns_Slice_testAllocation_XX_is_allocated)
    {
        snns_Slice_testAllocation_XX_is_allocated = true;
        memcpy((void *)snns_Slice_testAllocation_XX, v, 20);
        snns_Slice_testingFree(v);
        return (void *)snns_Slice_testAllocation_XX;
    }
    else if (!snns_Slice_testAllocation_YY_is_allocated)
    {
        snns_Slice_testAllocation_YY_is_allocated = true;
        memcpy((void *)snns_Slice_testAllocation_YY, v, 20);
        snns_Slice_testingFree(v);
        return (void *)snns_Slice_testAllocation_YY;
    }
    else if (!snns_Slice_testAllocation_ZZ_is_allocated)
    {
        snns_Slice_testAllocation_ZZ_is_allocated = true;
        memcpy((void *)snns_Slice_testAllocation_ZZ, v, 20);
        snns_Slice_testingFree(v);
        return (void *)snns_Slice_testAllocation_ZZ;
    }
    else
    {
        return NULL;
    }
}

static void snns_Slice_test_fixtures_set_up_custom_memory_handlers(void)
{
    snns_Slice_memory.malloc = &snns_Slice_testingMalloc;
    snns_Slice_memory.free = &snns_Slice_testingFree;
    snns_Slice_memory.realloc = &snns_Slice_testingRealloc;
}

static void *snns_Slice_reporting_malloc(size_t s)
{
    assert(snns_Slice_testBool == false);
    snns_Slice_testBool = s || true;
    return NULL;
}

static void *snns_Slice_reporting_realloc(void *v, size_t s)
{
    assert(snns_Slice_testBool == false);
    snns_Slice_testBool = (size_t)v || s || true;
    return NULL;
}

static void snns_Slice_reporting_free(void *v)
{
    assert(snns_Slice_testBool == false);
    snns_Slice_testBool = (size_t)v || true;
}

static bool snns_ok(snns_Slice_Result result)
{
    return result == 0;
}

// And the actual tests

static void snns_Slice_isInit_discriminates_between_Init_and_NotInit(void)
{
    snns_Slice this;

    // isInit will accept an Init slice
    {
        this.arr = NULL;
        this.cap = 0;
        assert(snns_Slice_isInit(&this));
    }

    // isInit will reject a slice where cap and arr are set to non-zero values
    {
        this.arr = (void *)1;
        this.cap = 1;
        assert(snns_Slice_isInit(&this) == false);
    }

    // isInit does not return true early if just arr is Init
    {
        this.arr = NULL;
        this.cap = 1;
        assert(snns_Slice_isInit(&this) == false);
    }

    // isInit does not test in opposite order and
    // return true early if just cap is Init
    {
        this.arr = (void *)1;
        this.cap = 0;
        assert(snns_Slice_isInit(&this) == false);
    }

    return;
}

static void snns_Slice_doInit_initializes_slices(void)
{
    snns_Slice this;

    {
        // doInit doesn't change you AWAY from Init state
        this.arr = NULL;
        this.cap = 0;
        snns_Slice_doInit(&this);
        assert(snns_Slice_isInit(&this));
    }

    {
        // doInit resets arr to Init state
        this.arr = (void *)1;
        this.cap = 0;
        snns_Slice_doInit(&this);
        assert(snns_Slice_isInit(&this));
    }

    {
        // doInit resets cap to Init state
        this.arr = NULL;
        this.cap = 1;
        snns_Slice_doInit(&this);
        assert(snns_Slice_isInit(&this));
    }

    {
        // doInit does not early return after setting arr
        this.arr = (void *)1;
        this.cap = 1;
        snns_Slice_doInit(&this);
        assert(snns_Slice_isInit(&this));
    }
}

static void snns_Slice_makeNew_create_a_slice_in_Init_state(void)
{
    snns_Slice this = snns_Slice_makeNew();

    assert(snns_Slice_isInit(&this));
}

static void snns_Slice_Init_testGroup(void)
{
    snns_Slice_isInit_discriminates_between_Init_and_NotInit();
    snns_Slice_doInit_initializes_slices();
    snns_Slice_makeNew_create_a_slice_in_Init_state();
}

static void snns_Slice_isClear_will_report_true_on_an_Init_slice(void)
{
    snns_Slice this = snns_Slice_makeNew();
    assert(snns_Slice_isClear(&this));
}

static void snns_Slice_isClear_will_report_true_on_Alloc_slices_where_all_bytes_are_null(void)
{

    // Alloc a slice
    snns_Slice this = snns_Slice_makeNew();
    snns_Slice_Result alloc_result = snns_Slice_zAlloc(&this, 10);

    // Zero out the content
    assert(alloc_result == snns_Slice_Result_ok);
    for (size_t i = 0; i < this.cap; ++i)
    {
        ((this.arr))[i] = 0;
    }

    // Assert a zeroed slice isClear
    assert(snns_Slice_isClear(&this));

    // cleanup
    snns_Slice_deAlloc(&this);
}

static void snns_Slice_isClear_will_report_false_on_Alloc_slices_where_any_byte_is_not_null(void)
{
    // Alloc a slice
    snns_Slice this = snns_Slice_makeNew();
    snns_Slice_Result alloc_result = snns_Slice_zAlloc(&this, 10);

    // Zero out the content to start with a clean slice
    assert(alloc_result == snns_Slice_Result_ok);
    for (size_t i = 0; i < this.cap; ++i)
    {
        ((this.arr))[i] = 0;
    }

    // Alerts on first byte
    ((this.arr))[0] = 1;
    assert(!snns_Slice_isClear(&this));
    ((this.arr))[0] = 0;

    // Alerts on last byte
    ((this.arr))[this.cap - 1] = 127;
    assert(!snns_Slice_isClear(&this));
    ((this.arr))[this.cap - 1] = 0;

    // Alerts on multiple set bytes
    ((this.arr))[2] = 60;
    ((this.arr))[3] = 120;
    assert(!snns_Slice_isClear(&this));
    ((this.arr))[2] = 0;
    ((this.arr))[3] = 0;

    // Alerts on all set bytes
    for (size_t i = 0; i < this.cap; ++i)
    {
        (this.arr)[i] = (char)(i + 1);
    }
    assert(!snns_Slice_isClear(&this));

    snns_Slice_deAlloc(&this);
}

static void snns_Slice_isClear_testGroup(void)
{
    snns_Slice_isClear_will_report_true_on_an_Init_slice();
    snns_Slice_isClear_will_report_true_on_Alloc_slices_where_all_bytes_are_null();
    snns_Slice_isClear_will_report_false_on_Alloc_slices_where_any_byte_is_not_null();
}

static void snns_Slice_doClear_leaves_Init_slices_unchanged(void)
{
    snns_Slice this = snns_Slice_makeNew();

    assert(snns_Slice_isInit(&this));

    snns_Slice_doClear(&this);

    assert(snns_Slice_isInit(&this));
}

static void snns_Slice_doClear_leaves_zeroed_Alloc_slices_unchanged(void)
{
    // Arrange
    snns_Slice this = snns_Slice_makeNew();
    snns_Slice_Result result = snns_Slice_zAlloc(&this, 5);
    assert(result == snns_Slice_Result_ok);

    // Act
    snns_Slice_doClear(&this);

    // Assert
    assert(snns_Slice_isClear(&this));

    // Cleanup
    snns_Slice_deAlloc(&this);
}

static void snns_Slice_doClear_zeroes_out_Alloc_slices_with_set_bytes(void)
{
    // ARRANGE
    // - Alloc a slice
    snns_Slice this = snns_Slice_makeNew();
    snns_Slice_Result alloc_result = snns_Slice_zAlloc(&this, 10);
    assert(alloc_result == snns_Slice_Result_ok);

    // ARRANGE
    // - Fill it with something
    for (size_t i = 0; i < this.cap; ++i)
    {
        this.arr[i] = 28;
    }

    // ACT
    snns_Slice_doClear(&this);

    // ASSERT
    assert(snns_Slice_isClear(&this));

    // Clean
    snns_Slice_deAlloc(&this);
}

static void snns_Slice_doClear_testGroup(void)
{
    snns_Slice_doClear_leaves_Init_slices_unchanged();
    snns_Slice_doClear_leaves_zeroed_Alloc_slices_unchanged();
    snns_Slice_doClear_zeroes_out_Alloc_slices_with_set_bytes();
}

static void snns_Slice_memory_malloc_can_be_replaced(void)
{
    // Arrange
    snns_Slice this = snns_Slice_makeNew();
    snns_Slice_testBool = false;
    snns_Slice_memory.malloc = &snns_Slice_reporting_malloc;

    // Act
    snns_Slice_zAlloc(&this, 2);

    // Assert
    assert(snns_Slice_testBool == true);

    // cleanup
    snns_Slice_testBool = false;
    snns_Slice_memory.malloc = &malloc;
}

static void snns_Slice_memory_free_can_be_replaced(void)
{
    // Arrange
    snns_Slice this = {
        .arr = (void *)2,
        .cap = 2};
    snns_Slice_memory.free = &snns_Slice_reporting_free;
    snns_Slice_testBool = false;

    // Act
    snns_Slice_deAlloc(&this);

    // Assert
    assert(snns_Slice_testBool == true);

    // Cleanup
    snns_Slice_memory.free = &free;
    snns_Slice_testBool = false;
}

static void snns_Slice_memory_realloc_can_be_replaced(void)
{
    // Arrange
    snns_Slice this = {
        .arr = (void *)2,
        .cap = 2};
    snns_Slice_memory.realloc = &snns_Slice_reporting_realloc;
    snns_Slice_testBool = false;

    // Act
    snns_Slice_reAlloc(&this, this.cap + 1);

    // Assert
    assert(snns_Slice_testBool == true);

    // Cleanup
    snns_Slice_memory.realloc = &realloc;
    snns_Slice_testBool = false;
}

static void snns_Slice_MemoryFunctions_testGroup(void)
{
    snns_Slice_memory_malloc_can_be_replaced();
    snns_Slice_memory_free_can_be_replaced();
    snns_Slice_memory_realloc_can_be_replaced();

    // cannot test calloc because no function uses it yet
    // snns_Slice_memory_calloc_can_be_replaced();
}

static void snns_Slice_when_zAlloc_zero_bytes_to_Init_Slice(void)
{
    /*
    Expected outcome of allocating zero bytes to a slice
        that is in the Init state:
        (1) malloc is NOT called;
        (2) zAlloc returns Result_ok
        (3) Init has NOT changed state.
    */

    // Arrange
    snns_Slice this = snns_Slice_makeNew();
    snns_Slice_Result result;
    snns_Slice_testBool = false;
    snns_Slice_memory.malloc = &snns_Slice_reporting_malloc;

    // Act
    result = snns_Slice_zAlloc(&this, 0);

    // Assert
    //      (1) reporting_malloc did not get called and set
    //          testBool to true;
    assert(snns_Slice_testBool == false);

    //      (2) and that's fine - you didn't want anything
    //          malloced and didn't GET anything malloced
    assert(snns_ok(result));

    //      (3) So the slice is still in Init state
    assert(snns_Slice_isInit(&this));

    // Cleanup
    snns_Slice_testBool = false;
    snns_Slice_memory.malloc = &malloc;
}

static void snns_Slice_when_zAlloc_any_bytes_to_non_Init_slice(void)
{
    /*
    Expected outcome of allocating any bytes to a slice
    that is NOT in the Init state:
    (1) malloc is NOT called;
    (2) zAlloc returns Result_alreadyAllocated
    (3) slice has NOT changed state.
    */

    // Arrange
    snns_Slice_Result result;
    snns_Slice this = {.cap = 20, .arr = (void *)snns_Slice_testAllocation_QQ};
    snns_Slice_testBool = false;
    snns_Slice_memory.malloc = &snns_Slice_reporting_malloc;

    // Act
    result = snns_Slice_zAlloc(&this, 20);

    // Assert
    //      (1) reporting_malloc did not get called and set
    //          testBool to true;
    assert(snns_Slice_testBool == false);

    //      (2) but it's probably still a mistake, why are
    //          you allocating to a slice that is already
    //          allocated? Better let you know
    assert(result == snns_Slice_Result_alreadyAllocated);

    //      (3) slice had better be exactly as it started
    assert(this.cap == 20 && this.arr == (void *)snns_Slice_testAllocation_QQ);
}

static void snns_Slice_when_zAlloc_ten_bytes_to_Init_slice(void)
{
    /*
    Expected outcome of allocating 10 bytes to a slice
    "this" that is in the Init state:
    (1) zAlloc returns Result_OK
    (2) this->cap == 10 && this->arr == the address
        of a memory buffer. Because we are using a test
        fixture malloc, we will check if this->arr points
        to the specific memory location we know we'll get
        back from that malloc implementation.
    */
    
    // Arrange
    snns_Slice_Result result;
    snns_Slice this = snns_Slice_makeNew();
    snns_Slice_test_fixtures_doClean();
    snns_Slice_test_fixtures_set_up_custom_memory_handlers();
    

    // Act
    result = snns_Slice_zAlloc(&this, 10);

    // Assert
    assert(snns_ok(result));
    assert(this.cap == 10);
    assert(this.arr == snns_Slice_testAllocation_QQ);

    // Cleanup
    snns_Slice_test_fixtures_doClean();
    snns_Slice_test_fixtures_assert_areClean();
}

static void snns_Slice_when_zAlloc_thirty_bytes_to_Init_slice(void)
{}

static void snns_Slice_zAlloc_testGroup(void)
{
    snns_Slice_when_zAlloc_zero_bytes_to_Init_Slice();
    snns_Slice_when_zAlloc_any_bytes_to_non_Init_slice();
    snns_Slice_when_zAlloc_ten_bytes_to_Init_slice();
    snns_Slice_when_zAlloc_thirty_bytes_to_Init_slice();
}

int main(int argc, char **argv)
{
    puts(argv[0 * argc]);

    snns_Slice_Init_testGroup();
    snns_Slice_isClear_testGroup();
    snns_Slice_doClear_testGroup();
    snns_Slice_MemoryFunctions_testGroup();
    snns_Slice_zAlloc_testGroup();

    puts("---All tests passed");
}

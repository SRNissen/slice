#include "snns_slice.h"

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

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

static void snns_Slice_isClear_will_report_true_on_an_Init_slice(void)
{
    snns_Slice this = snns_Slice_makeNew();
    assert(snns_Slice_isClear_linearN(&this));
}

static void snns_Slice_isClear_will_report_true_on_Alloc_slices_where_all_bytes_are_null(void)
{

    // Alloc a slice
    snns_Slice this = snns_Slice_makeNew();
    snns_Slice_Result alloc_result = snns_Slice_calloc(&this, 10);

    // Zero out the content
    assert(alloc_result == snns_Slice_Result_ok);
    for (size_t i = 0; i < this.cap; ++i)
    {
        ((this.arr))[i] = 0;
    }

    // Assert a zeroed slice isClear
    assert(snns_Slice_isClear_linearN(&this));

    // cleanup
    snns_Slice_dealloc(&this);
}

static void snns_Slice_isClear_will_report_false_on_Alloc_slices_where_any_byte_is_not_null(void)
{
    // Alloc a slice
    snns_Slice this = snns_Slice_makeNew();
    snns_Slice_Result alloc_result = snns_Slice_calloc(&this, 10);

    // Zero out the content to start with a clean slice
    assert(alloc_result == snns_Slice_Result_ok);
    for (size_t i = 0; i < this.cap; ++i)
    {
        ((this.arr))[i] = 0;
    }

    // Alerts on first byte
    ((this.arr))[0] = 1;
    assert(!snns_Slice_isClear_linearN(&this));
    ((this.arr))[0] = 0;

    // Alerts on last byte
    ((this.arr))[this.cap - 1] = 127;
    assert(!snns_Slice_isClear_linearN(&this));
    ((this.arr))[this.cap - 1] = 0;

    // Alerts on multiple set bytes
    ((this.arr))[2] = 60;
    ((this.arr))[3] = 120;
    assert(!snns_Slice_isClear_linearN(&this));
    ((this.arr))[2] = 0;
    ((this.arr))[3] = 0;

    // Alerts on all set bytes
    for (size_t i = 0; i < this.cap; ++i)
    {
        (this.arr)[i] = (char)(i + 1);
    }
    assert(!snns_Slice_isClear_linearN(&this));

    snns_Slice_dealloc(&this);
}

static void snns_Slice_isClear_Alloc_group(void)
{
    snns_Slice_isClear_will_report_true_on_Alloc_slices_where_all_bytes_are_null();
    snns_Slice_isClear_will_report_false_on_Alloc_slices_where_any_byte_is_not_null();
}

static void snns_Slice_isClear_testGroup(void)
{
    snns_Slice_isClear_will_report_true_on_an_Init_slice();
    snns_Slice_isClear_Alloc_group();
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
    snns_Slice_Result result = snns_Slice_calloc(&this, 5);
    assert(result == snns_Slice_Result_ok);

    // Act
    snns_Slice_doClear(&this);

    // Assert
    assert(snns_Slice_isClear_linearN(&this));

    // Cleanup
    snns_Slice_dealloc(&this);
}

static void snns_Slice_doClear_zeroes_out_Alloc_slices_with_set_bytes(void)
{
    // ARRANGE
    // - Alloc a slice
    snns_Slice this = snns_Slice_makeNew();
    snns_Slice_Result alloc_result = snns_Slice_calloc(&this, 10);
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
    assert(snns_Slice_isClear_linearN(&this));

    // Clean
    snns_Slice_dealloc(&this);
}

static void snns_Slice_doClear_testGroup()
{
    snns_Slice_doClear_leaves_Init_slices_unchanged();
    snns_Slice_doClear_leaves_zeroed_Alloc_slices_unchanged();
    snns_Slice_doClear_zeroes_out_Alloc_slices_with_set_bytes();
}

int main()
{
    puts("\nTesting snns_Slice functions");

    snns_Slice_isInit_discriminates_between_Init_and_NotInit();
    snns_Slice_doInit_initializes_slices();
    snns_Slice_makeNew_create_a_slice_in_Init_state();
    snns_Slice_isClear_testGroup();
    snns_Slice_doClear_testGroup();

    puts("All tests passed");
}

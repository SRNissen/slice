#include "snns_slice.h"

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

void snns_Slice_isInit_discriminates_between_Init_and_NotInit()
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

void snns_Slice_doInit_initializes_slices()
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

void snns_Slice_makeNew_create_a_slice_in_Init_state()
{
    snns_Slice this = snns_Slice_makeNew();
        
    assert(snns_Slice_isInit(&this));
}

void snns_Slice_isClear_will_report_true_on_an_init_slice()
    {
        
    }
    
    snns_Slice_isClear_will_report_false_on_nonInit_slices()
    {
        
    }

void snns_Slice_isClear_testGroup()
{
    snns_Slice_isClear_will_report_true_on_an_init_slice();
    snns_Slice_isClear_will_report_false_on_nonInit_slices();
}

int main()
{
    puts("Testing snns_Slice functions");

    snns_Slice_isInit_discriminates_between_Init_and_NotInit();
    snns_Slice_doInit_initializes_slices();
    snns_Slice_makeNew_create_a_slice_in_Init_state();
    snns_Slice_isClear_testGroup();
}

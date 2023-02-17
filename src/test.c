#include "snns_slice.h"

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

void isInit_discriminates_between_Init_and_NotInit()
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

void doInit_initializes_slices()
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

void makeNew_create_a_slice_in_Init_state()
{
    snns_Slice this = snns_Slice_makeNew();
        
    assert(snns_Slice_isInit(&this));
}

int main()
{
    puts("Testing snns_Slice functions");

    isInit_discriminates_between_Init_and_NotInit();
    doInit_initializes_slices();
    makeNew_create_a_slice_in_Init_state();
}

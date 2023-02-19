#include "snns_slice.h"
#include <string.h>
#include <stdlib.h>

snns_Slice_Allocator_Ptr snns_Slice_malloc_ptr = &malloc;
snns_Slice_CAllocator_Ptr snns_Slice_calloc_ptr = &calloc;
snns_Slice_ReAllocator_Ptr snns_Slice_realloc_ptr = &realloc;
snns_Slice_Free_Ptr snns_slice_free_ptr = &free;

static bool snns_ok(snns_Slice_Result result)
{
    return result == 0;
}

snns_Slice snns_Slice_makeNew(void)
{
    snns_Slice this;
    snns_Slice_doInit(&this);
    return this;
}

void snns_Slice_doInit(snns_Slice *this)
{
    memset(this, 0, sizeof(snns_Slice));
}

bool snns_Slice_isInit(snns_Slice const *this)
{
    return this->arr == NULL &&
           this->cap == 0;
}

bool snns_Slice_isClear(snns_Slice const *this)
{
    char clear = 0;

    for (size_t i = 0; i < this->cap; ++i)
        clear |= ((char *)(this->arr))[i];

    return clear == 0;
}

void snns_Slice_doClear(snns_Slice *this)
{
    if (this->cap == 0)
    {
        return;
    }
    else
    {
        memset(this->arr, 0, this->cap);
        return;
    }
}

snns_Slice_Result snns_Slice_zAlloc(snns_Slice *this, size_t desired_minimum_capacity)
{
    if (!snns_Slice_isInit(this))
    {
        return snns_Slice_Result_alreadyAllocated;
    }
    else if (desired_minimum_capacity == 0)
    {
        return snns_Slice_Result_ok;
    }
    else
    {
        void *new_buffer = malloc(desired_minimum_capacity);

        if (!new_buffer)
        {
            return snns_Slice_Result_badAlloc;
        }
        else
        {
            memset(new_buffer, 0, desired_minimum_capacity);
            this->arr = new_buffer;
            this->cap = desired_minimum_capacity;
            return snns_Slice_Result_ok;
        }
    }
}

snns_Slice_Result snns_Slice_reAlloc(snns_Slice *this, size_t desired_minimum_capacity)
{
    if (snns_Slice_isInit(this))
    {
        return snns_Slice_zAlloc(this, desired_minimum_capacity);
    }
    else if (desired_minimum_capacity <= this->cap)
    {
        return snns_Slice_Result_ok;
    }
    else
    {
        void *new_arr = realloc(this->arr, desired_minimum_capacity);

        if (!new_arr)
        {
            return snns_Slice_Result_badAlloc;
        }
        else
        {
            this->arr = new_arr;
            memset((this->arr) + (this->cap), 0, desired_minimum_capacity - (this->cap));
            this->cap = desired_minimum_capacity;
            return snns_Slice_Result_ok;
        }
    }
}

void snns_Slice_deAlloc(snns_Slice *this)
{
    free(this->arr);
    snns_Slice_doInit(this);
}

snns_Slice_Result snns_Slice_copy(snns_Slice *to_this, snns_Slice const *from_other)
{
    if (snns_Slice_isInit(to_this) && snns_Slice_isInit(from_other))
    {
        return snns_Slice_Result_ok;
    }
    else if (to_this->cap == from_other->cap)
    {
        memcpy(to_this->arr, from_other->arr, from_other->cap);
        return snns_Slice_Result_ok;
    }
    else if (to_this->cap < from_other->cap)
    {
        snns_Slice_Result reallocResult = snns_Slice_reAlloc(to_this, from_other->cap);

        if (!snns_ok(reallocResult))
        {
            return reallocResult;
        }
        else
        {
            memcpy(to_this->arr, from_other->arr, from_other->cap);
            return snns_Slice_Result_ok;
        }
    }
    else
    {
        memcpy(to_this->arr, from_other->arr, from_other->cap);
        memset(to_this->arr + from_other->cap, 0, to_this->cap - from_other->cap);
        return snns_Slice_Result_ok;
    }
}

snns_Slice_Result snns_Slice_append(snns_Slice *append_to_this, snns_Slice const *append_from_other)
{
    if (snns_Slice_isInit(append_from_other))
    {
        return snns_Slice_Result_ok;
    }
    else if (snns_Slice_isInit(append_to_this))
    {
        return snns_Slice_copy(append_to_this, append_from_other);
    }
    else // if both slices have data
    {
        size_t sum_capacity = (append_to_this->cap) + (append_from_other->cap);

        if (sum_capacity < append_to_this->cap || sum_capacity < append_from_other->cap)
        {
            // You have somehow managed to overflow size_t - well done, you must have a lot of data.
            return snns_Slice_Result_badAlloc;
        }
        else
        {
            snns_Slice_Result reallocResult = snns_Slice_reAlloc(append_to_this, sum_capacity);

            if (!snns_ok(reallocResult))
            {
                return reallocResult;
            }
            else
            {
                memcpy((append_to_this->arr) + (append_to_this->cap),
                       append_from_other->arr,
                       append_from_other->cap);
                return snns_Slice_Result_ok;
            }
        }
    }
}

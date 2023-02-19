#ifndef SNNS_SLICE_H
#define SNNS_SLICE_H

#include <stddef.h>  //For bool params
#include <stdbool.h> //For size_t params

// namespace snns
/**********************************************************
    Søren Nissen Name Space
**********************************************************/



// type Slice
/**********************************************************
    a dynamic array of bytes that knows its own length

Slices are always in one of four states:

- UnInit
    A slice that has been declared with no values set by
    any programmer or function call

- Init
    this->arr == null
    this->cap == 0

- Alloc
    this->arr holds the address of a byte buffer
    this->cap holds the count of bytes in *(this->arr)

- Unknown
    Any slice that is not known to be UnInit, Init or Alloc
**********************************************************/
typedef struct snns_Slice
{
    char *arr;
    size_t cap;
} snns_Slice;



// type Slice_Result
/**********************************************************
    an enumration of Slice operation outcomes.
**********************************************************/
typedef enum snns_Slice_Result
{
    snns_Slice_Result_ok = 0,
    snns_Slice_Result_alreadyAllocated = 1,
    snns_Slice_Result_badAlloc = 2,
} snns_Slice_Result;



// makeNew
/**********************************************************
    Returns a Slice in Init state
**********************************************************/
snns_Slice snns_Slice_makeNew(void);



// void doInit(this)
/**********************************************************
    Sets a slice to Init state (does not deallocate)

(this)
Init:
        |(this) is in the Init state

UnInit:
        |(this) is in the Init state

Alloc:
        |(this) is in the Init state
        |
        |The allocation leaks

Unknown:
        |(this) is in the Init state
        |
        |If allocated, the allocation leaks. 
**********************************************************/
void snns_Slice_doInit(
    snns_Slice *this);



// bool isInit(this)
/**********************************************************
    Tests if (this) is in the Init state

(this)
Init:
        |(this) is unchanged
        |
        |returns true
        
Else:
        |(this) is unchanged
        |
        |returns false
**********************************************************/
bool snns_Slice_isInit(
    snns_Slice const *this); 



// bool isClear(this)
/**********************************************************
    Tests if (this->arr) is composed entirely of null bytes
    NB: Does this with a linear walk of every byte.

(this)
Init:
        |(this) is unchanged
        |
        |returns true
    
Alloc:
        |(this) is unchanged
        |
        |Starting from *(this->arr), the next (this->cap)
        |bytes are null
        |
        |returns true
    OR
        |(this) is unchanged
        |
        |Starting from *(this->arr), there is at least 1
        |non-null byte within the next (this->cap) bytes;
        |
        |returns false

Else:
        |Undefined behavior
**********************************************************/
bool snns_Slice_isClear(
    snns_Slice const *this);



// void doClear(this)
/**********************************************************
    Memset slice to null-bytes.
    
! ! Uses Memset! Not a security function ! !

    If you need a secure memset before dealloc, use
    memset_explicit (C23) or, if you don't have C23, look
    up "secure memset" in your favorite reference source

(this)
Init:
        |(this) is unchanged

Alloc:
        |All allocated bytes set to null

Else:
        |Undefined behavior
**********************************************************/
void snns_Slice_doClear(
    snns_Slice *this);






// Result zAlloc(this, desired_minimum_capacity)
/**********************************************************
    Allocates desired capacity of 0-initialized bytes.

(desired_minimum_capacity == 0)
(this)
Init:
        |(this) is unchanged
        |
        |returns Result_ok
        
Else:
        |(this) is unchanged
        |
        |returns Result_alreadyAllocated


(desired_minimum_capacity != 0)
(this)
Init:
        |(this) changes state to Alloc
        |
        |this->arr is assigned the desired allocation 
        |
        |this->cap is assigned the count of allocated bytes
        |
        |Allocated bytes are zero-initialized
        |
        |returns Result_ok
    OR 
        |(this) is unchanged
        |
        |returns Result_badAlloc

Else:
        |(this) is unchanged
        |
        |returns Result_alreadyAllocated
**********************************************************/
snns_Slice_Result snns_Slice_zAlloc(
    snns_Slice *this,
    size_t desired_minimum_capacity);



// Result reAlloc(this, desired_minimum_capacity)
/**********************************************************
    Grows allocation to desired capacity. Does not shink
    allocation, even if a smaller allocation is requested

(desired_minimum_capacity == 0)
(this)
Init:
        |(this) is unchanged
        |
        |returns Result_ok
        
Else:
        |(this) is unchanged
        |
        |returns Result_alreadyAllocated


(desired_minimum_capacity <= (this->cap))
(this)
Alloc:
        |(this) is unchanged
        |
        |returns Result_ok

Else:
        |Undefined behavior


(desired_minimum_capacity > (this->cap))
(this)
Alloc:
        |this->arr is holds the address of a location of
        |the desired size 
        |
        |this->cap is assigned the count of allocated bytes
        |
        |this->arr[0] to this->arr[old_cap-1] are unchanged
        |
        |this->arr[old_cap] to this->arr[new_cap-1] are null
        |
        |returns Result_ok;
    OR
        |(this) is unchanged
        |
        |returns Result_badAlloc
        
Else:
        |Undefined behavior
**********************************************************/
snns_Slice_Result snns_Slice_reAlloc(
    snns_Slice *this,
    size_t desired_minimum_capacity);



// void deAlloc(this)
/**********************************************************
    Deallocates any buffer and returns (this) to Init state
    
(this)
Init:
        |(this) is unchanged

Alloc:
        |(this) changes to Init state

Else:
        |Undefined behavior
**********************************************************/
void snns_Slice_deAlloc(snns_Slice *this);



// Result copy(to_this, const from_other)
/**********************************************************
    Overwrites content of (to_this) with the content
    of (from_other). Reallocates if necessary.
    
(this, other)
Init,Init:
        |(other) is unchanged
        |
        |(this) is unchanged
        |
        |returns Result_ok

Init,Alloc:
        |(other) is unchanged
        |
        |(this) changes to Alloc
        |
        |lhs->arr points to a buffer that holds the same
        |content as other->arr
        |
        |this->cap is set to the same value as other->cap
        |
        |returns Result_ok
    OR
        |(other) is unchanged
        |
        |(this) is unchanged
        |
        |returns Result_badAlloc

(this->cap >= other->cap)
Alloc,Init:
Alloc,Alloc:
        |(other) is unchanged
        |
        |(this) remains Alloc
        |
        |this->cap is unchanged
        |
        |this->arr is unchanged
        |
        |other->arr[0 to (other->cap)-1] are copied into
        |this->arr[0 to (other->cap)-1]
        |
        |this->arr[other->cap to (this->cap)-1] are set to null
        |
        |returns Result_ok

(this->cap < other->cap)
Alloc,Alloc:
        |(other) is unchanged
        |
        |(this) remains Alloc
        |
        |this->cap = other->cap
        |
        |this->arr is replaced with a pointer to a byte
        |array of size other->cap
        |
        |other->arr[0 to (other->cap)-1] are copied into
        |this->arr[0 to (other->cap)-1]
        |
        |returns Result_ok
    OR
        |(other) is unchanged
        |
        |(this) is unchanged
        |
        |returns Result_badAlloc

Else:
        |Undefined behavior
**********************************************************/
snns_Slice_Result snns_Slice_copy(
    snns_Slice *to_this,
    snns_Slice const *from_other);



// Result append(append_to_this, const append_from_other)
/**********************************************************
    Appends the content of (from_other->arr) to the end of
    (to_this->arr). Reallocates if necessary
    
(this, other)
Init,Init:
        |(other) is unchanged
        |
        |(this) is unchanged
        |
        |returns Result_ok

Aloc,Init:
        |(other) is unchanged
        |
        |(this) is unchanged
        |
        |returns Result_ok

Init,Aloc:
        |(other) is unchanged
        |
        |(this) changes to Alloc
        |
        |lhs->arr points to a buffer that holds the same
        |content as other->arr
        |
        |this->cap is set to the same value as other->cap
        |
        |returns Result_ok
    OR
        |(other) is unchanged
        |
        |(this) is unchanged
        |
        |returns Result_badAlloc

Aloc,Aloc:
        |(other) is unchanged
        |
        |(this) remains Aloc
        |
        |this->arr is replaced with a pointer to a byte
        |array of size (this->cap)+(other->cap)
        |
        |this->arr[0 to this->cap-1] is unchanged
        |
        |other->arr[0 to other->cap-1] are copied into
        |this->arr[this->cap to this->cap+other->cap-1]
        |
        |this->cap is set to (this->cap)+(other->cap)
        |
        |returns Result_ok
    OR
        |(other) is unchanged
        |
        |(this) is unchanged
        |
        |returns Result_badAlloc
**********************************************************/
snns_Slice_Result snns_Slice_append(
    snns_Slice *append_to_this,
    snns_Slice const *append_from_other);



// extern snns_Slice_memory
/**********************************************************
    Slice memory is managed by through pointers from the 
    extern struct snns_Slice_memory.
   
    If you do not modify them, their default targets are
        |malloc
        |realloc
        |calloc
        |free 
**********************************************************/
typedef struct snns_Slice_MemoryFunctions
{
    void*(*malloc)(size_t);
    void*(*calloc)(size_t,size_t);
    void*(*realloc)(void*,size_t);
    void(*free)(void*);
    } snns_Slice_MemoryFunctions;

extern snns_Slice_MemoryFunctions snns_Slice_memory;

#endif

# Lib snns_Slice

Convenience functions for a C byte slice

```C
    typedef struct snns_Slice
    {
        void *arr;
        size_t cap;
    } snns_Slice;
```

## Functions

All functions are described in more detail in `snns_slice.h`

```C
    //Returns an initialized slice
    snns_Slice snns_Slice_makeNew(void); 
```

```C
    //Initialized an existing slice
    void snns_Slice_doInit(snns_Slice*);
```

```C
    //Tests if a slice is initialized
    bool snns_Slice_isInit(snns_Slice*);
```

```C
    //Sets all allocated bytes to zero
    void snns_Slice_doClear(snns_Slice*);
```

```C
    //Tests if all allocated bytes are zero
    bool snns_Slice_isClear_linearN(snns_Slice*);
```

```C
    //Allocates a zeroed out array of bytes to the slice
    snns_Slice_Result snns_Slice_calloc(snns_Slice*, size_t);
```

```C
    //Re-allocates a slice if it is shorter than requested
    snns_Slice_Result snns_Slice_calloc(snns_Slice*, size_t);
```

```C
    //Deallocates a slice and sets it to the Init state
    void snns_Slice_dealloc(snns_Slice*);
```

```C
    //Overwrites content of first slice with content of second slice
    snns_Slice_Result snns_Slice_copy(snns_Slice*, snns_Slice*);
```

```C
    //Appends content of second slice to first slice
    snns_Slice_Result snns_Slice_append(snns_Slice*, snns_Slice*);
```
# BUG 1 -
## Category
Iteration error

## Description
The loop gets out-of-bound for filter_negative 

## Affected Lines in the original program
In `filter.c:118-119`

## Expected vs Observed
We expect the negative filter to correctly change the image without crashing.

## Steps to Reproduce
./filter test_imgs/ck.png output negative

## Suggested Fix Description
Replace <= by < in the loop

# BUG 2 -
## Category
Unchecked memory allocation

## Description
Memory allocation for neg not checked, might segfault if memory is low.

## Affected Lines in the original program
In `filter.c:141`

## Steps to Reproduce
./filter test_imgs/ck.png output negative

## Suggested Fix Description
Add check and return if pointer is NULL
if (!neg) {
    exit(1);
}

# BUG 3 -
## Category
Memory leak

## Description
Memory allocated for neg never freed

## Affected Lines in the original program
In `filter.c:155`

## Steps to Reproduce
./filter test_imgs/ck.png output negative

## Suggested Fix Description
Add free(neg)

# BUG 4 -
## Category
Temporal safety violation

## Description
Pointer initialized, but memory never allocated

## Affected Lines in the original program
In `filter.c:127`

## Steps to Reproduce
./filter test_imgs/ck.png output negative

## Suggested Fix Description
Add malloc(sizeof(struct pixel));

# BUG 5 -
## Category
Local persistent pointer bug

## Description
Local variable initialized in get_pixel ofr neg, and adress returned. Since it's a local pointer, the memory space will not be accessible from outside the method. 

## Affected Lines in the original program
In `filter.c:128`

## Steps to Reproduce
./filter test_imgs/ck.png output negative

## Suggested Fix Description
Define neg pointer instead, and return it.

# BUG 6 -
## Category
Stack buffer overflow

## Description
strcpy doesnt check the number of chars copied, might lead to a buffer overflow if user puts more than ARG_SIZE chars.

## Affected Lines in the original program
In `filter.c:272`

## Suggested Fix Description
Use strncpy instead

# BUG 1 -
## Category
Heap buffer overflow

## Description
The size of the memory allocated is not correct, so the heap might overflow during the loop, causing a segmentation fault.

## Affected Lines in the original program
In `resize.c:48`

## Expected vs Observed
We expect that the loop only writes to allocated memory.

## Steps to Reproduce
./resize test_imgs/ck.png output 10

## Suggested Fix Description
Write "malloc(n_pixels * sizeof(struct pixel))" instead of "malloc(n_pixels + sizeof(struct pixel))"

# BUG 2 -
## Category
Memory leak

## Description
The memory allocated for new_img->px is not freed when returning.

## Affected Lines in the original program
In `resize.c:87`

## Expected vs Observed
We expect all memory allocated is freed before returning. 

## Steps to Reproduce
./resize test_imgs/ck.png output 10

## Suggested Fix Description
Add free(new_img->px) before returning.
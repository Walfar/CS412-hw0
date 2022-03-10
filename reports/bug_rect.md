# BUG 1 -
## Category
Type error

## Description
The parameter passed to strtol is a char* instead of a char**, which generates a segmentation fault.

## Affected Lines in the original program
In `rect.c:34`

## Expected vs Observed
The parameter given should respect the method header definition.

## Steps to Reproduce
./rect test_imgs/ck.png output 10

## Suggested Fix Description
The argument must be char**, use &end_ptr instead of end_ptr
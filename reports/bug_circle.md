# BUG 1 -
## Category
Iteration error

## Description
The for loops can draw beyond height and width.

## Affected Lines in the original program
In `circle.c:52`

## Suggested Fix Description
Add a check in each loop, if( x >= 0 && x < width) and if( x >= 0 && x < width)


# BUG 2 -
## Category
Stack buffer overflow

## Description
The programm tries to access arg[7], but there is only 7 values possible, resulting in a segfault.

## Affected Lines in the original program
In `circle.c:29-30`

## Steps to Reproduce
./circle test_imgs/ck.png output 1 1 1 ffff00

## Suggested Fix Description
Replace arg[7] by arg[6]


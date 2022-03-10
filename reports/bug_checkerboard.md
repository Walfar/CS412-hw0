# BUG 1 -
## Category
Iteration error

## Description
When iterating on the square, some values can get out of bound.

## Affected Lines in the original program
In `checkerboard:114`

## Steps to Reproduce
./checkerboard output 100 100 9 ffff00 ffff00

## Suggested Fix Description
if(square_top_left_y + y < height && square_top_left_x + x < width){

# BUG 2 -
## Category
Memory safety violation

## Description
The programm tries to free a memory that is not allocated. This is due to a double free, one line 91 then followed by another one line 142.

## Affected Lines in the original program
In `checkerboard.c:91`

## Steps to Reproduce
./checkerboard output 1 1 1 ffff00 ffff00

## Suggested Fix Description
Remove free(img) line 91
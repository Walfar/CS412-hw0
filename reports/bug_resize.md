# BUGS IN RESIZE.c

# BUG 1 -
LIne: 47

Malloc without check, can output segfault
Fix: add check, exit if failed to allocate memory

# BUG 2 -
Line: 92

img->px is not freed. Memory allocation error.
Fix: add free(img->px) before free(img)
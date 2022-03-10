# BUGS IN SOLID.c

# BUG 1 -
line 10: Memory allocation unchecked

We don't check that the malloc was succesfull, which could result in a segfault.
Fix: add a check, return if malloc failed


# BUG 2 -
line 33: Stack buffer overflow

We don't check the size of the array being copied. An adversary could overflow the buffer by giving more than OUTPUT_NAME_SIZE chars in args.
Fix: use strncpy instead

# BUG 3 -
line 125: Command injection

An adversary can inject code in the command that will be called by the system.
Fix: Return an error if the output name contains non alphanumeric characters (line 37)


# BUG 5 -
line 127: Memory leak

Must free palette before returning
Fix: add free(palette)

# BUG 6 -
line 144: Memory leak

Must free palette in case of error return.
Fix: add free(palette)

# BUG 7 -
line 138: pointer error

Pointers for palette and img must be set to NULL after freed
Fix: ptr = NULL

# BUG 8 -
line 158: pointer error

Pointers for palette and img must be set to NULL after freed
Fix: ptr = NULL

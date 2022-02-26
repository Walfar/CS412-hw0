# BUGS IN SOLID.c

# BUG 1 -
line: 10

We don't check that the malloc was succesfull, which could result in segfault.
Fix: add a check, exit if malloc failed


# BUG 2 -
line: 33

We don't check the size of the array being copied. An adversary could overflow the buffer by giving more than OUTPUT_NAME_SIZE chars in args.
Fix: use strncpy instead

# BUG 3 -
line: 124-125

An adversary can inject code in the command that will be called by the system.
Fix: sanatize the command

# BUG 4 -
line: 142

Must free palette in case of error return.
Fix: add free(palette)

# BUG 5 -
line: 127

Must free palette before returning
Fix: add free(palette)

# BUG 6 -
line: 43/51

Type is unsigned long ??
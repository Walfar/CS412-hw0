# BUG-1
## Category
Memory allocation unchecked

## Description
We don't check that the malloc was succesfull after calling "struct pixel *palette = allocate_palette()", which could result in a segfault if running on a computer with low memory.

## Affected Lines in the original program
In `solid.c:16`

## Expected vs Observed
We expect that the programm ends smoothly, even if there isn't enough memory. But, in this case, it results in a segfault.

## Steps to Reproduce
Run "./solid output 1 1 ffff00" on a computer with low memory

## Suggested Fix Description
After calling "struct pixel *palette = allocate_palette()" add a check that returns if the pointer is NULL, i.e no memory was allocated.
"if (!palette) {
    return 1;
}"

# BUG 2 -
## Category
Stack Buffer Overflow

## Description
We don't check the size of the array being copied. An adversary could overflow the buffer by giving more than OUTPUT_NAME_SIZE chars in args.

## Affected Lines in the original program
In `solid.c:35`

## Expected vs Observed
We expect that a user is restricted to the memory space he has access to when giving his inputs. The user, when putting more than 500 characters (OUTPUT_NAME_SIZE) can overflow the buffer and write code to unrestricted memory.

## Steps to Reproduce
./solid a(500 times)maliciouscode 1 1 ffff00

## Suggested Fix Description
Use strncpy(output_name, argv[1], OUTPUT_NAME_SIZE) instead of strcpy

# BUG 3 -
## Category
Command injection

## Description
An adversary can inject code in the command that will be called by the system.

## Affected Lines in the original program
In `solid.c:135`

## Expected vs Observed
We expect all user's inputs are correctly sanitized to avoid code injection.

## Steps to Reproduce
./solid "&&maliciouscommand 1 1 ffff00

## Suggested Fix Description
Sanitize by accepting only alphanumerics chars from the user's input. Add the following after receiving the inputs:
for (size_t i = 0; i < strlen(output_name); i++) {
    if (!isalnum(output_name[i])) {
      goto error;
    }
} 

# BUG 4 -
## Category
Memory Leak

## Description
Memory allocated for palette is not freed.

## Affected Lines in the original program
In `solid.c:127`, `solid.c:144`

## Expected vs Observed
We expect all memory allocated is freed before returning. 

## Steps to Reproduce
./solid output 1 1 ffff00

## Suggested Fix Description
Add free(palette) before returning at the end of the programm (line 137), and when a error_mem occurs (line 155).

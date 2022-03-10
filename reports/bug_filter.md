# BUG 1
filter_negative, line 118-119
Bug: <= makes out of bound loop (SEGFAULT)
fix: < instead 

# BUG 2
filter_negative, line 124
Bug: unchecked malloc for neg

# BUG 3
filter_negative, line 136
Bug: neg memory not freed

# BUG 4
line 108
Bug: pixel is only initialzed, but no memory allocated
Fix: add "malloc"

-- BUG LOOP ONLY ON VALID PIXELS (in-bound)

# BUG 5
transparency filter
Add check if transparency is bigger than 255 !
line 170:   if (local_alpha > 255) local_alpha = 255;
# BUGS IN RESIZE.c

# BUG 1
line 48
in malloc * instead of +

# BUG 2
line 69: 
exchange y,x by x,y (same goes with nearest_x and nearest_y)
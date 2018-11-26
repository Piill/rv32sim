li sp, 0
li t0, -1
li t1, 1
slt t0, x0, t0 # x0 < t0 -> no
slt t1, t0, t1 # t0 < t1 -> yes

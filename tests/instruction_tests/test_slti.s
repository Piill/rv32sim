li sp, 0
li t0, -1
li t1, 1
slti t0, t1, -1 # t1 < -0 -> no
slti t1, t0, 1 # t0 < 1 -> yes

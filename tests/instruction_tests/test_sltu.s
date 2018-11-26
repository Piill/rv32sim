li sp, 0
li t0, -1
li t1, 1
sltu t0, t0, t1 # t0 < x0 -> no
sltu t1, x0, t1 # x0 < t1 -> yes

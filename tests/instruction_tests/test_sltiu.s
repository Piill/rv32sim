li sp, 0
li t0, -1
li t1, 1
sltiu t0, t1, -1 # t1 < -1 -> yes
sltiu t1, t0, 1 # t0 < 1 -> no

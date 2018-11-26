li a0, 10
li t0, 0x1
li t1, 0x0
beq t0, x0, test1
li t2, 1
test1:
beq t1, x0, test2
ecall
test2:
li t2, 100
li sp, 0
ecall

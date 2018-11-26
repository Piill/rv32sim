li a0, 10
li t0, 0
li t1, -1 
bge t0, x0, test1 # yes
ecall
test1:
beq t1, x0, test2 # no
li t2, 100
test2:
li sp, 0
ecall

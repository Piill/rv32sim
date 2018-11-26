li sp, 0
li a0, 10
li t0, 0
li t1, -1  
blt t1, x0, test1 # yes
ecall
test1:
blt t0, x0, test2 # no
li t2, 100 
test2:
ecall


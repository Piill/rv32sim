li sp, 0
li a0, 10
li t0, 0
li t1, -1  
bgeu t1, x0, test1 # yes
ecall
test1:
bgeu t0, t1, test2 # no
li t2, 100 
test2:
ecall

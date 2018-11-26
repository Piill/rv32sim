li t0, 1
jal t1, test
li t0, 100
test:
addi t0, t0, 1
li sp, 0
ecall

addi s0, x0, 1
addi s1, x0, 1
addi s2, x0, 1
addi s3, x0, 1
addi s4, x0, 1
addi t0, x0, 5
addi t1, x0, 10
blt t0, t1, 8
addi s0, x0, 0
addi t0, x0, 5
addi t1, x0, 5
blt t0, t1, 8
addi s1, x0, 0
addi t0, x0, 10
addi t1, x0, 5
blt t0, t1, 8
addi s2, x0, 0
addi t0, x0, -5
addi t1, x0, 10
blt t0, t1, 8
addi s3, x0, 0
addi t0, x0, 5
addi t1, x0, -10
blt t0, t1, 8
addi s4, x0, 0
addi a0, x0, 10
ecall

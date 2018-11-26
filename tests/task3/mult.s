li a0, 10
li a1, 2
jal ra, mult
li a1, 5
jal ra, mult
li a0, 10
ecall


mult:
	addi a0, a0, -1
    add t0, t0, a1
    bne a0, x0, mult
    add a0, t0, x0
    add a1, t0, x0
    jalr x0, 0(ra)

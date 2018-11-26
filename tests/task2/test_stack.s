addi sp, sp, -24
li t0, 0xfea8f65c
sw t0, 0(sp)
sh t0, 8(sp)
sb t0, 16(sp)
lw t2, 0(sp)
lh t1, 8(sp)
lb t0, 16(sp)
li sp, 0
ecall

	.text
	li a0,0xff000000
	li a1,0x000000ff
    li a2,8
	sll a0, a0, a2
	sll a1, a1, a2
	ecall

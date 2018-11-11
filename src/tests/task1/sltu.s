	.text
	li a0,0xff000000
	li a1,0x000000ff
   li a2,8
	sltu a3, a0, a2
	sltu a1, a2, a0

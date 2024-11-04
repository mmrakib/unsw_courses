# COMP1521 21T2 ... final exam, question 2

	.data
even_parity_str:	.asciiz "the parity is even\n"
odd_parity_str:		.asciiz "the parity is odd\n"

# int count = 0;
# for (int i = 0; i < 32; i++) {
# 	if (x & 0x80000000) {
#		count++;
#	}
#   x = x << 1;
# }
# if (count % 2 == 0) {
# 	print_string(even_parity_str)
# } else {
#   print_string(odd_parity_str)
# }

	.text
main:
	li	$v0, 5
	syscall
	move	$t0, $v0
	# input is in $t0
	
	# i and count definitions, respectively
	li $s0, 0
	li $s1, 0

	# Constants
	li $t1, 32
	li $t2, 0x80000000

	loop1_step:
	bge $s0, $t1, loop1_end

	and $t3, $t0, $t2
	beqz $t3, if1_false

	addi $s1, $s1, 1
	j if1_end

	if1_false:

	if1_end:
	
	sll $t0, $t0, 1

	addi $s0, $s0, 1
	j loop1_step

	loop1_end:

	li $t1, 2 # Replace 32 with 2
	div $t4, $s1, $t1
	mfhi $t5
	bnez $t5, if2_false

	# Syscall
	li	$v0, 4
	la	$a0, even_parity_str
	syscall
	j if2_end

	if2_false:

	# Syscall
	li	$v0, 4
	la	$a0, odd_parity_str
	syscall

	if2_end:

	# Return
	li	$v0, 0
	jr	$ra

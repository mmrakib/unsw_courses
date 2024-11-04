# Sieve of Eratosthenes
# https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
main:

    # PUT YOUR CODE
    li $s0, 0

    loop1_step:
    bge $s0, 1000, loop1_end
    li $t0, 1
    sb $t0, prime($s0)
    addi $s0, $s0, 1
    j loop1_step
    loop1_end:

    li $s0, 2

    loop2_step:
    bge $s0, 1000, loop2_end
    lb $t0, prime($s0)
    beqz $t0, if1_end

    move $a0, $s0
    li $v0, 1
    syscall
    li $a0, '\n'
    li $v0, 11
    syscall
    move $s1, $s0
    mul $s1, $s1, 2

    loop3_step:
    bge $s1, 1000, loop3_end
    sb $0, prime($s1)
    add $s1, $s1, $s0
    j loop3_step
    loop3_end:

    if1_end:
    addi $s0, $s0, 1
    j loop2_step
    loop2_end:

    li $v0, 0           # return 0
    jr $31

.data
prime:
    .space 1000
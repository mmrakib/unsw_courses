# read two integers and print all the integers which have their bottom 2 bits set.

main:
    li $v0, 5
    syscall
    move $t0, $v0

    li $v0, 5
    syscall
    move $t1, $v0
    # $t0 - x
    # $t1 - y
    # $t2 - i & 3
    # $t3 - (i & 3) == 3

    addi $s0, $t0, 1
    loop_step:
    bge $s0, $t1, loop_end

    # Loop body
    andi $t2, $s0, 3
    seq $t3, $t2, 3
    beqz $t3, if_false

    move $a0, $s0
    li $v0, 1
    syscall
    li $a0, '\n'
    li $v0, 11
    syscall

    if_false:

    if_end:
    addi $s0, $s0, 1
    j loop_step

    loop_end:


    # # THESE LINES JUST PRINT 42
    # # REPLACE THE LINES BELOW WITH YOUR CODE
    # li $a0, 42
    # li $v0, 1
    # syscall
    # li   $a0, '\n'
    # li   $v0, 11
    # syscall
    # # REPLACE THE LINES ABOVE WITH YOUR CODE


end:
    li $v0, 0
    jr $31

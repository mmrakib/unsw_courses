# read a line and print whether it is a palindrom

main:
    la   $a0, str0       # printf("Enter a line of input: ");
    li   $v0, 4
    syscall

    la   $a0, line
    la   $a1, 256
    li   $v0, 8          # fgets(buffer, 256, stdin)
    syscall              #

    li   $t0, 0
    loop1_step:
    la   $t1, line
    add  $t1, $t1, $t0
    lb   $t2, ($t1)
    beqz $t2, loop1_end
    addi $t0, $t0, 1
    j loop1_step
    loop1_end:

    li   $t1, 0
    move $t2, $t1
    addi $t2, $t0, -2

    loop2_step:
    bge  $t1, $t2, loop2_end

    la   $t3, line
    add  $t3, $t3, $t1
    lb   $t4, ($t3)

    la   $t3, line
    add  $t3, $t3, $t2
    lb   $t5, ($t3)

    beq  $t4, $t5, if1_end

    la   $a0, not_palindrome
    li   $v0, 4
    syscall

    li   $v0, 0          # return 0
    jr   $ra

    if1_end:

    addi $t1, $t1, 1
    addi $t2, $t2, -1
    j loop2_step
    loop2_end:

    la   $a0, palindrome
    li   $v0, 4
    syscall

    li   $v0, 0          # return 0
    jr   $ra


.data
str0:
    .asciiz "Enter a line of input: "
palindrome:
    .asciiz "palindrome\n"
not_palindrome:
    .asciiz "not palindrome\n"


# line of input stored here
line:
    .space 256


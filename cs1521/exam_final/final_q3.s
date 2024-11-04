# read an integer
# print 1 iff  the least significant bit is equal to the most significant bit
# print 0 otherwise

# int main() {
#     int x;
#     scanf("%d", &x);
    
#     int lsb = x & 0x00000001;
#     int msb = (x & 0x80000000) >> 31;

#     if (lsb == msb) {
#         print("1\n");
#     } else {
#         print("0\n");
#     }

#     return 0;
# }

main:
    li   $v0, 5
    syscall
    move $t0, $v0
    # $t0 - x
    # $t1 - lsb
    # $t2 - msb
    # $t3 - temp for 0x80000000

    # lw $t3, 0x80000000
    andi $t1, $t0, 1
    #and $t2, $t0, $t3
    move $t2, $t0
    srl $t2, $t2, 31
    andi $t2, $t2, 1

    bne $t1, $t2, if_false

    li $a0, 1
    li $v0, 1
    syscall
    li $a0, '\n'
    li $v0, 11
    syscall

    j if_end
    if_false:

    li $a0, 0
    li $v0, 1
    syscall
    li $a0, '\n'
    li $v0, 11
    syscall

    if_end:

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

# this code reads a line of input and prints 42
# change it to check the string for brackets

# read a line of input and checks whether it consists only of balanced brackets
# if the line contains characters other than ()[]{} -1 is printed
# if brackets are not balance,  -1 is printed
# if the line contains only balanced brackets, length of the line is printed

match:
    addi $sp, $sp, -4     #    
    sw   $ra, 0($sp)      # Pushes return address to stack

    move $t0, $a0         # $t0 = index

    mul  $t1, $t0, 1      #   calculate &line[index]
    la   $t2, line        #
    add  $t3, $t1, $t2    #
    lb   $t4, ($t3)       # $t4 = c = line[index]

    li   $t5, -1          # $t5 = r = -1
    li   $t6, 0           # $t6 = w = 0

    bne  $t4, '[', if3_else1

    li   $t6, ']'

    b if3_end
    if3_else1:
    bne  $t4, '(', if3_else2

    li   $t6, ')'

    b if3_end
    if3_else2:
    bne  $t4, '{', if3_end

    li   $t6, '}'

    b if3_end
    if3_end:


    beqz $t6, if4_false

    move $a0, $t0        # Places index + 1 and w as args
    addi $a0, $a0, 1     #
    move $a1, $t6        #

    addi $sp, $sp, -12   # Saves index + 1, r and w
    sw   $t0, 8($sp)     #
    sw   $t5, 4($sp)     #
    sw   $t6, 0($sp)     #

    jal  check           # check(index + 1, w)

    lw   $t6, 0($sp)     #
    lw   $t5, 4($sp)     #
    lw   $t0, 8($sp)     #
    addi $sp, $sp, 12    # Loads both index + 1, r and w

    move $t5, $v0        # r = check(index + 1, w)

    b if4_false
    if4_false:

    move $v0, $t5        # Set return value as r

    lw   $ra, 0($sp)     # Pops return address from stack
    addi $sp, $sp, 4     #
    jr   $ra             # Returns with return value: r
end_match:


check:
    addi $sp, $sp, -4     # Makes room for return address
    sw   $ra, 0($sp)      # Pushes return address to stack

    move $t0, $a0         # Places args in temp variables if args get overwritten
    move $t5, $a1
    # $t0 - index
    # $t1 - offset = index * sizeof(char)
    # $t2 - &line
    # $t3 - &line + offset
    # $t4 - value of: &line + offset, or line[index]
    # $t5 - what

    mul  $t1, $t0, 1     #   calculate &line[index]
    la   $t2, line       #
    add  $t3, $t1, $t2   #
    lb   $t4, ($t3)

    bne  $t4, $t5, if1_else # if (line[index] == what) {
    # $t1 to $t4 are never saved because we only use them in the if clause above,
    # so it doesn't matter if they get overwritten every recursive check() call

    addi $t0, $t0, 1     # index = index + 1

    b if1_end
    if1_else:            # } else {

    move $a0, $t0        # Places index as arg

    addi $sp, $sp, -8    # Saves both index and match just in case $t0 and $t5 get overwritten    
    sw   $t0, 4($sp)     #
    sw   $t5, 0($sp)     #

    jal  match           # match(index)

    lw   $t5, 0($sp)     #
    lw   $t0, 4($sp)     #
    addi $sp, $sp, 8     # Load both index and match 

    move $t0, $v0        # index = match(index)

    blez $t0, if2_false  # if (index > 0) {

    move $a0, $t0        # Places index and match as args
    move $a1, $t5        #

    addi $sp, $sp, -8    # Saves both index and match just in case $t0 and $t5 get overwritten    
    sw   $t0, 4($sp)     #
    sw   $t5, 0($sp)     #

    jal  check           # check(index, what)

    lw   $t5, 0($sp)     #
    lw   $t0, 4($sp)     #
    addi $sp, $sp, 8     # Loads both index and match

    move $t0, $v0        # index = check(index, what)

    b if2_end
    if2_false:

    if2_end: # }

    if1_end: # } 

    move $v0, $t0        # Sets return value as index

    lw   $ra, 0($sp)     #
    addi $sp, $sp, 4     # Pops return address from stack 
    jr   $ra             # Returns with return value: index
end_check:


main:
    addi $sp, $sp, -8
    sw   $ra, 4($sp) 

    la   $a0, line
    la   $a1, 1024
    li   $v0, 8          # fgets(line, 1024, stdin);
    syscall
    move $t0, $a0

    sw   $t0, 0($sp)

    li   $a0, 0
    li   $a1, '\n'
    jal  check           # check(0, '\n')

    lw   $t0, 0($sp)

    move $t1, $v0        # r = check(0, '\n')   

    # THESE LINES JUST PRINT 42
    # REPLACE THE LINES BELOW WITH YOUR CODE
    move $a0, $t1
    li $v0, 1
    syscall
    li   $a0, '\n'
    li   $v0, 11
    syscall
    # REPLACE THE LINES ABOVE WITH YOUR CODE

    lw   $ra, 4($sp)
    addi $sp, $sp, 8

    li   $v0, 0          # return 0
    jr   $31


# PUT YOU FUNCTION DEFINITIONS HERE


.data
line:
    .byte 0:1024

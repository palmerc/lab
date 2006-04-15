# MIPS Adventure Game Program
# Cameron Palmer, Matthew Quick, Michael Gallaty
# April 2006

    .data
        moves: .word 1,2,3,1,1
        moves_size: .word 5
    .text

main:
    add $s0, $zero, $zero   # Set the initial state to zero
    lw $s1, moves_size
    
    #while there are moves available loop
while:
    la $t0, moves
    lw $a1, $s1(moves)          # Move the current move into $a1
    
ifZero
    add $a0, $zero, $s0         # move the current state into $a0
    bne $s0, $zero, ifOne   # if state != 0
    jal StateZero
ifOne:
    addi $t0, $zero, 1
    bne $s0, $t0, ifTwo    # if state != 1
    jal StateOne
ifTwo:
    addi $t0, $zero, 2
    bne $s0, $t0, ifThree    # if state != 2
    jal StateTwo
ifThree:
    addi $t0, $zero, 3
    bne $s0, $t0, ifFour  # if state != 3
    jal StateThree
ifFour:
    addi $t0, $zero, 4
    bne $s0, $t0, ifFive   # if state != 4
    jal StateFour
ifFive:
    addi $t0, $zero, 5
    bne $s0, $t0, ifSix   # if state != 5
    jal StateFive
ifSix:
    addi $t0, $zero, 6
    bne $s0, $t0, ifSeven    # if state != 6
    jal StateSix
ifSeven:
    addi $t0, $zero, 7
    bne $s0, $t0, ifEight  # if state != 7
    jal StateSeven
ifEight:
    addi $t0, $zero, 8
    bne $s0, $t0, ifNine  # if state != 8
    jal StateEight
ifNine:
    addi $t0, $zero, 9
    bne $s0, $t0, ifTen   # if state != 9
    jal StateNine
ifTen:    
    addi $t0, $zero, 10
    bne $s0, $t0, ifZero    # if state != 10
    jal StateTen
    
    add $s0, $zero, $v0     # The state change should be moved into the current state
    addi $s1, $s1, -1
    jne $s1, $zero, while

StateZero:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    
    # if StateZero and Direction East Goto StateOne
    
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

StateOne:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    # if StateOne and Direction North Goto StateTwo
    # if StateOne and Direction East Goto StateThree
    # if StateOne and Direction South Goto StateSix
    # if StateOne and Direction West Goto StateZero
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
    
StateTwo:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    # Assert Potion
    # if StateTwo and Direction South Goto StateOne
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
    
StateThree:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    # if StateThree and Direction North Goto StateFour
    # if StateThree and Direction East Goto StateFive
    # if StateThree and Direction West Goto StateOne
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
    
StateFour:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    # if StateFour and Direction South Goto StateThree
    # if StateFour and Direction East Goto StateFive
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
    
StateFive:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    # if StateFive and Direction West Goto StateFour
    # if StateFive and Direction South Goto StateThree
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
    
StateSix:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    # if StateSix and Direction West Goto StateSeven
    # if StateSix and Direction East Goto StateEight
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
    
StateSeven:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    # Asset Sword
    # if StateSeven and Direction East Goto StateSix
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
    
StateEight:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    # if StateEight and Sword Goto StateTen
    # if StateEight and No Sword Goto StateNine
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
    
StateNine:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    # if StateNine and Potion Goto StateZero and Deassert potion
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
    
StateTen:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    # if StateTen Assert WIN
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
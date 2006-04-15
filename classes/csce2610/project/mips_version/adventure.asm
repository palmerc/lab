# MIPS Adventure Game Program
# Cameron Palmer, Matthew Quick, Michael Gallaty
# April 2006

    .data
        moves: .word 1,2,3,1,1
        moves_size: .word 5
        potion: .word 0
        sword: .word 0
        win: .word 0
        dead: .word 0
    .text

###
### function StateZero
### accepts: $a0, current state, $a1, direction to move
### returns: $v0, new state
###
StateZero:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    addi $v0, $zero, 0
    add $t0, $zero, $a1
    # if StateZero and Direction East Goto StateOne
    addi $t1, $zero, 1
    bne $t0, $t1, ExitZero
    addi $v0, $zero, 3
    j ExitZero
    
ExitZero:
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

###
### function StateOne
### accepts: $a0, current state, $a1, direction to move
### returns: $v0, new state
###
StateOne:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    addi $v0, $zero, 1
    add $t0, $zero, $a1
    # if StateOne and Direction North Goto StateTwo
NorthOne:    
    bne $t0, $zero, EastOne
    addi $v0, $zero, 2
    j ExitOne
    # if StateOne and Direction East Goto StateThree
EastOne:
    addi $t1, $zero, 1
    bne $t0, $t1, SouthOne
    addi $v0, $zero, 3
    j ExitOne
    # if StateOne and Direction South Goto StateSix
SouthOne:
    addi $t1, $zero, 2
    bne $t0, $t1, WestOne
    addi $v0, $zero, 6
    j ExitOne
    # if StateOne and Direction West Goto StateZero
WestOne:
    addi $t1, $zero, 3
    bne $t0, $t1, ExitOne
    addi $v0, $zero, 0
    j ExitOne
ExitOne:
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
    
###
### function StateTwo
### accepts: $a0, current state, $a1, direction to move
### returns: $v0, new state
###
StateTwo:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    addi $v0, $zero, 2
    # Assert Potion
    addi $t0, $zero, 1
    la $t1, potion
    sw $t0, 0($t1)
        
    add $t0, $zero, $a1
    # if StateTwo and Direction South Goto StateOne
SouthTwo:
    addi $t1, $zero, 2
    bne $t0, $t1, ExitTwo
    addi $v0, $zero, 6
    j ExitTwo

ExitTwo:
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
   
###
### function StateThree
### accepts: $a0, current state, $a1, direction to move
### returns: $v0, new state
###   
StateThree:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    addi $v0, $zero, 3
    add $t0, $zero, $a1
    # if StateThree and Direction North Goto StateFour
NorthThree:
    bne $t0, $zero, EastThree
    addi $v0, $zero, 4
    j ExitThree
    # if StateThree and Direction East Goto StateFive
EastThree:
    addi $t1, $zero, 1
    bne $t0, $t1, WestThree
    addi $v0, $zero, 5
    j ExitThree
    # if StateThree and Direction West Goto StateOne
WestThree:
    addi $t1, $zero, 3
    bne $t0, $t1, ExitThree
    addi $v0, $zero, 1
    j ExitThree

ExitThree:
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
    
###
### function StateFour
### accepts: $a0, current state, $a1, direction to move
### returns: $v0, new state
###
StateFour:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    addi $v0, $zero, 4
    add $t0, $zero, $a1
    # if StateFour and Direction East Goto StateFive
EastFour:
    addi $t1, $zero, 1
    bne $t0, $t1, SouthFour
    addi $v0, $zero, 5
    j ExitFour
    # if StateFour and Direction South Goto StateThree
SouthFour:
    addi $t1, $zero, 2
    bne $t0, $t1, ExitFour
    addi $v0, $zero, 3
    j ExitFour
    
ExitFour:
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
    
###
### function StateFive
### accepts: $a0, current state, $a1, direction to move
### returns: $v0, new state
###
StateFive:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    addi $v0, $zero, 5
    add $t0, $zero, $a1
    # if StateFive and Direction South Goto StateThree
SouthFive:
    addi $t1, $zero, 2
    bne $t0, $t1, WestFive
    addi $v0, $zero, 3
    j ExitFive
    # if StateFive and Direction West Goto StateFour
WestFive:
    addi $t1, $zero, 3
    bne $t0, $t1, ExitFive
    addi $v0, $zero, 4
    j ExitFive
    
ExitFive:
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
    
###
### function StateSix
### accepts: $a0, current state, $a1, direction to move
### returns: $v0, new state
###
StateSix:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    addi $v0, $zero, 6
    add $t0, $zero, $a1
    # if StateSix and Direction East Goto StateEight
EastSix:
    addi $t1, $zero, 1
    bne $t0, $t1, WestSix
    addi $v0, $zero, 8
    j ExitSix
    # if StateSix and Direction West Goto StateSeven
WestSix:
    addi $t1, $zero, 3
    bne $t0, $t1, ExitSix
    addi $v0, $zero, 7
    j ExitSix
ExitSix:
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

###
### function StateSeven
### accepts: $a0, current state, $a1, direction to move
### returns: $v0, new state
###
StateSeven:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    addi $v0, $zero, 7
    # Assert Sword
    addi $t0, $zero, 1
    la $t1, sword
    sw $t0, 0($t1)
    
    add $t0, $zero, $a1
    # if StateSeven and Direction East Goto StateSix
EastSeven:
    addi $t1, $zero, 1
    bne $t0, $t1, ExitSeven
    addi $v0, $zero, 6
    j ExitSeven
ExitSeven:
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

###
### function StateEight
### accepts: $a0, current state, $a1, direction to move
### returns: $v0, new state
###
StateEight:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    
    lw $t0, sword
    # if StateEight and Sword Goto StateTen
HasSword:
    addi $t1, $zero, 1
    bne $t0, $t1, NoSword
    addi $v0, $zero, 10
    j ExitEight
    # if StateEight and No Sword Goto StateNine
NoSword:
    addi, $v0, $zero, 9
    
ExitEight:
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
    
###
### function StateNine
### accepts: $a0, current state, $a1, direction to move
### returns: $v0, new state
###
StateNine:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    lw $t0, potion
    
    la $t0, dead
    addi $t1, $zero, 1
    sw $t1, 0($t0)
    
    # if StateNine and Potion Goto StateZero and Deassert potion
HasPotion:
    addi $t1, $zero, 1
    bne $t0, $t1, NoPotion
    add $v0, $zero, $zero
    la $t0, potion
    sw $zero, 0($t0)
NoPotion:
    addi $v0, $zero, 9
    
ExitNine:
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

###
### function StateTen
### accepts: $a0, current state, $a1, direction to move
### returns: $v0, new state
###
StateTen:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    add $t0, $zero, $a1
    # if StateTen Assert WIN
    la $t0, win
    addi $t1, $zero, 1
    sw $t1, 0($t0)
    
    addi $v0, $zero, 10
ExitTen:
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
    
###
### Main Function
###
###
main:
    add $s0, $zero, $zero   # Set the initial state to zero
    lw $s1, moves_size
    
    #while there are moves available loop
while:
    la $t0, moves
    add $t1, $zero, $s1
    sll $t1, $t1, 2
    add $t0, $t0, $t1
    lw $a1, 0($t0)          # Move the current move into $a1
    
ifZero:
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
    beq $s1, $zero, exit
    j while
exit:
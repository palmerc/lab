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
    lw $a1, $s1(moves)
    
    add $a0, $zero, $s0         # move the current state into $a0
    beq $s0, $zero, StateZero   # if state == 0
    addi $t0, $zero, 1
    beq $s0, $t0, StateOne    # if state == 1
    addi $t0, $zero, 2
    beq $s0, $t0, StateTwo    # if state == 2
    addi $t0, $zero, 3
    beq $s0, $t0, StateThree  # if state == 3
    addi $t0, $zero, 4
    beq $s0, $t0, StateFour   # if state == 4
    addi $t0, $zero, 5
    beq $s0, $t0, StateFive   # if state == 5
    addi $t0, $zero, 6
    beq $s0, $t0, StateSix    # if state == 6
    addi $t0, $zero, 7
    beq $s0, $t0, StateSeven  # if state == 7
    addi $t0, $zero, 8
    beq $s0, $t0, StateEight  # if state == 8
    addi $t0, $zero, 9
    beq $s0, $t0, StateNine   # if state == 9
    addi $t0, $zero, 10
    beq $s0, $t0, StateTen    # if state == 10

    addi $s1, $s1, -1
    jne $s1, $zero, while

StateZero:
    # if StateZero and Direction East Goto StateOne

StateOne:
    # if StateOne and Direction North Goto StateTwo
    # if StateOne and Direction East Goto StateThree
    # if StateOne and Direction South Goto StateSix
    # if StateOne and Direction West Goto StateZero

StateTwo:
    # Assert Potion
    # if StateTwo and Direction South Goto StateOne
    
StateThree:
    # if StateThree and Direction North Goto StateFour
    # if StateThree and Direction East Goto StateFive
    # if StateThree and Direction West Goto StateOne

StateFour:
    # if StateFour and Direction South Goto StateThree
    # if StateFour and Direction East Goto StateFive
    
StateFive:
    # if StateFive and Direction West Goto StateFour
    # if StateFive and Direction South Goto StateThree

StateSix:
    # if StateSix and Direction West Goto StateSeven
    # if StateSix and Direction East Goto StateEight
StateSeven:
    # Asset Sword
    # if StateSeven and Direction East Goto StateSix

StateEight:
    # if StateEight and Sword Goto StateTen
    # if StateEight and No Sword Goto StateNine

StateNine:
    # if StateNine and Potion Goto StateZero and Deassert potion

StateTen:
    # if StateTen Assert WIN
# MIPS Adventure Game Program
# Cameron Palmer, Matthew Quick, Michael Gallaty
# April 2006

    .data
        potion: .word 0
        sword: .word 0
        win: .word 0
        dead: .word 0
        exit_value: .word 0
        current_state: .asciiz "\nCurrent state: "
        next_move: .asciiz "\nEnter a direction: "
        line_feed: .asciiz "\n"
        welcome: .asciiz "\n\nWelcome to the text-based adventure game.\n\n"
        state0_descrip: .asciiz "You are in the Cave of Cacophony.  The sounds, the smells are almost exactly\nlike the sight: grim.  There is only one way to go from here... east.\n"
        state1_descrip: .asciiz "You proceed through a Twisty Tunnel, and alas, you find a light at the end of\nthe tunnel.  No, you aren't dead, but you do find yourself being able to go\nto one of four places.\n\nTo the north you see a Parlor.  To the south, you hear water.\n\nTo the west you go back through the tunnel.  To the east, you see what looks to\nbe a maze.\n"
        state2_descrip: .asciiz "You enter the Potion Parlor.  The door opens easily to a small hut that looks as\nif it had been abandoned for years.  Lining the walls are many bottles labeled\nPOTION.  I don't think the owner, or former owner, would notice one missing if\nyou took one...\n\nThere is no other way to out so you should leave the way you came, South.\n"
        state3_descrip: .asciiz "You seem to be caught in a Lengthy Labyrinth.  North South East or West??\n"
        state4_descrip: .asciiz "You seem to be caught in a Mini-Maze.  North South East or West??\n"
        state5_descrip: .asciiz "You seem to be caught in a Hall of Mirrors.  North South East or West??\n"
        state6_descrip: .asciiz "You happen upon a Rapid River.  You see two caves, one of which is easily\naccessed to the east.  The other on the west side...well... you have to cross\nthe river...and its about a 1/2 mile wide...\n"
        state7_descrip: .asciiz "You enter the cave.  As you proceed, the level of light grows dimmer.  And\ndimmer...and dimmer.  You begin to grope around to find your way.  You stop when\nyou feel a cut on your hand and smell blood.  You finally see a reflection in\nwhat little light there is, its a SWORD.\n"
        state8_descrip: .asciiz "You enter the cave.  As you proceed, the level of light grows dimmer.  And\nsuddenly, a flash of fire appears revealing a dragon fully ready to bite you in\ntwo. You realize that you have stumbled into the Dragon's Den.\n\n"
        state8_withsword: .asciiz "In a flash of panicked brilliance you reach for your sword.  You swing left, you\nswing right, but to no avail.  The dragon lunges at you and you point your sword\nin fear.  As luck would have it, you hit your mark.  The dragon limps over, and\na light at the end of the cave can be seen to the East.\n"
        state8_nosword: .asciiz "The dragon moves around to the entrance blocking it.  You would notice that\nthere is another light, but unfortunately, you're running around like a\nover-caffeinated ferret.  Moments later, you find yourself looking at your own,\ncrispy, char-grilled, well-done, quite dead, body.  Welcome to the after-life.\n"
        state9_withpot: .asciiz "\nThe potion next to your body cracks, and liquid begins to seep onto your body.\nYou regain consciousness, enough to engulf the rest of the potion.  You see a\npath to a cave, so you begin to follow it.  You end up where you started...\nSo that's what that smell was...\n"
        state9_descrip: .asciiz "You're body gets thrown into a pile of other, decaying, and foul-smelling\nbodies.  But you really don't notice the smell, because the dead don't smell.\nCongratulations your incompetence has landed you in the Grievous Graveyard.\n"
        state10_descrip: .asciiz "As you proceed along the wall, the light turns from a shade of white, to a shade\nof yellow.  You find yourself in a room filled with gold, women, treasure,\ngoblets, and wine.  Ok, no women...or wine...but you do have gold, and gold gets\nwomen...and wine.  But women too...\n"
    .text

###
### function GetMove
### accepts: nothing
### returns: $v0, the move
###
GetMove:
    addi $sp, $sp, -8
    sw $a0, 4($sp)
    sw $ra, 0($sp)
    
    # Make your move, creep. - robocop
    li $v0, 4
    la $a0, next_move
    syscall
    li $v0, 5               # Read in the move using System Call
    syscall

    lw $a0, 4($sp)
    lw $ra, 0($sp)
    addi $sp, $sp, 8
    jr $ra

###
### function StateZero
### accepts: $a0, current state, $a1, direction to move
### returns: $v0, new state
###
StateZero:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    
    # Display state message
    add $t0, $zero, $v0
    add $t1, $zero, $a0
    li $v0, 4
    la $a0, state0_descrip
    syscall
    add $v0, $zero, $t0
    add $a0, $zero, $t1
    
    jal GetMove
    move $t0, $v0
    
    addi $v0, $zero, 0      # State defaults to zero
    # if StateZero and Direction East Goto StateOne
    addi $t1, $zero, 1
    bne $t0, $t1, ExitZero
    addi $v0, $zero, 1      # State changes to one

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

    # Display state message
    add $t0, $zero, $v0
    add $t1, $zero, $a0
    li $v0, 4
    la $a0, state1_descrip
    syscall
    add $v0, $zero, $t0
    add $a0, $zero, $t1

    jal GetMove
    move $t0, $v0

    addi $v0, $zero, 1      # State defaults to one
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

    # Display state message
    add $t0, $zero, $v0
    add $t1, $zero, $a0
    li $v0, 4
    la $a0, state2_descrip
    syscall
    add $v0, $zero, $t0
    add $a0, $zero, $t1
    
    # Assert Potion
    addi $t0, $zero, 1
    la $t1, potion
    sw $t0, 0($t1)

    jal GetMove
    move $t0, $v0

    addi $v0, $zero, 2          # Establish state two as the current state    
    # if StateTwo and Direction South Goto StateOne
SouthTwo:
    addi $t1, $zero, 2
    bne $t0, $t1, ExitTwo
    addi $v0, $zero, 1
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

    # Display state message
    add $t0, $zero, $v0
    add $t1, $zero, $a0
    li $v0, 4
    la $a0, state3_descrip
    syscall
    add $v0, $zero, $t0
    add $a0, $zero, $t1

    jal GetMove
    move $t0, $v0

    addi $v0, $zero, 3
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
    
    # Display state message
    add $t0, $zero, $v0
    add $t1, $zero, $a0
    li $v0, 4
    la $a0, state4_descrip
    syscall
    add $v0, $zero, $t0
    add $a0, $zero, $t1
    
    jal GetMove
    move $t0, $v0

    addi $v0, $zero, 4
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
    
    # Display state message
    add $t0, $zero, $v0
    add $t1, $zero, $a0
    li $v0, 4
    la $a0, state5_descrip
    syscall
    add $v0, $zero, $t0
    add $a0, $zero, $t1

    jal GetMove
    move $t0, $v0

    addi $v0, $zero, 5
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

    # Display state message
    add $t0, $zero, $v0
    add $t1, $zero, $a0
    li $v0, 4
    la $a0, state6_descrip
    syscall
    add $v0, $zero, $t0
    add $a0, $zero, $t1

    jal GetMove
    move $t0, $v0

    addi $v0, $zero, 6
    # if StateSix and Direction North Goto StateOne
NorthSix:
    addi $t1, $zero, 0
    bne $t0, $t1, EastSix
    addi $v0, $zero, 1
    j ExitSix
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
    
    # Display state message
    add $t0, $zero, $v0
    add $t1, $zero, $a0
    li $v0, 4
    la $a0, state7_descrip
    syscall
    add $v0, $zero, $t0
    add $a0, $zero, $t1

    # Assert Sword
    addi $t0, $zero, 1
    la $t1, sword
    sw $t0, 0($t1)
    
    jal GetMove
    move $t0, $v0
    
    addi $v0, $zero, 7
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

    # Display state message
    add $t0, $zero, $v0
    add $t1, $zero, $a0
    li $v0, 4
    la $a0, state8_descrip
    syscall
    add $v0, $zero, $t0
    add $a0, $zero, $t1

    lw $t0, sword
    # if StateEight and Sword Goto StateTen
HasSword:
    addi $t1, $zero, 1
    bne $t0, $t1, NoSword
    
    # Display sword message
    add $t0, $zero, $v0
    add $t1, $zero, $a0
    li $v0, 4
    la $a0, state8_withsword
    syscall
    add $v0, $zero, $t0
    add $a0, $zero, $t1
    
    addi $v0, $zero, 10     # Set next state to 10
    j ExitEight
    # if StateEight and No Sword Goto StateNine
NoSword:
    # Display the lack of sword message
    add $t0, $zero, $v0
    add $t1, $zero, $a0
    li $v0, 4
    la $a0, state8_nosword
    syscall
    add $v0, $zero, $t0
    add $a0, $zero, $t1
    addi, $v0, $zero, 9     # Set next state to 9
    
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

    # Display state message
    add $t0, $zero, $v0
    add $t1, $zero, $a0
    li $v0, 4
    la $a0, state9_descrip
    syscall
    add $v0, $zero, $t0
    add $a0, $zero, $t1
      
    la $t0, dead
    addi $t1, $zero, 1
    sw $t1, 0($t0)
    
    lw $t0, potion
    
    # if StateNine and Potion Goto StateZero and Deassert potion
HasPotion:
    addi $t1, $zero, 1
    bne $t0, $t1, NoPotion
    # Display potion message
    add $t0, $zero, $v0
    add $t1, $zero, $a0
    li $v0, 4
    la $a0, state9_withpot
    syscall
    add $v0, $zero, $t0
    add $a0, $zero, $t1

    add $v0, $zero, $zero
    la $t0, potion
    sw $zero, 0($t0)
    
    la $t0, dead
    add $t1, $zero, $zero
    sw $t1, 0($t0)
    j ExitNine
    
NoPotion:
    la $t0, exit_value
    addi $t1, $zero, 1
    sw $t1, 0($t0)
    
    addi $v0, $zero, 9      # Set state to 9
    
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

    # Display state message
    add $t0, $zero, $v0
    add $t1, $zero, $a0
    li $v0, 4
    la $a0, state10_descrip
    syscall
    add $v0, $zero, $t0
    add $a0, $zero, $t1

    addi $v0, $zero, 10
    
    add $t0, $zero, $a1
    # if StateTen Assert WIN
    la $t0, win
    addi $t1, $zero, 1
    sw $t1, 0($t0)
       
    la $t0, exit_value
    addi $t1, $zero, 1
    sw $t1, 0($t0)
   
ExitTen:
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
    
###
### Main Function
###
###
main:
    # Display welcome message
    li $v0, 4
    la $a0, welcome
    syscall

    add $s0, $zero, $zero   # Set the initial state to zero
    # Display current state
    add $t0, $zero, $v0     # Since the syscall functions use $a0 and $v0 save off
    add $t1, $zero, $a0     # the values into $t0 and $t1 and restore them later
    li $v0, 4
    la $a0, current_state
    syscall
    li $v0, 1
    move $a0, $s0
    syscall
    li $v0, 4
    la $a0, line_feed
    syscall
    add $v0, $zero, $t0     # Restore the orignal $v0
    add $a0, $zero, $t1
  
while:        # while 1
    
    add $a0, $zero, $s0         # move the current state into $a0
    
ifZero:
    bne $s0, $zero, ifOne   # if state != 0
    jal StateZero
    j Next
ifOne:
    addi $t0, $zero, 1
    bne $s0, $t0, ifTwo    # if state != 1
    jal StateOne
    j Next
ifTwo:
    addi $t0, $zero, 2
    bne $s0, $t0, ifThree    # if state != 2
    jal StateTwo
    j Next
ifThree:
    addi $t0, $zero, 3
    bne $s0, $t0, ifFour  # if state != 3
    jal StateThree
    j Next
ifFour:
    addi $t0, $zero, 4
    bne $s0, $t0, ifFive   # if state != 4
    jal StateFour
    j Next
ifFive:
    addi $t0, $zero, 5
    bne $s0, $t0, ifSix   # if state != 5
    jal StateFive
    j Next
ifSix:
    addi $t0, $zero, 6
    bne $s0, $t0, ifSeven    # if state != 6
    jal StateSix
    j Next
ifSeven:
    addi $t0, $zero, 7
    bne $s0, $t0, ifEight  # if state != 7
    jal StateSeven
    j Next
ifEight:
    addi $t0, $zero, 8
    bne $s0, $t0, ifNine  # if state != 8
    jal StateEight
    j Next
ifNine:
    addi $t0, $zero, 9
    bne $s0, $t0, ifTen   # if state != 9
    jal StateNine
    j Next
ifTen:    
    addi $t0, $zero, 10
    bne $s0, $t0, ifZero    # if state != 10
    jal StateTen
    
Next:
    add $s0, $zero, $v0     # The state change should be moved into the current state
    addi $s1, $s1, 1

    # Display current state
    add $t0, $zero, $v0     # Since the syscall functions use $a0 and $v0 save off
    add $t1, $zero, $a0     # the values into $t0 and $t1 and restore them later
    li $v0, 4
    la $a0, current_state
    syscall
    li $v0, 1
    move $a0, $s0
    syscall
    li $v0, 4
    la $a0, line_feed
    syscall
    add $v0, $zero, $t0     # Restore the orignal $v0
    add $a0, $zero, $t1
    
    # Let us see if the exit value has been set
    lw $t0, exit_value
    addi $t1, $zero, 1
    beq $t0, $t1, exit
    # Back to the top of the loop
    j while
exit:
    li $v0, 10              # Exit system call
    syscall

#!/usr/bin/perl -w
use English;
use strict;
require Tk;
require Tk::Table;
use Tk;
use Tk::Table;

# Conway's Game of Life

# Gosper Glider Gun
my @current = ( [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1],
                [0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1],
                [1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [1,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0] );
my @next;
my ($xMax, $yMax) = (36, 36);
my $size = 15;
my $canvas;
use vars qw/$repeat/;

# Rules to Conway's Game of Life
# Survivals - Every counter with two or three neighbors survives for the next 
# generation.
#
# Deaths - Every counter with four or more neighbors dies from overpopulation. 
# Every counter with one neighbor or none dies from isolation.
#
# Births - Each empty cell adjacent to exactly three neighbors, no more, no 
# fewer, is a birth cell. A counter is placed on it at the next move.

sub calculateNG {
    @next = ();
    for (my $x = 0; $x < $xMax; $x++) {
        for (my $y = 0; $y < $yMax; $y++) {
            # Who are the people in your neighborhood?
            my $neighbors = 0;
            my $cell = $current[$x][$y];
            # print "X->$x, Y->$y\n";
            # North -> y-1
            if (($y-1 >= 0) && ($current[$x][$y-1] == 1)) {
                $neighbors++;
            }
            # Northeast -> x+1, y-1
            if (($x+1 < $xMax) && ($y-1 >= 0) && ($current[$x+1][$y-1] == 1)) {
                $neighbors++;
            }
            # East -> x+1
            if (($x+1 < $xMax) && ($current[$x+1][$y] == 1)) {
                $neighbors++;
            }
            # Southeast -> x+1, y+1
            if (($x+1 < $xMax) && ($y+1 < $yMax) && ($current[$x+1][$y+1] == 1)) {
                $neighbors++;
            }
            # South -> y+1
            if (($y+1 < $yMax) && ($current[$x][$y+1] == 1)) {
                $neighbors++;
            }
            # Southwest -> x-1, y+1
            if (($x-1 >= 0) && ($y+1 < $yMax) && ($current[$x-1][$y+1] == 1)) {
                $neighbors++;
            }
            # West -> x-1
            if (($x-1 >= 0) && $current[$x-1][$y] == 1) {
                $neighbors++;
            }
            # Northwest -> x-1, y-1
            if (($x-1 >= 0) && ($y-1 >= 0) && $current[$x-1][$y-1] == 1 ) {
                $neighbors++;
            }
            
            
            # Surviors
            if ($cell == 1 && ($neighbors == 2 || $neighbors == 3)) {
                $next[$x][$y] = 1;
            }
            # Babies
            elsif ($cell == 0 && $neighbors == 3) {
                $next[$x][$y] = 1;
            }
            # Dirt
            elsif ($cell == 1 && ($neighbors <= 1 || $neighbors >= 4)) {
                $next[$x][$y] = 0;
            }
            else {
                $next[$x][$y] = 0;
            }
        }
    }
    @current = @next;
}

sub drawGUI {
    
    my ($xMax, $yMax) = ($xMax, $yMax);
    my $Version="1.0";
    my $main = new MainWindow;
        
    $main->title("Conway's Game of Life");

    my $menu_bar=$main->Frame(-relief=>'groove',
                -borderwidth=>3,
                -background=>'grey',
                )->pack('-side'=>'top', -fill=>'x');
                            
    my $file_mb=$menu_bar->Menubutton(-text=>'File',
                -background=>'grey',
                -activebackground=>'grey',
                -foreground=>'Black',
                )->pack(-side=>'left');

        $file_mb->command(-label=>'Run',
                -activebackground=>'grey',
                -command=>\&run);
        
        $file_mb->command(-label=>'Stop',
                -activebackground=>'grey',
                -command=>\&stop);
        
        $file_mb->separator();

        $file_mb->command(-label=>'Exit',
                -activebackground=>'grey',
                -command=>sub{$main->destroy});

    my $edit_mb=$menu_bar->Menubutton(-text=>'Edit',
                -background=>'grey',
                -activebackground=>'grey',
                -foreground=>'Black',
                )->pack(-side=>'left');
        $edit_mb->command(-label=>'Pre-defined Organisms',
                -activebackground=>'grey',
                -command=>\&run);

    my $help_mb=$menu_bar->Menubutton(-text=>'Help',
                -background=>'grey',
                -activebackground=>'grey',
                -foreground=>'black',
                )->pack(-side=>'right');
                        
        $help_mb->command(-label=>'About',
                -activebackground=>'grey',
                -command=>\&about_txt);
        $help_mb->command(-label=>'Help',
                -activebackground=>'grey',
                -command=>\&help_txt);

    $canvas = $main->Canvas(-bg => 'grey',
         -width  => $yMax * $size,
         -height => $xMax * $size,
        )-> pack(qw/-side top/);

    $main-> bind('<Any-Enter>', sub { $canvas->Tk::focus });

    # Draw the grid.
    for my $i (0 .. $xMax) {
        $canvas-> createLine(0, $i * $size,
		       $yMax * $size, $i * $size,
		       -fill =>  'white',
		      );
    }

    for my $i (0 .. $yMax) {
        $canvas->createLine($i * $size, 0,
		       $i * $size, $xMax * $size,
		       -fill =>  'white',
		      );
    }

    my $frame = $main->Frame->pack(qw/-side top -expand 1
				 -fill both -padx 10 -pady 10/);
                 

    my $life_table=$main->Table(-columns => 36,
                                -rows => 36
                                )->pack(qw/-expand 0 -fill x -padx 1/);
    # Fill in the starting pattern.
    fill($canvas);
}

sub fill {
    for (my $x = 0; $x < $xMax; $x++) {
        for (my $y = 0; $y < $yMax; $y++) {
            if ($current[$x][$y] == 1) {
                $canvas->createRectangle(
                    $y * $size, $x * $size,
                    ($y + 1) * $size, ($x + 1) * $size,
                    -fill => 'red',
                    -tags => "$x-$y",
                    );
            }
        }
    }
}

sub unfill {
    for (my $x = 0; $x < $xMax; $x++) {
        for (my $y = 0; $y < $yMax; $y++) {
            if ($current[$x][$y] == 1) {
                $canvas->delete("$x-$y");
            }
        }
    }
}

sub run {
    $repeat = $canvas->repeat(200, \&step);
}

sub stop {
    $repeat->cancel();
}

sub step {
    unfill();
    calculateNG();
    fill();
    
    $canvas->update();
}

drawGUI();
MainLoop();
#while (1) {
    #system("clear");
    #calculateNG();
    #printGeneration(@current)

    #sleep 1;
#}
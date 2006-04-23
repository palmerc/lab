#!/usr/bin/perl -w
use English;
use strict;
require Tk;
require Tk::DialogBox;
use Tk;

# Conway's Game of Life

# Glider
my @block = ( [1,1],
              [1,1] );

my @boat = ( [1,1,0],
             [1,0,1],
             [0,1,0] );
             
my @blinker = ( [1,1,1] );

my @toad = ( [0,1,1,1],
             [1,1,1,0] );
             
my @glider = ( [0,1,0],
               [0,0,1],
               [1,1,1] );

my @lwss = ( [1,0,0,1,0],
             [0,0,0,0,1],
             [1,0,0,0,1],
             [0,1,1,1,1] );
             
my @diehard = ( [0,0,0,0,0,0,1,0],
                [1,1,0,0,0,0,0,0],
                [0,1,0,0,0,1,1,1] );

my @acorn = ( [0,1,0,0,0,0,0],
              [0,0,0,1,0,0,0],
              [1,1,0,0,1,1,1] );

# Gosper Glider Gun
my @gosper = ( [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0],
               [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0],
               [0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1],
               [0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1],
               [1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
               [1,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0],
               [0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0],
               [0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
               [0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0] );


my @cameron = ( [1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1],
                [1,0,0,0,0,0,0,0,1,0,1,0,0,0,1,1,0,1,1],
                [1,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,1,0,1],
                [1,0,0,0,0,0,1,1,1,1,1,1,1,0,1,0,0,0,1],
                [1,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,1],
                [1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1] );


my @current;
my @next;
my ($xMax, $yMax) = (60, 60);
my $size = 15;
my $canvas;
my $repeat;
my $main;

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



sub run {
    $repeat = $canvas->repeat(200, \&step);
}

sub stop {
    if ($repeat) {
        $repeat->cancel();
    }
}

sub step {
    unfill();
    calculateNG();
    fill();
    
    $canvas->update();
}

sub initializeArray {
    # Initialize the default array
    for (my $x = 0; $x < $xMax; $x++) {
        for (my $y = 0; $y < $yMax; $y++) {
            $current[$x][$y] = 0;
        }
    }    
}

sub copyShapeInto {
    my (@array) = @_;
    my $rows = $#{$array[0]} + 1;
    my $cols = $#array + 1;
    
    my $centerx = ($xMax - $cols) / 2;
    my $centery = ($yMax - $rows) / 2;
    
    for (my $x = 0; $x < $cols; $x++) {
        for (my $y = 0; $y < $rows; $y++) {
            $current[$x+$centerx][$y+$centery] = $array[$x][$y];
        }
    }    
}

sub drawGUI {
    my $Version="1.0";
    
    $main = new MainWindow;    
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
        $edit_mb->command(-label=>'Grid Size',
                -activebackground=>'grey',
                -command=> \&grid_dialog);


    my $help_mb=$menu_bar->Menubutton(-text=>'Help',
                -background=>'grey',
                -activebackground=>'grey',
                -foreground=>'black',
                )->pack(-side=>'right');
                        
        $help_mb->command(-label=>'About',
                -activebackground=>'grey',
                -command=> \&about_txt);
                
    gridDraw();

}

sub gridDraw {
    if ($canvas) {
        $canvas->destroy();
    }
    $canvas = $main->Canvas(-bg => 'grey',
         -width  => $yMax * $size,
         -height => $xMax * $size,
        )->pack(qw/-side top -fill both/);

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
                 
    # Fill in the starting pattern.
    fill($canvas);    
}

sub about_txt {
  my $popup=$main->DialogBox(-title=>"About",
  -buttons=>["OK"],);
  $popup->add("Label", -text=>"Conway's Game of Life\nCameron Palmer, Mitra Mahadavian, and Kristina Ratliff\nCSCE 4930\nApril 2006\n")->pack;
  $popup->Show;
}

sub grid_dialog {
  my $popup=$main->DialogBox(-title=>"Grid Size", -background => 'grey',
  -buttons=>["OK", "Cancel"],);
  
    my $right1 = $popup->Frame(-background => 'grey')->pack(-side => 'left', -pady => 2, -padx => 2);
    my $des1 = $right1->Label(-text => 'X Size', -background => 'grey', -pady => 4)->pack();
    my $des2 = $right1->Label(-text => 'Y Size', -background => 'grey', -pady => 4)->pack();
    my $right2 = $popup->Frame(-background => 'grey')->pack(-side => 'left', -pady => 2, -padx => 2);
    my $ent0 = $right2->Entry(-width => 5)->pack(-anchor=>'n',-fill=>'x');
    my $ent1 = $right2->Entry(-width => 5)->pack(-anchor=>'n',-fill=>'x');
    $ent0->insert('end',$xMax);
    $ent1->insert('end',$yMax);
    my $button = $popup->Show;
  
    if ($button eq "OK") {
        my $x = $ent0->get();
        my $y = $ent1->get();
        if ($x > 10 and $y > 10) {
            $xMax = $x;
            $yMax = $y;
            initializeArray();
            copyShapeInto(@gosper);
            gridDraw();
        }
    }
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

####
#### Main
####

initializeArray();
copyShapeInto(@gosper);
drawGUI();
MainLoop();

#!/usr/bin/perl -w

# Conway's Game of Life
# Copyright (C) 2006 Cameron Palmer

# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

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
my ($colsMax, $rowsMax) = (60, 60);
my $size = 15;
my $canvas;
my $repeat;
my $main;
my $generation_counter;
my $status_bar;

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
    for (my $rows = 0; $rows < $rowsMax; $rows++) {
        for (my $cols = 0; $cols < $colsMax; $cols++) {
            # Who are the people in your neighborhood?
            my $neighbors = 0;
            my $cell = $current[$cols][$rows];
            #print "X->$cols, Y->$rows\n";
            # North -> y-1
            if (($rows-1 >= 0) && ($current[$cols][$rows-1] == 1)) {
                $neighbors++;
            }
            # Northeast -> x+1, y-1
            if (($cols+1 < $colsMax) && ($rows-1 >= 0) && ($current[$cols+1][$rows-1] == 1)) {
                $neighbors++;
            }
            # East -> x+1
            if (($cols+1 < $colsMax) && ($current[$cols+1][$rows] == 1)) {
                $neighbors++;
            }
            # Southeast -> x+1, y+1
            if (($cols+1 < $colsMax) && ($rows+1 < $rowsMax) && ($current[$cols+1][$rows+1] == 1)) {
                $neighbors++;
            }
            # South -> y+1
            if (($rows+1 < $rowsMax) && ($current[$cols][$rows+1] == 1)) {
                $neighbors++;
            }
            # Southwest -> x-1, y+1
            if (($cols-1 >= 0) && ($rows+1 < $rowsMax) && ($current[$cols-1][$rows+1] == 1)) {
                $neighbors++;
            }
            # West -> x-1
            if (($cols-1 >= 0) && $current[$cols-1][$rows] == 1) {
                $neighbors++;
            }
            # Northwest -> x-1, y-1
            if (($cols-1 >= 0) && ($rows-1 >= 0) && $current[$cols-1][$rows-1] == 1 ) {
                $neighbors++;
            }
            
            
            # Surviors
            if ($cell == 1 && ($neighbors == 2 || $neighbors == 3)) {
                $next[$cols][$rows] = 1;
            }
            # Babies
            elsif ($cell == 0 && $neighbors == 3) {
                $next[$cols][$rows] = 1;
            }
            # Dirt
            elsif ($cell == 1 && ($neighbors <= 1 || $neighbors >= 4)) {
                $next[$cols][$rows] = 0;
            }
            else {
                $next[$cols][$rows] = 0;
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
    $generation_counter++;
    counterUpdate();
    unfill();
    calculateNG();
    fill();
    $canvas->update();
    $status_bar->update();
}

sub initializeArrays {
    $generation_counter = 0;
    # Initialize the default array
    for (my $rows = 0; $rows < $rowsMax; $rows++) {
        for (my $cols = 0; $cols < $colsMax; $cols++) {
            $current[$cols][$rows] = 0;
        }
    }    
}

sub copyShapeInto {
    my (@array) = @_;
    my $y = $#array + 1;
    my $x = $#{$array[0]} + 1;
    
    my $centerx = ($colsMax - $x) / 2;
    my $centery = ($rowsMax - $y) / 2;
    
    for (my $rows = 0; $rows < $y; $rows++) {
        for (my $cols = 0; $cols < $x; $cols++) {
            $current[$cols+$centerx][$rows+$centery] = $array[$rows][$cols];
        }
    }    
}

sub drawGUI {
    my $Version="1.0";
    
    $main = new MainWindow;
    #$main->resizable(0, 0);
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

        $file_mb->command(-label=>'Open',
                -activebackground=>'grey',
                -command=>\&open_file);
        
        $file_mb->command(-label=>'Save',
                -activebackground=>'grey',
                -command=>\&save_file);
        
        $file_mb->command(-label=>'To PostScript',
                -activebackground=>'grey',
                -command=>\&postit);
        
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
        $edit_mb->command(-label=>'Shape',
                -activebackground=>'grey',
                -command=> \&shape_dialog);
        $edit_mb->command(-label=>'Clear Screen',
                -activebackground=>'grey',
                -command=> \&clr_screen);
    
    my $run_mb=$menu_bar->Menubutton(-text=>'Run',
                -background=>'grey',
                -activebackground=>'grey',
                -foreground=>'Black',
                )->pack(-side=>'left');
        
        $run_mb->command(-label=>'Start',
                -activebackground=>'grey',
                -command=>\&run);
        
        $run_mb->command(-label=>'Stop',
                -activebackground=>'grey',
                -command=>\&stop);



    my $help_mb=$menu_bar->Menubutton(-text=>'Help',
                -background=>'grey',
                -activebackground=>'grey',
                -foreground=>'black',
                )->pack(-side=>'right');
                        
        $help_mb->command(-label=>'About',
                -activebackground=>'grey',
                -command=> \&about_txt);
                
    gridDraw();
    $status_bar = $main->Label()->pack(-side => 'bottom', -anchor => 'w');
    counterUpdate();
}

sub gridDraw {
    if ($canvas) {
        $canvas->destroy();
    }
    
    $canvas = $main->Canvas(-bg => 'grey',
         -width  => $colsMax * $size,
         -height => $rowsMax * $size,
        )->pack(qw/-side top -fill both/);
    
    $canvas->CanvasBind("<1>"=> sub {
        my ($x, $y) = ($Tk::event->x, $Tk::event->y);
        #print "loc $x-$y\n";
        my $cols = int($x / $size);
        my $rows = int($y / $size);
        
        if ($current[$cols][$rows] == 0) {
            #print "create $cols-$rows\n";
            $canvas->createRectangle($cols * $size, $rows * $size,
                    ($cols + 1) * $size, ($rows + 1) * $size,
                    -tags => "$cols-$rows",
                    -fill => 'red');
            $current[$cols][$rows] = 1;
        } elsif ($current[$cols][$rows] == 1) {
            #print "DELETE $cols-$rows\n";
            $canvas->delete("$cols-$rows");
            $canvas->update();
            $current[$cols][$rows] = 0;
        }    
        #print "array value $current[$cols][$rows]\n";
    });

    # Draw the grid.
    for my $i (0 .. $colsMax) {
        $canvas-> createLine(0, $i * $size,
		       $rowsMax * $size, $i * $size,
		       -fill =>  'white',
		      );
    }

    for my $i (0 .. $rowsMax) {
        $canvas->createLine($i * $size, 0,
		       $i * $size, $colsMax * $size,
		       -fill =>  'white',
		      );
    }
                 
    # Fill in the starting pattern.
    fill($canvas);
}

sub counterUpdate {       
    $status_bar->configure(-text => "Generation: $generation_counter");
}

sub shape_dialog {
    my $shape;
    my $popup=$main->DialogBox(-title=>"Shapes", -background => 'grey',
            -buttons=>["OK", "Cancel"],);
    
    my $right1 = $popup->Frame(-background => 'grey')->pack(-side => 'left', -pady => 2, -padx => 2);
    my $des1 = $right1->Label(-text => 'Choose from', -background => 'grey', -pady => 4)->pack(-side => 'left');
    my $rdb0 = $right1-> Radiobutton(-text=>"Gosper Glider Gun", -background => 'grey',
            -activebackground=>'grey', -highlightbackground => 'grey', -value=>"gosper",  -variable=>\$shape)->pack(-side => 'top', -anchor=>'w');
    my $rdb1 = $right1-> Radiobutton(-text=>"Lightweight Spaceship", -background => 'grey',
            -activebackground=>'grey', -highlightbackground => 'grey', -value=>"lwss",  -variable=>\$shape)->pack(-side => 'top', -anchor=>'w');
    my $rdb2 = $right1-> Radiobutton(-text=>"Cameron", -background => 'grey',
            -activebackground=>'grey', -highlightbackground => 'grey', -value=>"cameron",  -variable=>\$shape)->pack(-side => 'top', -anchor=>'w');
    my $rdb3 = $right1-> Radiobutton(-text=>"Acorn", -background => 'grey',
            -activebackground=>'grey', -highlightbackground => 'grey', -value=>"acorn",  -variable=>\$shape)->pack(-side => 'top', -anchor=>'w');
    my $rdb4 = $right1-> Radiobutton(-text=>"Diehard", -background => 'grey',
            -activebackground=>'grey', -highlightbackground => 'grey', -value=>"diehard",  -variable=>\$shape)->pack(-side => 'top', -anchor=>'w');

    my $button = $popup->Show();
    
    if ($button eq "OK") {
        initializeArrays();
        if ($shape eq "cameron") {
            copyShapeInto(@cameron);
        } elsif ($shape eq "gosper") {
            copyShapeInto(@gosper);
        } elsif ($shape eq "lwss") {
            copyShapeInto(@lwss);
        } elsif ($shape eq "acorn") {
            copyShapeInto(@acorn);
        } elsif ($shape eq "diehard") {
            copyShapeInto(@diehard);
        } else {
            print "How on earth did you get here?\n";
        }
            
        gridDraw();
        counterUpdate();
    }
}

sub postit {
    my $popup=$main->DialogBox(-title=>"PostScript Ouput", -background => 'grey',
            -buttons=>["OK", "Cancel"],);
    my $right1 = $popup->Frame(-background => 'grey')->pack(-side => 'left', -pady => 2, -padx => 2);
    my $des1 = $right1->Label(-text => 'File name', -background => 'grey', -pady => 4)->pack();
    my $right2 = $popup->Frame(-background => 'grey')->pack(-side => 'left', -pady => 2, -padx => 2);
    my $ent0 = $right2->Entry(-width => 15)->pack(-anchor=>'n',-fill=>'x');
    
    my $button = $popup->Show();
    
    if ($button eq "OK") {
        my $file = $ent0->get();
        $canvas->update();
        $canvas->postscript(-file => $file);
    }
}

sub open_file {
    initializeArrays();
    my @pixels;
    
    my $popup=$main->DialogBox(-title=>"Open Game", -background => 'grey',
            -buttons=>["OK", "Cancel"],);
    my $right1 = $popup->Frame(-background => 'grey')->pack(-side => 'left', -pady => 2, -padx => 2);
    my $des1 = $right1->Label(-text => 'File name', -background => 'grey', -pady => 4)->pack();
    my $right2 = $popup->Frame(-background => 'grey')->pack(-side => 'left', -pady => 2, -padx => 2);
    my $ent0 = $right2->Entry(-width => 15)->pack(-anchor=>'n',-fill=>'x');
    
    my $button = $popup->Show();
    
    if ($button eq "OK") {
        my ($rows, $cols);
        my $file = $ent0->get();
        
        open(FH, "<$file");
        my $line_number = 0;
        foreach my $line (<FH>) {
            @pixels = split ',', $line;
            my $pixel_number = 0;
            foreach my $el (@pixels) {
                $current[$pixel_number][$line_number] = $el;
                $pixel_number++;
            }
            $line_number++;
        }
        close FH;
    }
    gridDraw();
}

sub save_file {
    my $popup=$main->DialogBox(-title=>"Save Game", -background => 'grey',
            -buttons=>["OK", "Cancel"],);
    my $right1 = $popup->Frame(-background => 'grey')->pack(-side => 'left', -pady => 2, -padx => 2);
    my $des1 = $right1->Label(-text => 'File name', -background => 'grey', -pady => 4)->pack();
    my $right2 = $popup->Frame(-background => 'grey')->pack(-side => 'left', -pady => 2, -padx => 2);
    my $ent0 = $right2->Entry(-width => 15)->pack(-anchor=>'n',-fill=>'x');
    
    my $button = $popup->Show();
    
    if ($button eq "OK") {
        my ($rows, $cols);
        my $file = $ent0->get();
        
        open(FH, ">$file");
        for ($rows = 0; $rows < $rowsMax; $rows++) {
            for ($cols = 0; $cols < $colsMax; $cols++) {
                if (($cols + 1) == $colsMax) {
                    print FH "$current[$cols][$rows]\n";
                } else {
                    print FH "$current[$cols][$rows],";
                }
                
            }
        }
        close FH;
    }
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
    $ent0->insert('end',$colsMax);
    $ent1->insert('end',$rowsMax);
    my $button = $popup->Show;
  
    if ($button eq "OK") {
        my $cols = $ent0->get();
        my $rows = $ent1->get();
        if ($cols > 10 and $rows > 10) {
            $colsMax = $cols;
            $rowsMax = $rows;
            initializeArrays();
            gridDraw();
        }
    }
}

sub clr_screen {
    initializeArrays();
    gridDraw();
}

sub fill {
    for (my $rows = 0; $rows < $rowsMax; $rows++) {
        for (my $cols = 0; $cols < $colsMax; $cols++) {
            if ($current[$cols][$rows] == 1) {
                $canvas->createRectangle(
                    $cols * $size, $rows * $size,
                    ($cols + 1) * $size, ($rows + 1) * $size,
                    -fill => 'red',
                    -tags => "$cols-$rows",
                );
            }
        }
    }
}

sub unfill {
    for (my $rows = 0; $rows < $rowsMax; $rows++) {
        for (my $cols = 0; $cols < $colsMax; $cols++) {
            if ($current[$cols][$rows] == 1) {
                $canvas->delete("$cols-$rows");
            }
        }
    }
}

####
#### Main Program
####

initializeArrays();
drawGUI();
MainLoop();

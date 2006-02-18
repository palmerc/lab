<?php
   $title = "Sudoku";
   require 'sudoku-template.php';

function display_message() {
   echo"
<p>So I want a web-based Sudoku solver, and maybe a generator. The idea is to solve
the Permuation Bipartite Graph using a Pile and Chain Exclusion, from ideas 
culled from Dr. Dobb's.</p>

<p>CURRENT STATE: Got a decent looking CSS layout, and input works. The values are
stored in an array and don't disappear on submit. Includes a starting hardcoded
puzzle to work with.</p>
      ";
}

function display_conflicts($conflict_values) {
   echo '<div class="leftside"><ol>';
   for ($i = 0; $i < 9; $i++) { 
      for ($j = 0; $j < 9; $j++) {
         if (count($conflict_values[$i][$j]) == 0) continue;
         $conflict_temp = array_unique($conflict_values[$i][$j]);
         sort($conflict_temp);
         $values = implode(',', $conflict_temp);
         $row = $i + 1;
         $col = $j + 1;

         echo "<li>Conflicts Row $row Col $col -> $values</li>\n";
      }
   }
   echo '</ol></div>';
}

function display_potentials($potential_values) {
   echo '<div class="leftside"><ol>';
   for ($i = 0; $i < 9; $i++) { 
      for ($j = 0; $j < 9; $j++) {
         if (count($potential_values[$i][$j]) == 0) continue;
         $potential_temp = array_unique($potential_values[$i][$j]);
         sort($potential_temp);
         $values = implode(',', $potential_temp);
         $row = $i + 1;
         $col = $j + 1;

         echo "<li>Potentials Row $row Col $col -> $values</li>\n";
      }
   }
   echo '</ol></div>';
}

function draw_sudoku($sudoku) { 
?>
   <form class="inputform" method="post" action="<?php echo $_SERVER['PHP_SELF'] ?>">
      <table class="sudoku">
   
      <?php
      for ($i = 0; $i < 9; $i++) {
         echo '<tr';
         if ($i != 0 && ($i % 3) == 0) {
            echo ' class="thickline">';
         } else {
            echo '>';
         }
            
         for ($j = 0; $j < 9; $j++) {
            echo '<td';
            if ($j != 0 && ($j %3) == 0) {
               echo ' class="thickline">';
            } else {
               echo '>';
            }
            if ($sudoku[$i][$j] > 0) {
               $cellvalue = $sudoku[$i][$j];
            } else {
               $cellvalue = '';
            }
            echo "<input name=\"sudoku[{$i}][{$j}]\" value=\"{$cellvalue}\"";
            echo ' type="text" size="1" maxlength="1" />';
            
            echo '</td>';
         }
         echo "</tr>\n";
      }
      ?>
      </table>
      <br />
      <input type="submit" value="Submit" />
   </form>
<?php
}

function box_conflicts($sudoku) {
   for ($i = 0; $i < 9; $i++) {
      for ($j = 0; $j < 9; $j++) {
         if ($sudoku[$i][$j] == '') continue;
         if ($i < 3 && $j < 3)
            $box0[] = $sudoku[$i][$j];
         else if ($i < 3 && $j < 6)
            $box1[] = $sudoku[$i][$j];
         else if ($i < 3 && $j < 9)
            $box2[] = $sudoku[$i][$j];
         else if ($i < 6 && $j < 3)
            $box3[] = $sudoku[$i][$j];
         else if ($i < 6 && $j < 6)
            $box4[] = $sudoku[$i][$j];
         else if ($i < 6 && $j < 9)
            $box5[] = $sudoku[$i][$j];
         else if ($i < 9 && $j < 3)
            $box6[] = $sudoku[$i][$j];
         else if ($i < 9 && $j < 6)
            $box7[] = $sudoku[$i][$j];
         else if ($i < 9 && $j < 9)
            $box8[] = $sudoku[$i][$j];
      }
   }
   
   return array($box0, $box1, $box2, $box3, $box4, $box5, $box6, $box7, $box8);
}

function conflicts($sudoku) {
   $box_conflicts = box_conflicts($sudoku);
   for ($i = 0; $i < 9; $i++) {
      for ($j = 0; $j < 9; $j++) {
         $conflict_temp = array();
         $cell_value = $sudoku[$i][$j];
         if ($cell_value != '') continue;
         // For the current square try the row and columns to see if a value conflicts
         for ($row = 0; $row < 9; $row++) {
            if ($row == $i) continue;
            if ($sudoku[$row][$j] == '') continue;
            $conflict_temp[] = $sudoku[$row][$j];
         }
         for ($col = 0; $col < 9; $col++) {
            if ($col == $j) continue;
            if ($sudoku[$i][$col] == '') continue;
            $conflict_temp[] = $sudoku[$i][$col];
         }
         if ($i < 3 && $j < 3)
            $conflict_temp = array_merge($conflict_temp, $box_conflicts[0]);
         else if ($i < 3 && $j < 6)
            $conflict_temp = array_merge($conflict_temp, $box_conflicts[1]);
         else if ($i < 3 && $j < 9)
            $conflict_temp = array_merge($conflict_temp, $box_conflicts[2]);
         else if ($i < 6 && $j < 3)
            $conflict_temp = array_merge($conflict_temp, $box_conflicts[3]);
         else if ($i < 6 && $j < 6)
            $conflict_temp = array_merge($conflict_temp, $box_conflicts[4]);
         else if ($i < 6 && $j < 9)
            $conflict_temp = array_merge($conflict_temp, $box_conflicts[5]);
         else if ($i < 9 && $j < 3)
            $conflict_temp = array_merge($conflict_temp, $box_conflicts[6]);
         else if ($i < 9 && $j < 6)
            $conflict_temp = array_merge($conflict_temp, $box_conflicts[7]);
         else if ($i < 9 && $j < 9)
            $conflict_temp = array_merge($conflict_temp, $box_conflicts[8]);
         $conflict_values[$i][$j] = $conflict_temp;
      }
   }
   return $conflict_values;
}

function potentials($conflict_values) {
   for ($i = 0; $i < 9; $i++) {
      for ($j = 0; $j < 9; $j++) {
         $potentials = array(1,2,3,4,5,6,7,8,9);
         if (count($conflict_values[$i][$j]) == 0) continue;
         $conflict_temp = $conflict_values[$i][$j];
         $conflict_temp = array_unique($conflict_temp);
         sort($conflict_temp);
         
         $potential_values[$i][$j] = array_diff($potentials, $conflict_temp);
      }
   }
   return $potential_values;
}

function possible_values($sudoku) {
   $conflict_values = conflicts($sudoku);
   display_conflicts($conflict_values);
   $potential_values = potentials($conflict_values);
   display_potentials($potential_values);
}

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
   for ($i = 0; $i < 9; $i++) {
      for ($j = 0; $j < 9; $j++) {
         $sudoku[$i][$j] = $_POST['sudoku'][$i][$j];
      }
   }
   display_message();
   draw_sudoku($sudoku);
   possible_values($sudoku);
//   print_r($_POST);
} else {
   # Short term solution taken from Sudoku to go by Michael Mepham
   $sudoku[0][1] = '6';
   $sudoku[0][5] = '3';
   $sudoku[0][7] = '5';
   $sudoku[1][0] = '4';
   $sudoku[1][3] = '8';
   $sudoku[1][6] = '7';
   $sudoku[2][1] = '2';
   $sudoku[2][6] = '1';
   $sudoku[2][7] = '9';
   $sudoku[2][8] = '8';
   $sudoku[3][0] = '2';
   $sudoku[3][4] = '7';
   $sudoku[3][5] = '5';
   $sudoku[3][7] = '8';
   $sudoku[4][4] = '6';
   $sudoku[5][1] = '8';
   $sudoku[5][3] = '4';
   $sudoku[5][4] = '3';
   $sudoku[5][8] = '2';
   $sudoku[6][0] = '7';
   $sudoku[6][1] = '9';
   $sudoku[6][2] = '5';
   $sudoku[6][7] = '1';
   $sudoku[7][2] = '4';
   $sudoku[7][5] = '7';
   $sudoku[7][8] = '9';
   $sudoku[8][1] = '1';
   $sudoku[8][3] = '5';
   $sudoku[8][7] = '4';
   display_message();
   draw_sudoku($sudoku);
}

?>
<?php
   $title = "Sudoku";
   require 'sudoku-template.php';

function possible_values($sudoku) {
   // Move cell by cell and test values
   echo '<div class="leftside"><ol>';
   for ($i = 0; $i < 9; $i++) {
      for ($j = 0; $j < 9; $j++) {
         $conflict_values = array();
         $cell_value = $sudoku[$i][$j];
         if ($cell_value != '') continue;
         // For the current square try the row and columns to see if a value conflicts
         for ($row = 0; $row < 9; $row++) {
            if ($row == $i) continue;
            if ($sudoku[$row][$j] == '') continue;
            $conflict_values[] = $sudoku[$row][$j];
         }
         for ($col = 0; $col < 9; $col++) {
            if ($col == $j) continue;
            if ($sudoku[$i][$col] == '') continue;
            $conflict_values[] = $sudoku[$i][$col];
         }
         $conflict_values = array_unique($conflict_values);
         sort($conflict_values);
         $values = implode(',', $conflict_values);
         $row = $i + 1;
         $col = $j + 1;
         echo "<li>Conflicts Row $row Col $col -> $values</li>\n";
      }
   }
   echo '</ol></div>';
}

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
   for ($i = 0; $i < 9; $i++) {
      for ($j = 0; $j < 9; $j++) {
         $sudoku[$i][$j] = $_POST['sudoku'][$i][$j];
      }
   }
   possible_values($sudoku);
//   print_r($_POST);
} else {
   # Short term solution taken from Sudoku to go by Michael Mepham
   $_POST['sudoku'][0][1] = '6';
   $_POST['sudoku'][0][5] = '3';
   $_POST['sudoku'][0][7] = '5';
   $_POST['sudoku'][1][0] = '4';
   $_POST['sudoku'][1][3] = '8';
   $_POST['sudoku'][1][6] = '7';
   $_POST['sudoku'][2][1] = '2';
   $_POST['sudoku'][2][6] = '1';
   $_POST['sudoku'][2][7] = '9';
   $_POST['sudoku'][2][8] = '8';
   $_POST['sudoku'][3][0] = '2';
   $_POST['sudoku'][3][4] = '7';
   $_POST['sudoku'][3][5] = '5';
   $_POST['sudoku'][3][7] = '8';
   $_POST['sudoku'][4][4] = '6';
   $_POST['sudoku'][5][1] = '8';
   $_POST['sudoku'][5][3] = '4';
   $_POST['sudoku'][5][4] = '3';
   $_POST['sudoku'][5][8] = '2';
   $_POST['sudoku'][6][0] = '7';
   $_POST['sudoku'][6][1] = '9';
   $_POST['sudoku'][6][2] = '5';
   $_POST['sudoku'][6][7] = '1';
   $_POST['sudoku'][7][2] = '4';
   $_POST['sudoku'][7][5] = '7';
   $_POST['sudoku'][7][8] = '9';
   $_POST['sudoku'][8][1] = '1';
   $_POST['sudoku'][8][3] = '5';
   $_POST['sudoku'][8][7] = '4';
}

?>



<p>So I want a web-based Sudoku solver, and maybe a generator. The idea is to solve
the Permuation Bipartite Graph using a Pile and Chain Exclusion, from ideas 
culled from Dr. Dobb's.</p>

<p>CURRENT STATE: Got a decent looking CSS layout, and input works. The values are
stored in an array and don't disappear on submit. Includes a starting hardcoded
puzzle to work with.</p>

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
         if ($_POST['sudoku'][$i][$j] > 0) {
            $cellvalue = $_POST['sudoku'][$i][$j];
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

<br class="leftside" />
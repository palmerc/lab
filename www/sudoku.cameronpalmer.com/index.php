<?php
   $title = "Sudoku";
   require 'sudoku-template.php';
?>

<p>So I want a web-based Sudoku solver, and maybe a generator. The idea is to solve
the Permuation Bipartite Graph using a Pile and Chain Exclusion, from ideas 
culled from Dr. Dobb's.</p>

<table class="sudoku">

<?php
for ($i = 0; $i < 9; $i++) {
   if ($i != 0 && ($i % 3) == 0) {
      print '<tr class="thickline">';
   } else {
      print '<tr>';
   }
      
   for ($j = 0; $j < 9; $j++) {
      if ($j != 0 && ($j %3) == 0) {
         print '<td class="thickline">';
      } else {
         print '<td>';
      }
      print '<input type="text" size="2" /></td>';
   }
   print '</tr>';
}
?>
</table>

<br class="leftside" />
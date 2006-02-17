<?php
   require 'sudoku-template.php';
?>

<h1>Sudoku</h1>
<p>So I want a web-based Sudoku solver, and maybe a generator. The idea is to solve
the Permuation Bipartite Graph using a Pile and Chain Exclusion, from ideas 
culled from Dr. Dobb's.</p>

<table border="1">

<?php
for ($i = 0; $i < 9; $i++) {
   print '<tr>';
   for ($j = 0; $j < 9; $j++) {
      print "<td><input type=\"text\" size=\"2\" /></td>";
   }
   print '</tr>';
}
?>

</table>
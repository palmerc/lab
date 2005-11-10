<?php
	$title = "CSCE 2410 - PHP Program 2";
	$section = "Assignment: Generate a Static Multiplication Table";
	require("../../php-template.php");
?>
<div>
	<table class="progtable">
<?php
echo'
		<!-- The table head -->
        <tr>
            <th>
                &nbsp;
            </th>
	';
            for ($i=1; $i < 13; $i++) {
                echo "<th>{$i}</th>";
            }
echo'
		</tr>
		<!-- Generate the table -->
	';
	for ($i=1; $i < 13; $i++) {
		echo"
			<tr>
				<th>{$i}</th>
			";
			for ($j=1; $j < 13; $j++) {
				echo "<td>" . $i * $j. "</td>";
			}
		echo"
			</tr>
			";
	}

?>
    </table>
</div>
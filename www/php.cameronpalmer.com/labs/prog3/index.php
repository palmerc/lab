<?php
	$title = "CSCE 2410 - PHP Program 3";
	$section = "Assignment: Generate a Multiplication Table";
	require("../../php-template.php");
	$default_size = 12;
	$min_size = 2;
	$max_size = 12;
	$table_size = $default_size;
	
if ($_SERVER['REQUEST_METHOD'] == "POST"){
	if ($_REQUEST['text_field']) $table_size = $_REQUEST['text_field'];
	if ($_REQUEST['table_select']) $table_size = $_REQUEST['table_select'];
	if ($_REQUEST['radio_button']) $table_size = $_REQUEST['radio_button'];
	if ($table_size > $max_size and $table_size < $min_size) {
		echo "<p>Value out of range, must be between {$min_size} and {$max_size}</p>\n";
		$table_size = 12;
	}
}

?>

<div>
	<form method="post" action="?">
		<input name="text_field" type="text" size="3" maxlength="2" value="<?php echo "{$table_size}"; ?>" />
		<input type="submit" name="text_submit_button" value="Submit" />
		<select name="table_select" />
			<?php
			for ($i = $min_size; $i <= $max_size; $i++){
				echo "<option>{$i}</option>\n";
			}
			?>
		</select>
		<input type="submit" name="select_submit_button" value="Submit" /> 	
		<?php
		for ($i = $min_size; $i <= $max_size; $i++){
			echo "{$i}<input type=\"radio\" name=\"radio_button\" value=\"{$i}\" ";
				if ($_SERVER['REQUEST_METHOD'] != "POST" and $i == $max_size){
					echo 'checked="checked" ';
				}
			echo "/>\n";
		}
		?>
		<input type="submit" name="radio_submit_button" value="Submit" /> 	
	</form>
</div>

<div>
	<table id="progtable">
<?php
echo'
		<!-- The table head -->
        <tr>
            <th>
                &nbsp;
            </th>
	';
            for ($i=1; $i <= $table_size; $i++) {
                echo "<th>{$i}</th>";
            }
echo'
		</tr>
		<!-- Generate the table -->
	';
	for ($i=1; $i <= $table_size; $i++) {
		echo"
			<tr>
				<th>{$i}</th>
			";
			for ($j=1; $j <= $table_size; $j++) {
				echo "<td>" . $i * $j. "</td>";
			}
		echo"
			</tr>
			";
	}

?>
    </table>
</div>
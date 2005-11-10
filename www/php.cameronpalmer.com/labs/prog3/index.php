<?php
	$title = "CSCE 2410 - PHP Program 3";
	$section = "Assignment: Generate a Dynamic Multiplication Table";
	require("../../php-template.php");
	$default_size = 12;
	$min_size = 2;
	$max_size = 12;
	$table_size = $default_size;
	
if ($_SERVER['REQUEST_METHOD'] == "POST"){
	// Get the last value entered so that we can stay on the same value when illegal input is given
	$last_size = $_REQUEST['last_size'];
	// Check to see which button was hit and grab the value
	if (isset($_POST['text_submit_button'])){
		$table_size = $_REQUEST['text_field'];
	} elseif (isset($_POST['select_submit_button'])){
		$table_size = $_REQUEST['table_select'];
	} elseif (isset($_POST['radio_submit_button'])){
		$table_size = $_REQUEST['radio_button'];
	}
	// Validate the result. Is it in the range of acceptable values?
	if ($table_size > $max_size or $table_size < $min_size) {
		if (is_int($table_size)) {
			echo "<p class=\"error\">Value out of range, must be between {$min_size} and {$max_size}</p>\n";
		} else {
			echo "<p class=\"error\">Non-integer, must be an integer between {$min_size} and {$max_size}</p>\n";
		}
		$table_size = $last_size;
	} else {
		echo "<p class=\"nonerror\">Value should be between {$min_size} and {$max_size}</p>\n";
		$last_size = $table_size;
	}
	// If it isn't a POST populate $last_size with the default 
} else {
	echo "<p class=\"nonerror\">Value should be between {$min_size} and {$max_size}</p>\n";
	$last_size = $default_size;
}

?>

<!-- BEGIN Generate the form -->
<div class="form">
	<form method="post" action="<?php echo $_SERVER['PHP_SELF']; ?>">
		<input type="hidden" name="last_size" value="<?php echo $last_size; ?>" />
		<div id="text_input">			
			<input name="text_field" type="text" size="3" maxlength="2" value="<?php echo $table_size; ?>" />
			<input type="submit" name="text_submit_button" value="Submit" />
		</div>
		<div id="select_input">
			<select name="table_select">
				<!-- BEGIN Select Options -->
				<?php
				for ($i = $min_size; $i <= $max_size; $i++){
					echo "
				<option value=\"{$i}\" "; 
					if ($i == $table_size){
						echo 'selected="selected"';
					}
					echo ">{$i}</option>\n";
				}
				?>
				<!-- END Select Options -->
			</select>
			<input type="submit" name="select_submit_button" value="Submit" />
		</div>
		
		<div id="radio_input">
			<!-- BEGIN Radio Options -->
			<?php
			for ($i = $min_size; $i <= $max_size; $i++){
				echo "
			<label for=\"Radio{$i}\">{$i}</label><input id=\"Radio{$i}\" type=\"radio\" name=\"radio_button\" value=\"{$i}\" ";
					if ($i == $table_size){
						echo 'checked="checked" ';
					}
				echo "/>\n";
			}
			?>
			<!-- END Radio Options -->
			<input type="submit" name="radio_submit_button" value="Submit" />
		</div>
	</form>
</div>
<!-- END Generate the form -->
<!-- BEGIN Generate the multiplication table -->
		<?php table_generate($table_size); ?>
<!-- END Generate the multiplication table -->

<?php
function table_generate($table_size){
		echo'
		<table class="progtable">
		<!-- The table head -->
		<tr><th>&nbsp;</th>';
    for ($i=1; $i <= $table_size; $i++) {
	  	echo "<th scope=\"col\" class=\"col\">{$i}</th>";
    }
	  echo"</tr>
		<!-- Generate the table -->
		";
		for ($i=1; $i <= $table_size; $i++) {
			echo"
			<tr><th scope=\"row\">{$i}</th>";
			for ($j=1; $j <= $table_size; $j++) {
				echo "<td>" . $i * $j. "</td>";
			}
			echo"</tr>\n";
		}
		echo'
		</table>
		<!-- END table -->
		';
}
?>
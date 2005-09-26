<?php
	$title = "CSCE 2410 - PHP Program 4";
	$section = "Assignment: Matrices";
	require("../../php-template.php");
	$rows = 5;
	$cols = 5;
		
if ($_SERVER['REQUEST_METHOD'] == "POST"){

	// Check to see which button was hit and grab the value
	//print_r($_REQUEST);
	$matrixA = array();
	$matrixB = array();
	$matrixC = array();
			
	for ($i=0; $i < $rows; $i++){
		for ($j=0; $j < $cols; $j++){
			$matrixA[$i][$j] = $_REQUEST['matrixA'][$i][$j];
			$matrixB[$i][$j] = $_REQUEST['matrixB'][$i][$j];
		}
	}

	if (isset($_POST['add'])){
		$matrixC = matrix_add($matrixA, $matrixB);
		matrix_result($matrixC);
	}
	if (isset($_POST['subtract'])){
		$matrixC = matrix_subtract($matrixA, $matrixB);
		matrix_result($matrixC);
	} 
} else {
	$matrixA = array();
	$matrixB = array();
	for ($i=0; $i < $rows; $i++){
		for ($j=0; $j < $cols; $j++){
			$matrixA[$i][$j] = $i + $j;
			$matrixB[$i][$j] = $i + $j;
		}
	}
	//print_r($matrix_array);
}

?>

<!-- BEGIN Generate the form -->
<div class="form">
	<form method="post" action="<?php echo $_SERVER['PHP_SELF']; ?>">
		<?php matrix_display("matrixA", "Matrix A:", $matrixA); ?>
		<?php matrix_display("matrixB", "Matrix B:", $matrixB); ?>
		<div style="float: left; clear: left;">
			<input type="submit" name="add" value="Add Matrix A and B" />
			<input type="submit" name="subtract" value="Subtract Matrix B from A" />
		</div>
	</form>
</div>

<?php

function matrix_display($matrix_name, $matrix_display_name, $matrix){
		$cols = count($matrix);
		$rows = $cols;
		echo "<table class=\"matrix\">\n";
		echo "<tr><th colspan=\"{$cols}\">{$matrix_display_name}</th></tr>\n";
		for ($i=0; $i < $rows; $i++){
			echo "<tr>";
			for ($j=0; $j < $cols;$j++){
				echo "<td><input type=\"text\" name=\"{$matrix_name}[{$i}][{$j}]\" size=\"6\" value=\"";
				$value = $matrix[$i][$j];
				echo $value;
				echo "\" /></td>";
			}
			echo "</tr>\n";
		}
		echo "</table>\n";
}

function matrix_add($matrixA, $matrixB){
	$matrixC = array();
	$cols = count($matrixA);
	$rows = $cols;
	for ($i=0; $i < $rows; $i++){
		for ($j=0; $j < $cols; $j++){
			$matrixC[$i][$j] = (integer)$matrixA[$i][$j] + (integer)$matrixB[$i][$j];
		}
	}
	//print_r($matrixC);
	return $matrixC;
}

function matrix_subtract($matrixA, $matrixB){
	$matrixC = array();
	$cols = count($matrixA);
	$rows = $cols;
	for ($i=0; $i < $rows; $i++){
		for ($j=0; $j < $cols; $j++){
			$matrixC[$i][$j] = (integer)$matrixA[$i][$j] - (integer)$matrixB[$i][$j];
		}
	}
	return $matrixC;
}

function matrix_result($matrix){
		$cols = count($matrix);
		$rows = $cols;
		echo "<table class=\"progtable\">\n";
		echo "<tr><th colspan=\"{$cols}\">Matrix Result:</th></tr>\n";
		for ($i=0; $i < $rows; $i++){
			echo "<tr>";
			for ($j=0; $j < $cols;$j++){
				echo "<td>";
				echo $matrix[$i][$j];
				echo "</td>";
			}
			echo "</tr>\n";
		}
		echo "</table>\n";
}

?>
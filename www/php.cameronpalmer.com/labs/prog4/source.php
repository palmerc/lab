<?php
	$title = "CSCE 2410 - PHP Source Code";
	$section = "Program 5";
	require("../../php-template.php");
	echo "<p id=\"srcode\">";
	highlight_file($root_dir."/labs/prog5/index.php");
	echo "</p>";
?>

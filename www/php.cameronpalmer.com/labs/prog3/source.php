<?php
	$title = "CSCE 2410 - PHP Source Code";
	$section = "Program 3";
	require("../../php-template.php");
	echo "<p id=\"srcode\">";
	highlight_file($root_dir."/labs/prog3/index.php");
	echo "</p>";
?>
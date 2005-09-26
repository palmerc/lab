<?php
	$title = "CSCE 2410 - PHP Source Code";
	$section = "Program 4";
	require("../../php-template.php");
	echo "<p id=\"srcode\">";
	highlight_file($root_dir."/labs/prog4/index.php");
	echo "</p>";
?>
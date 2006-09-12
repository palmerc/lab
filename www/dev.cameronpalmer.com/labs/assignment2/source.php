<?php
	$title = "CSE 4410 - PHP Source Code";
	$section = "Assignment 2";
	require("../../php-template.php");
	echo "<p id=\"srcode\">";
	highlight_file($root_dir."/labs/assignment2/index.php");
	echo "</p>";
?>
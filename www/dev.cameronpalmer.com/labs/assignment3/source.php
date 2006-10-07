<?php
	$title = "CSE 4410 - PHP Source Code";
	$section = "Assignment 3";
	require("../../dev-template.php");
	echo "<p id=\"srcode\">";
	highlight_file($root_dir."/labs/assignment3/index.php");
	echo "</p>";
?>
<?php
	$title = "CSCE 2410 - PHP Source Code";
	$section = "Final Program";
	require("../../php-template.php");
	echo "<p id=\"srcode\">";
	highlight_file($root_dir."/labs/final/index.php");
	echo "</p>";
?>

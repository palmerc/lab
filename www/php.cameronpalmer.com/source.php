<?php
	$title = "Source Code";
	$section = "Source Code";
	require("php-template.php");
	highlight_file($root_dir.$_REQUEST['filename']);
?>
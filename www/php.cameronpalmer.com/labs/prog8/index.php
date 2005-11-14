<?php
  $title = "CSCE 2410 - PHP Program 8";
  $section = "Assignment: Web Page Parsing";
  require("../../php-template.php");

	echo'
	<div class="leftside">
			';
	echo"
		<form action=\"{$_SERVER['PHP_SELF']}\" method=\"post\">
			";
	echo'
			<p>
			Enter URL: <input type="text" name="urlfile" />
			</p>
			<p>
			<input type="submit" value="Submit" />
		</form>
	</div>
			';
	echo'
		<div class="leftside">
		<hr />
			';


	if ($_SERVER['REQUEST_METHOD'] == "POST") {
		$urlfile = $_POST['urlfile'];
		$page_array = file($urlfile);
		$page_string = implode('', $page_array);
		$page_string = preg_replace('/\s\s+/', ' ', $page_string);
//		$page_string = preg_replace('/\n/', '', $page_string);
	
		echo '<pre>';
		echo strlen($page_string) . ": ";
		echo htmlspecialchars($page_string) . "\n";				
		echo '</pre>';		
	}
	
?>
<?php
	$title = "CSCE 2410 - PHP Program Final Project";
  $section = "Assignment: SQL Web Calendar";
  $alternate_css = "c/calendar.css"; 
  require("../../php-template.php");
	require(strtolower(dirname(__FILE__)).'/Calendar.php');

	if ($_GET) {
		$date = $_SERVER['argv'][0];
		$year = substr($date, 0, 4);
		$month = substr($date, 4, 2);
				
		$cal = new Calendar($month, $year);
		$today = $cal->getToday();
 		$cal->htmlCalendar();
 		
	} else {
	  $cal = new Calendar();
 		$today = $cal->getToday();
 		$cal->htmlCalendar();
	}
?>
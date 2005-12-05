<?php
	$title = "CSCE 2410 - PHP Program Final Project";
  $section = "Assignment: SQL Web Calendar";
  $alternate_css = "c/calendar.css"; 
  require("../../php-template.php");

  require(strtolower(dirname(__FILE__)).'/Calendar.php');
  $cal = new Calendar();
 	$today = $cal->getToday();
 	$cal->htmlCalendar();
?>
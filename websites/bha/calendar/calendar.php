<?
/*  ©2004 Proverbs, LLC. All rights reserved.  */

$setday = 0;
$setmonth = 0;
$setyear = 0;
$setview = "calendar";

if (isset($_REQUEST['view']) && $_REQUEST['view'] != "")
	$setview = $_REQUEST['view'];
if (isset($_REQUEST['day']) && $_REQUEST['day'] != "")
	$setday = $_REQUEST['day'];
if (isset($_REQUEST['month']) && $_REQUEST['month'] != "")
	$setmonth = $_REQUEST['month'];
if (isset($_REQUEST['year']) && $_REQUEST['year'] != "")
	$setyear = $_REQUEST['year'];

if ($setview == "schedule")
	require ('schedule.inc.php');
else
	require ('calendar.inc.php');

$page->createpage($setday, $setmonth, $setyear);

$page->displaypage();
?>
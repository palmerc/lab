<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">

<head>
<title>COBA Web Calendar</title>
	<link rel="stylesheet" type="text/css" href="main.css" />
</head>
<body>

<?php

$current_ym = mktime();	
// Display the current month if called but accept changes via the URL. So we will need GET
if ($_SERVER['argv']) {
	$requested_ym = array_pop($_SERVER['argv']);
	$data_array = query_month($requested_ym);
	display_month($reqested_ym, $data_array);
} else {
	$data_array = query_month($current_ym);
	display_month($current_ym, $data_array);
}

?>

</body>
</html>

<?php

function query_month($ym) {
	// Make database query for the specified month and return a nice pretty array of days

	$firstday = mktime(0, 0, 0, date("m"), 1, date("Y"));
	$lastday = mktime(0, 0, 0, date("m")+1, -1, date("Y"));
	echo "The first day: " . date("l, M d, Y", $firstday) . "<br />\n";
	echo "The last day: " . date("l, M d, Y", $lastday) . "<br />\n";
	$startdayofweek = date("w", $firstday);
	$lastdayofweek = date("w", $lastday);
	
	if ($startdayofweek != 7) {
		$hoursback = ($startdayofweek) * 24;
		$begincalendar = mktime(date("H")-$hoursback, 0, 0, date("m"), 1, date("Y"));
		echo "The top of the calendar: " . date("l, M d Y",$begincalendar) . "<br />";
		for ($i=0; $i < $startdayofweek; $i++) {
			$day = mktime(0, 0, 0, date("m", $begincalendar), date("d", $begincalendar)+$i, date("Y", $begincalendar));
			$data_array[] = date("d", $day);
		}
	}

	$currentcalendar = mktime(0, 0, 0, date("m"), 1, date("Y"));
	for ($i=1; $i <= date("d", $lastday); $i++) {
		$day = mktime(0, 0, 0, date("m", $currentcalendar), $i, date("Y", $currentcalendar));
		$data_array[] = date("d", $day);
	}
	
	if ($lastdayofweek != 6) {
		if ($lastdayofweek == 7) {
			$hoursforward = ($lastdayofweek - 1) * 24;
			$loopdays = $lastdayofweek - 1;
		} else { 
			$hoursforward = (6 - $lastdayofweek) * 24;
			$loopdays = 6 - $lastdayofweek;
		}
		$endcalendar = mktime(date("H")+$hoursforward, 0, 0, date("m"), date("d",$lastday), date("Y"));
		echo "The bottom of the calendar: " . date("l, M d Y",$endcalendar) . "<br />";
		for ($i=0; $i < $loopdays; $i++) {
			$day = mktime(0, 0, 0, date("m", $endcalendar), 1+$i, date("Y", $endcalendar));
			$data_array[] = date("d", $day);
		}
	}
	
	//$date_array[] = $day, $events;

		
	return $data_array;
}

function display_month($ym, $data_array) {
	//print_r($data_array);
	// Convert the $ym (year month) to a displayable title
	$title = date("F Y", $ym);
	// Create the previous and next month arrows link
	$prev_mo = date("Ym", mktime(0, 0, 0, date("m")-1, date("d"),  date("Y")));
	$next_mo = date("Ym", mktime(0, 0, 0, date("m")+1, date("d"),  date("Y")));
	echo'
	<table cellspacing="0" width="100%" id="calendar">
		<tr id="title">
			';
	echo"
			<th id=\"lastmonth\"><a href=\"{$prev_mo}\">&laquo;</a></th>
			<th colspan=\"5\" id=\"thismonth\">{$title}</th>
			<th id=\"nextmonth\"><a href=\"{$next_mo}\">&raquo;</a></th>
			";
	echo'
		</tr>
		<!-- CALENDAR HEADINGS -->
		<tr id="days">
			<th class="sun">Sun</th>
			<th class="mon">Mon</th>
			<th class="tue">Tue</th>
			<th class="wed">Wed</th>
			<th class="thu">Thu</th>
			<th class="fri">Fri</th>
			<th class="sat">Sat</th>
		</tr>
			';

	$calendardays = 0;
	$num_weeks = 6;
	for ($i=0; $i < $num_weeks; $i++) {
		echo'<tr id="firstweek">';
		for ($j=1; $j <= 7; $j++) {
			$day = $data_array[$calendardays];
			echo"
			<td class=\"jun sun\" id=\"jun30\">
				<div class=\"date\">{$day}</div>
			</td>
					";
			$calendardays++;
		}
		echo'</tr>';
	}
	echo'
	</table>
			';
} // END display_month()
?>
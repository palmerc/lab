<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"><html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">

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
	return array();
}

function display_month($ym, $data_array) {

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

	$num_weeks = 5;
	for ($i=0; $i < $num_weeks; $i++) {
		echo'<tr id="firstweek">';
		for ($j=1; $j <= 7; $j++) {
			echo"
			<td class=\"jun sun\" id=\"jun30\">
				<div class=\"date\">{$j}</div>
			</td>
					";
		}
		echo'</tr>';
	}
	echo'
	</table>
			';
} // END display_month()
?>
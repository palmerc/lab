<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<?php

// Database connection variables
// Be sure to set these to your own connection
$HOST = 'localhost';
$USER = 'stc';
$PASS = 'stcdbpw';
$NAME = 'stc';

// Connect to MySQL database
$con = mysql_connect($HOST, $USER, $PASS);
if (!$con) {
  // Since the entire script depends on connection, die if connection fails
  die("Error connecting to MySQL database!");
}
mysql_select_db($NAME, $con);

function getEventDays($month, $year) {
  $days = array();
  $sql = mysql_query("SELECT DAY(event_date) AS day, COUNT(event_id) FROM calendar_events WHERE MONTH(event_date) = '$month' AND YEAR(event_date) = '$year' GROUP BY day");

  if (mysql_num_rows($sql) > 0) {
  while ($row = mysql_fetch_array($sql)) $days[] = $row['day'];
  }

  return $days;
}



//Old code**************

error_reporting('0');
ini_set('display_errors', '0');
// Gather variables from
// user input and break them
// down for usage in our script

if(!isset($_REQUEST['date'])){
   $date = mktime(0,0,0,date('m'), date('d'), date('Y'));
} else {
   $date = $_REQUEST['date'];
}

$day = date('d', $date);
$month = date('m', $date);
$year = date('Y', $date);

// Get the first day of the month
$month_start = mktime(0,0,0,$month, 1, $year);

// Get friendly month name
$month_name = date('M', $month_start);

// Figure out which day of the week
// the month starts on.
$month_start_day = date('D', $month_start);

switch($month_start_day){
    case "Sun": $offset = 0; break;
    case "Mon": $offset = 1; break;
    case "Tue": $offset = 2; break;
    case "Wed": $offset = 3; break;
    case "Thu": $offset = 4; break;
    case "Fri": $offset = 5; break;
    case "Sat": $offset = 6; break;
}
 $eventDays   = getEventDays($month, $year);

// determine how many days are in the last month.
if($month == 1){
   $num_days_last = cal_days_in_month(0, 12, ($year -1));
} else {
   $num_days_last = cal_days_in_month(0, ($month -1), $year);
}
// determine how many days are in the current month.
$num_days_current = cal_days_in_month(0, $month, $year);

// Build an array for the current days
// in the month
for($i = 1; $i <= $num_days_current; $i++){
    $num_days_array[] = $i;
}

// Build an array for the number of days
// in last month
for($i = 1; $i <= $num_days_last; $i++){
    $num_days_last_array[] = $i;
}

// If the $offset from the starting day of the
// week happens to be Sunday, $offset would be 0,
// so don't need an offset correction.

if($offset > 0){
    $offset_correction = array_slice($num_days_last_array, -$offset, $offset);
    $new_count = array_merge($offset_correction, $num_days_array);
    $offset_count = count($offset_correction);
}

// The else statement is to prevent building the $offset array.
else {
    $offset_count = 0;
    $new_count = $num_days_array;
}

// count how many days we have with the two
// previous arrays merged together
$current_num = count($new_count);

// Since we will have 5 HTML table rows (TR)
// with 7 table data entries (TD)
// we need to fill in 35 TDs
// so, we will have to figure out
// how many days to appened to the end
// of the final array to make it 35 days.


if($current_num > 35){
   $num_weeks = 6;
   $outset = (42 - $current_num);
} elseif($current_num < 35){
   $num_weeks = 5;
   $outset = (35 - $current_num);
}
if($current_num == 35){
   $num_weeks = 5;
   $outset = 0;
}
// Outset Correction
for($i = 1; $i <= $outset; $i++){
   $new_count[] = $i;
}

// Now let's "chunk" the $all_days array
// into weeks. Each week has 7 days
// so we will array_chunk it into 7 days.
$weeks = array_chunk($new_count, 7);


// Build Previous and Next Links
$previous_link = "<a href=\"".$_SERVER['PHP_SELF']."?date=";
if($month == 1){
   $previous_link .= mktime(0,0,0,12,$day,($year -1));
} else {
   $previous_link .= mktime(0,0,0,($month -1),$day,$year);
}

$previous_link .= "\"><< </a>";

$next_link = "<a href=\"".$_SERVER['PHP_SELF']."?date=";
if($month == 12){
   $next_link .= mktime(0,0,0,1,$day,($year + 1));
} else {
   $next_link .= mktime(0,0,0,($month +1),$day,$year);
}
$next_link .= "\"> >></a>";


echo "<table border=\"1\" cellpadding=\"2\" cellspacing=\"0\" width=\"100%\" class=\"calendar\">\n".
     "<tr class=\"monthheader\">\n".
     "<th colspan=\"7\">\n".
     "<table align=\"center\">\n".
     "<tr>\n".
     "<th colspan=\"2\" width=\"75\" align=\"left\">$previous_link</td>\n".
     "<th colspan=\"3\" width=\"150\" align=\"center\">$month_name $year</td>\n".
     "<th colspan=\"2\" width=\"75\" align=\"right\">$next_link</td>\n".
     "</tr>\n".
     "</table>\n".
     "</th>\n".
     "<tr class=\"daysheader\">\n".
     "<th>Sun</th><th>Mon</th><th>Tue</th><th>Wed</th><th>Thu</th><th>Fri</th><th>Sat</th>\n".
     "</tr>\n";
	 

	 

// Now we break each key of the array 
// into a week and create a new table row for each
// week with the days of that week in the table data

$i = 0;
foreach($weeks AS $week){
       echo "<tr>\n";
       foreach($week as $d){
         if($i < $offset_count){
             $day_link = "<a href=\"".$_SERVER['PHP_SELF']."?date=".mktime(0,0,0,$month -1,$d,$year)."\">$d</a>";
             echo "<td class=\"nonmonthdays\">$day_link</td>\n";
         }
         if(($i >= $offset_count) && ($i < ($num_weeks * 7) - $outset)){
            $day_link = "<a href=\"".$_SERVER['PHP_SELF']."?date=".mktime(0,0,0,$month,$d,$year)."\">$d</a>";
           if($date == mktime(0,0,0,$month,$d,$year)){
               echo "<td class=\"today\">$d</td>\n";
           } else {
               echo "<td class=\"days\">$day_link</td>\n";
           }
        } elseif(($outset > 0)) {
            if(($i >= ($num_weeks * 7) - $outset)){
               $day_link = "<a href=\"".$_SERVER['PHP_SELF']."?date=".mktime(0,0,0,$month +1,$d,$year)."\">$d</a>";
               echo "<td class=\"nonmonthdays\">$day_link</td>\n";
           }
        }
        $i++;
      }
      echo "</tr>\n";   
}

// Close out your table and that's it!
echo '<tr><td colspan="7" class="days"> </td></tr>';
echo '</table>';
?>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1" />
<title>PHP Calendar</title>
<style type="text/css">
table.calendar {
	font-family: Arial, Helvetica, sans-serif;
	background-color: #FFF;
}
.monthheader, .monthheader a {
	font-family: Geneva, Arial, Helvetica, sans-serif;
	color: #333;
	background-color: #FBE762;
	text-decoration: none;
	}	
.daysheader {
	background: #333;
	border-color: #333;
	color: #FFFFFF;
	}
.days a {
	color: #333333; 
	text-decoration: none;
	border-width: 0 0 1px 1px;
	border-style: solid;
	border-color: #888;
	float: right;
	padding: .125em .25em 0 .25em;
	background-color:#EEE;
	}
.nonmonthdays {
	background: #EEE;
	}
.nonmonthdays a {
	text-decoration: none;
	color: #BBB;
	float: right;
	padding: .125em .25em 0 .25em;
	}
td {
	vertical-align: top;
	padding: 0;
	border: 0px solid gray;
	border-width: 0 0 1px 1px;
	height: 4em;
	width: 7em;
	}
.today {
	background-color: #CBBDDF;
	text-align: right;
	padding: .125em .25em 0 0;
	border-width: 0 0 1px 1px;
	border-style: solid;
	border-color: #888;
	}
.td "Sun" {
	background-color: #CBBDDF;
	}

html > body tr#days th.sat {
	border-right: 1px solid #000;
}
.table.calendar .tr.days .th {
	color: #fff; 
	background-color: #333;
	font-weight: bold; 
	text-align: center;
	/*padding: 1px 0.33em;*/
}
table#calendar tr#caltitle th {
	text-align: center;
	background: #FBE762; 
	color: #333;
	border: 1px solid;
	border-right: #000;
	border-bottom: #FBE762;
	border-left: #000;
	padding: .3em 0 .2em 0;
	font-family:Geneva, Arial, Helvetica, sans-serif; 
	font-size: 120%;
}
table#calendar tr#caltitle th#lastmonth a {
	text-align: center;
	color: #333;
	border-left: #000;
}
table#calendar tr#caltitle th#nextmonth a {
	text-align: center;
	color: #333;
	border-right: #000;
}
table#calendar td {
	
}
table#calendar td.sat {
	border-right: 1px solid gray;
}
table#calendar td.sat, table#calendar td.sun {
	background: #EEE;
}
table#calendar a {
	font-weight: bold;
	display: block;
	margin: 0;
}
table#calendar a:link {
	color: #fff;
}
table#calendar a:visited {
	color: #fff;
}
table#calendar a:hover {
	color: #000;
}

table#calendar tr#lastweek td {
	border-bottom: 2px solid #AAB;
}
table#calendar td.holiday {
	background: #BBF;
}
div.event {
	color: #000;	
	margin: 0.5em;
	font-size: 0.70em;
}
table#calendar div.event a {
	color: #000;
	font-weight: normal;
	border-bottom-style: dotted;
	border-bottom-width: thin;
}
div.event span {
	display: block;
}
div.holiday {
	font-style: italic;
}
span.time {
	font-weight: bold;
}
span.loc {
	color: #555;
	font-style: italic;
}
div.date {
	font-family: Geneva, Arial, Helvetica, sans-serif;
	font-size: 12px;
	float: right;
	text-align: center;
	border: 1px solid gray;
	border-width: 0 0 1px 1px;
	padding: 0.125em 0.25em 0 0.25em;
	margin: 0; 
	background: #F3F3F3;
}
div.date today {
	background: #CCBDDF;
}
</style>
</head>

<body>

</body>
</html>

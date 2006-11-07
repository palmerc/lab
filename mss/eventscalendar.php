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

function drawCalendar($month, $year) {
  // set variables we will need to help with layouts
  $first       = mktime(0,0,0,$month,1,$year); // timestamp for first of the month
  $offset      = date('w', $first); // what day of the week we start counting on
  $daysInMonth = date('t', $first);
  $monthName   = date('F', $first);
  $weekDays    = array('Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat');
  $eventDays   = getEventDays($month, $year);
  
  // Start drawing calendar
  $out  = "<table id=\"myCalendar\">\n";
  $out .= "<tr><th colspan=\"3\">$monthName $year</th></tr>\n";
  $out .= "<tr>";
  foreach ($weekDays as $wd) $out .= "<td class=\"weekDays\">$wd</td>\n";


   // Previous month link
   $prevTS = strtotime("$year-$month-01 -1 month"); // timestamp of the first of last month
   $pMax = date('t', $prevTS);
   $pDay = ($day > $pMax) ? $pMax : $day;
   list($y, $m) = explode('-', date('Y-m', $prevTS)); 
   $pMax = date('t', $prevTS);
   $pDay = ($day > $pMax) ? $pMax : $day;
   list($y, $m) = explode('-', date('Y-m', $prevTS));
   echo "<p>";
   echo "<a href=\"?year=$year&month=$m&day=$pDay\"><< Prev</a>\n";

   // Next month link
   $nextTS = strtotime("$year-$month-01 +1 month");
   $nMax = date('t', $nextTS);
   $nDay = ($day > $nMax) ? $nMax : $day;
   list($y, $m) = explode('-', date('Y-m', $nextTS));
   echo "<a href=\"?year=$y&month=$m&day=$nDay\">Next >></a>\n";
   echo "</p>";


   // Build Previous and Next Links from untitled-6
   //$previous_link = "<a href=\"".$_SERVER['PHP_SELF']."?date=";
   //if($month == 1){
   //   $previous_link .= mktime(0,0,0,12,$day,($year -1));
   //} else {
    //  $previous_link .= mktime(0,0,0,($month -1),$day,$year);
   //}

   //$previous_link .= "\"><< </a>";

   //$next_link = "<a href=\"".$_SERVER['PHP_SELF']."?date=";
   //if($month == 12){
     // $next_link .= mktime(0,0,0,1,$day,($year + 1));
   //} else {
     // $next_link .= mktime(0,0,0,($month +1),$day,$year);
   //}
   //$next_link .= "\"> >></a>";

     $i = 0;
     for ($d = (1 - $offset); $d <= $daysInMonth; $d++) {
       if ($i % 7 == 0) $out .= "<tr>\n"; // Start new row
       if ($d < 1) $out .= "<td class=\"nonMonthDay\"> </td>\n";
       else {
         if (in_array($d, $eventDays)) {
           $out .= "<td class=\"monthDay\">\n";
           $out .= "<a href=\"?year=$year&month=$month&day=$d\">$d</a>\n";
           $out .= "</td>\n";
         } else $out .= "<td class=\"monthDay\">$d</td>\n";
       }
       ++$i; // Increment position counter
       if ($i % 7 == 0) $out .= "</tr>\n"; // End row on the 7th day
     }
     
     // Round out last row if we don't have a full week
     if ($i % 7 != 0) {
       for ($j = 0; $j < (7 - ($i % 7)); $j++) {
         $out .= "<td class=\"nonMonthDay\"> </td>\n";
       }
       $out .= "</tr>\n";
     }
     
     $out .= "</table>\n";
       
       return $out;
   } //END function drawCalendar

$year  = isset($_GET['year']) ? $_GET['year'] : date('Y');
$month = isset($_GET['month']) ? $_GET['month'] : date('m');

echo drawCalendar($month, $year);

// Calendar is done, let's list events for selected day
$sql = mysql_query("SELECT * FROM calendar_events WHERE event_date = '$year-$month-$day'");
if (mysql_num_rows($sql) > 0) {
  while ($e = mysql_fetch_array($sql)) {
    echo "<p>$e[event_title]</p>\n";
  }
} else {
  echo "<p>No events today</p>\n";
} 
?>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1" />
<title>Lone Star Community</title>
<style type="text/css">
#myCalendar {
	font-family: Arial, Helvetica, sans-serif;
	background-color: #FFF;
   text-align: right;
}
a, th {
	font-family: Geneva, Arial, Helvetica, sans-serif;
	color: #333;
	background-color: #FBE762;
	text-decoration: none;
   text-align: center;
	}	
.weekDays {
	background: #333;
	border-color: #333;
	color: #FFFFFF;
   text-align: center;
   padding-top: 1.5em;
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
.nonMonthDay {
	background: #EEE;
	}
.nonMonthDay a {
	text-decoration: none;
	color: #BBB;
	float: right;
	padding: .125em .25em 0 .25em;
	}
.monthDay {
   padding: .125em .25em 0 .25em;
   border-width: 0 0 1px 1px;
	border-style: solid;
	border-color: #888;
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
</style>
</head>

<body>
</body>
</html>

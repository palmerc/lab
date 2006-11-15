<?php
require('database.php');
require('user.php');
session_start();
if (!$_SESSION['email']) 
   header('location:login.php');
$user_id = $_SESSION['email'];
$first = $_SESSION['first_name'];

database_connect();
$result = retrieve_user($user_id);
$admin = $result[0]['admin'];
database_disconnect();

$title = "Lone Star Community - Calendar";
$alternate_css = "c/calendar.css";
require('stc-template.php');

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

database_connect();
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
database_disconnect();
?>
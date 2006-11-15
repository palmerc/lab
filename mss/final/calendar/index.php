<?php
require('../database.php');
require('../user.php');

session_start();
if (!$_SESSION['email']) 
   header('location:login.php');
$user_id = $_SESSION['email'];
$first = $_SESSION['first_name'];

database_connect();
$result = retrieve_user($user_id);
$admin = $result[0]['admin'];

$title = "Lone Star Community - Calendar";
$alternate_css = "{$ptr}c/calendar.css";
$calendar = 1;
require(strtolower(dirname(__FILE__)).'/Calendar.php');

function getEvents($Ymd) {
    $sql = "SELECT * FROM tbl_news WHERE calstart <='{$Ymd}' AND calend >='{$Ymd}' AND status='Publish' AND calshow='1'";
    $r = mysql_query($sql) or die("Can't update record {$longday}<br />".mysql_error());

    while($p = mysql_fetch_array($r)) {
        $events[] = array("$Ymd" => array($p['title'], $p['body'], mklink($p) ) );
    }
    print_r($events);
}

if ($_GET) {
   require("../stc-template.php");
    $date = $_SERVER['QUERY_STRING'];
    $year = substr($date, 0, 4);
    $month = substr($date, 4, 2);
            
    $cal = new Calendar($month, $year);
    $today = $cal->getToday();
    $events = getEvents($cal->year.$cal->month.$cal->day);
    $cal->htmlCalendar();
    
} else {
    $today = date("Ym");
    // Write the current month to the URL so that it is always obvious how to
    // change the month in the command-line. Otherwise it would only appear when
    // people changed months using the arrow buttons.
    header("Location: http://" . $_SERVER['HTTP_HOST'] 
        . rtrim(dirname($_SERVER['PHP_SELF']), '/\\')
        . "/" 
        . $today);
}
database_disconnect();
?>
<?php
$settings['pagetype'] = "i t";
$settings['title'] = "UNT/College of Business/News/Calendar";
$settings['extrasheets'] = array("calendar.css"); //or false
require(strtolower(dirname(__FILE__)).'/Calendar.php');
require(strtolower(dirname(__FILE__)).'/../../common/common.php');
$db_name="cobadb";
include("../../dbconnect/cobaweb.php");
require(strtolower($_SERVER["DOCUMENT_ROOT"])."/news/_functions.inc");

function getEvents($daysArray) {
    foreach ($daysArray as $longday) {
        $year = substr($longday, 0, 4);
        $month = substr($longday, 4, 2);
        $day = substr($longday, 6, 2);
        $sqlday = $year."-".$month."-".$day;
        print_r($sqlday);
        $sql = "SELECT * FROM tbl_news WHERE calstart <='{$sqlday}' AND calend >='{$sqlday}' AND status='Publish' AND calshow='1'";
        $r = mysql_query($sql) or die("Can't fetch record {$sqlday}<br />".mysql_error());
    
        while($p = mysql_fetch_array($r)) {
            $events[] = array("$Ymd" => array($p['title'], $p['body'], mklink($p) ) );
        }
    }
    print_r($events);
}

if ($_GET) {
    $date = $_SERVER['QUERY_STRING'];
    $year = substr($date, 0, 4);
    $month = substr($date, 4, 2);
            
    $cal = new Calendar($month, $year);
    $today = $cal->getToday();
    $events = getEvents($cal->daysArray);
    $cal->htmlCalendar();
    
} else {
    $current_month = date("Ym");
    // Write the current month to the URL so that it is always obvious how to
    // change the month in the command-line. Otherwise it would only appear when
    // people changed months using the arrow buttons.
    header("Location: http://" . $_SERVER['HTTP_HOST'] 
        . rtrim(dirname($_SERVER['PHP_SELF']), '/\\')
        . "/" 
        . $current_month);
}
?>
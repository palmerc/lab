<?php
	$title = "CSCE 2410 - PHP Program Final Project";
    $section = "Assignment: SQL Web Calendar";
    $alternate_css = "c/calendar.css"; 
    require("../../php-template.php");
    require(strtolower(dirname(__FILE__)).'/Calendar.php');
    $db_name="cobadb";
    include("../../../../../../dbconnect/cobaweb.php");
    require(strtolower($_SERVER["DOCUMENT_ROOT"])."/news/_functions.inc");

function getEvents($Ymd) {
    $sql = "SELECT * FROM tbl_news WHERE calstart <='{$Ymd}' AND calend >='{$Ymd}' AND status='Publish' AND calshow='1'";
    $r = mysql_query($sql) or die("Can't update record {$longday}<br />".mysql_error());

    while($p = mysql_fetch_array($r)) {
        $events[] = array("$Ymd" => array($p['title'], $p['body'], mklink($p) ) );
    }
    print_r($events);
}

if ($_GET) {
    $date = $_SERVER['QUERY_STRING'];
    $year = substr($date, 0, 4);
    $month = substr($date, 4, 2);
            
    $cal = new Calendar($month, $year);
    $today = $cal->getToday();
    $events = getEvents($this->year.$this->month.$this->day);
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
?>
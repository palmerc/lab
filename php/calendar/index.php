<?php
$settings['pagetype'] = "i";
$settings['title'] = "UNT/College of Business/News/Calendar";
$settings['extrasheets'] = array("main.css"); //or false
require(strtolower(dirname(__FILE__)).'/../../common/common.php');
include("../../_generalinfo.inc");
//include("../../about/_aboutside.inc");
$db_name="cobadb";
include("../../dbconnect/cobaweb.php");

// Display the current month if called but accept changes via the URL. So we will need GET
if ($_GET) {
    list($requested_ym,) = each($_GET);
	$data_array = query_month($requested_ym);
	display_month($requested_ym, $data_array);
} else {
    $current_ym = date("Ym", mktime());
    // Write the current month to the URL so that it is always obvious how to
    // change the month in the command-line. Otherwise it would only appear when
    // people changed months using the arrow buttons.
    header("Location: http://" . $_SERVER['HTTP_HOST'] 
        . rtrim(dirname($_SERVER['PHP_SELF']), '/\\')
        . "/" 
        . $current_ym);
}

function query_month($ym) {
	// Make database query for the specified month and return a nice pretty array of days
    $year = substr($ym, 0, 4);
    $month = substr($ym, 4, 2);
    //echo "{$month} {$year}<br />\n";
    
    // Grab the first day and last day of the month
	$firstday = mktime(0, 0, 0, $month, 1, $year);
	$lastday = mktime(0, 0, 0, $month+1, 0, $year);
	//echo "The first day: " . date("l, M d, Y", $firstday) . "<br />\n";
	//echo "The last day: " . date("l, M d, Y", $lastday) . "<br />\n";
    
    // What day of the week is the first and last day of the month?
    // 1 to 7 AKA Monday to Sunday
    $startdayofweek = date("w", $firstday);
	$enddayofweek = date("w", $lastday);
    $lastdayofmonth = date("d", $lastday);
	
    // We have a square calendar so we need to show the end of the previous month
    // if the first isn't on a Sunday
	if ($startdayofweek != 7) {
        // Zero is the last day of the previous month as far as mktime is concerned
		//echo "The top of the calendar: " . date("l, M d Y",$begincalendar) . "<br />";
		for ($i=0; $i < $startdayofweek; $i++) {                
			$data_array[] = mktime(0, 0, 0, $month, -($startdayofweek-1)+$i, $year);
		}
	}

    // The month we are interested in is handled here.
	for ($i=1; $i <= $lastdayofmonth; $i++) {
        $data_array[] = mktime(0, 0, 0, $month, $i, $year);
	}
	
    // We have a square calendar so we need to show the beginning of the next month
    // if the last day isn't on a Saturday
	if ($enddayofweek != 6) {
		if ($enddayofweek == 7) {
			$loopdays = $enddayofweek - 1;
		} else { 
			$loopdays = 6 - $enddayofweek;
		}
		//$endcalendar = mktime(0, 0, 0, $month, $lastdayofmonth+$daysforward, $year);
		//echo "The bottom of the calendar: " . date("l, M d Y",$endcalendar) . "<br />";
		for ($i=1; $i <= $loopdays; $i++) {
			$data_array[] = mktime(0, 0, 0, $month, $lastdayofmonth+$i, $year);
		}
	}
		
	return $data_array;
}

function display_month($ym, $data_array) {
    //echo "{$ym}<br />\n";
    //echo '<pre>';
	//print_r($data_array);
    //echo '</pre>';
	// Convert the $ym (year month) to a displayable title
    $year = substr($ym, 0, 4);
    $month = substr($ym, 4, 2);
    //echo "{$month} {$year}<br />\n";
    $month_name = date("F", mktime(0, 0, 0, $month, 1, $year));
	
	// Create the previous and next month arrows link
	$prev_mo = date("Ym", mktime(0, 0, 0, $month-1, 1,  $year));
	$next_mo = date("Ym", mktime(0, 0, 0, $month+1, 1,  $year));
	echo'
	<table cellspacing="0" width="100%" id="calendar">
		<tr id="title">
			';
	echo"
			<th id=\"lastmonth\"><a href=\"{$prev_mo}\">&laquo;</a></th>
			<th colspan=\"5\" id=\"thismonth\">{$month_name} {$year}</th>
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

	$i = 0;
    $week = 0;
    $nameofweek = array("firstweek", "secondweek", "thirdweek", "fourthweek", "fifthweek", "sixthweek");
	while ($i < count($data_array)) {
        if (count($data_array) - $i <= 7) {
            $thisweek = "lastweek";
        } else {
            $thisweek = $nameofweek[$week];
        }
		echo"
        <tr id=\"{$thisweek}\">
        ";
		for ($j=0; $j < 7; $j++) {
			$day = date("M D d", $data_array[$i]);
            $longday = date("Y-m-d", $data_array[$i]);
            $shortmonth = strtolower(substr($day, 0 , 3));
            $shortday = strtolower(substr($day, 4, 3));
            $date = strtolower(substr($day, 8, 2));
            $sql = "SELECT * FROM tbl_news WHERE calstart='{$longday}'";
            $r = mysql_query($sql) or die("Can't update record {$longday}<br />".mysql_error());
            $p = mysql_fetch_array($r);
            list($news_title, $news_body) = array($p['title'], $p['body']);
             
            
            //Print the calendar day.
			echo"
			<td class=\"{$shortmonth} {$shortday}\" id=\"{$shortmonth}{$date}\">
                <div class=\"date\">{$date}</div>
                <div class=\"event\">";
            if ($news_title) {
                echo "<a href=\"?\">{$news_title}</a>";
            } 
            echo"
            </div>
			</td>
                ";
			$i++;
            
		}
        $week++;
		echo'
        </tr>
        ';
	}
	echo'
	</table>
			';
} // END display_month()
?>
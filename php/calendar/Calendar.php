<?php
class Calendar {
    var $month, $year, $today, $dayHeadings;

    // Constructor for the Calendar class
    function Calendar($month, $year)
    {
        $this->setToday();
        $this->setDayHeadings(5);
        if (!$month) { $this->setMonth(date("m")); }
        else { $this->setMonth($month); }
        if (!$year) { $this->setYear(date("Y")); }
        else { $this->setYear($year); }
    }
    
    function setMonth($month)
    {
        $this->month = $month;
    }
    
    function setYear($year)
    {
        $this->year = $year;
    }

    function setToday()
    {
        $this->today = date("Ymd");
    }
    
    function setDayHeadings($startingDay=0)
    {
        // zero is sunday, six is saturday
        $defaultDays = array('Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat');
     
        if ($startingDay != 0) {
            for ($index=$startingDay; count($newArrangement) < 7; $index++) {
                if ($index < 7) {
                    $newArrangement[] = $defaultDays[$index];
                } else {
                    $index = 0;
                    $newArrangement[] = $defaultDays[$index];
                }
            }
            
            $this->dayHeadings = $newArrangement;
            return;
        } 
        
        $this->dayHeadings = $defaultDays;
        return;
    }
    
    function getDayHeadings()
    {
        return $this->dayHeadings;
    }

    // getTodayDate
    function getToday()
    {
        return $this->today;
    }
   
    // getWeekNumeric
    function getWeekNumeric()
    {
        return date("W");
    }
   
   
    // Function to split the YMD format into an array
    function splitYmd($Ymd) {
        $year = substr($Ymd, 0, 4);
        $month = substr($Ymd, 4, 2);
        $day = substr($Ymd, 6, 2);
        return array($year, $month, $day);
    }
   
    // getDayofWeek
    function getShortDayofWeek($Ymd)
    {
        list($year, $month, $day) = $this->splitYmd($Ymd);
        return date("D", mktime(0, 0, 0, $month, $day, $year));
    }
    
    function getLongDayofWeek($Ymd)
    {
        list($year, $month, $day) = $this->splitYmd($Ymd);
        return date("l", mktime(0, 0, 0, $month, $day, $year));
    }
    
    function getDaysinMonth($Ymd)
    {
        list($year, $month, $day) = $this->splitYmd($Ymd);
        return date("t", mktime(0, 0, 0, $year, $month, $day));
    }
    
    // getPrevMonth
    function getPrevMonth()
    {
        return date("Ym", mktime(0, 0, 0, $this->month-1, 1,  $this->year));
    }
    
    // getNextMonth
    function getNextMonth()
    {
        return date("Ym", mktime(0, 0, 0, $this->month+1, 1,  $this->year));
    }
    
    function getLongMonthName()
    {
        return date("F", mktime(0, 0, 0, $this->month, 1, $this->year));
    }
    
    function htmlCalendar()
    {
        //
        // Start the calendar table display
        echo'
        <table style="float: left;" cellspacing="0" width="100%" id="calendar">
            ';
        
        //
        // Calendar navigation bar
        echo'
            <tr id="caltitle">
                <th id="lastmonth"><a href="';
            echo $this->getPrevMonth();
            echo '">&laquo;</a></th><th colspan="5" id="thismonth">';
            echo $this->getLongMonthName();
            echo " {$this->year}</th>";
            echo '<th id="nextmonth"><a href="';
            echo $this->getNextMonth();
            echo '">&raquo;</a></th>
            </tr>
            ';
       
        //
        // Create day of the week headings
        echo'
            <tr id="days">
            ';
        $dayHeadings = $this->getDayHeadings();
        foreach ($dayHeadings as $day) {
            echo'
                <th class="';
                    echo strtolower($day);
                    echo "\">{$day}</th>
                ";
        }
        echo'
            </tr>
            ';
        // End day of the week headings
       
       
        $week = 0;
        
        //$nameofweek = array("firstweek", "secondweek", "thirdweek", "fourthweek", "fifthweek", "sixthweek");
        while ($week < count($data_array)) {
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
                //$sql = "SELECT * FROM tbl_news WHERE calstart <='{$longday}' AND calend >='{$longday}' AND status='Publish' AND calshow='1'";
                //$r = mysql_query($sql) or die("Can't update record {$longday}<br />".mysql_error());
                
                //Print the calendar day.
                echo"
                <td class=\"{$shortmonth} {$shortday}\" id=\"{$shortmonth}{$date}\">
                    <div class=\"date\">{$date}</div>
                    <div class=\"event\">";
                //while($p = mysql_fetch_array($r)) {
                    //echo "<pre>";
                    //print_r($p);
                    //echo "</pre>";
                //    list($news_title, $news_body, $news_link) = array($p['title'], $p['body'], mklink($p));
                 
                
                //    if ($news_title) {
                //        echo "<a href=\"{$news_link}\">{$news_title}</a>";
                //    } 
                //}
                echo"
                </div>
                </td>
                    ";
                //$i++;
                
            }
            $week++;
            echo'
            </tr>
            ';
        }
        echo'
        </table>
        <br style="clear: left;" />
        <br />
            ';
    } // END display_month()

} // END class Calendar

    $cal = new Calendar();
    $today = $cal->getToday();
    echo $cal->getLongDayofWeek($today);
    $cal->htmlCalendar();
    
?>
<?php
class Calendar {
    var $month, $year, $today, $dayHeadings, $numofweeks, $daysArray;

    // Constructor for the Calendar class
    // constructor arguments are $month, $year
    function Calendar()
    {
    	$this->setToday();
        $this->setDayHeadings(0);
    	if (func_num_args() == 2) {
    		$this->setMonth(func_get_arg(0));
    		$this->setYear(func_get_arg(1));
    	} else { 
    		$this->setMonth(date("m"));
    		$this->setYear(date("Y"));
    	}
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
    
    function getDayofWeekNumeric($Ymd)
    {
        list($year, $month, $day) = $this->splitYmd($Ymd);
        return date("w", mktime(0, 0, 0, $month, $day, $year));
    }
    
    function getDaysinMonth()
    {
        return date("t", mktime(0, 0, 0, $this->year, $this->month, 1));
    }
    
    function getWeeksinMonth()
    {
        return ceil($this->getDaysinMonth() / 7);
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
       
        $firstday = $this->getDayofWeekNumeric($this->year.$this->month.'01');
        
        $week = 0;
        $dayindex = 1;
        while ($week < $this->getWeeksinMonth()) {
            $thisweek = $week + 1;
            echo"
            <tr id=\"week{$thisweek}\">
            ";
            for ($j=0; $j < 7; $j++) {
                $epochtime = mktime(0, 0, 0, $this->month, $dayindex-$firstday, $this->year);
                $day = date("M D d", $epochtime);
                $longday = date("Ymd", $epochtime);
                $shortmonth = strtolower(substr($day, 0 , 3));
                $shortday = strtolower(substr($day, 4, 3));
                $date = strtolower(substr($day, 8, 2));
                $this->daysArray[] = $longday;
                
                if ($this->today == $longday) {
                    //Print the calendar day.
                    echo <<< DAY
                <td class="{$shortmonth} {$shortday}" id="{$shortmonth}{$date}">
                    <div class="date" id="today">{$date}</div>
                    <div class="event"></div>
                </td>
DAY;
                } else {
                    echo <<< DAY
                <td class="{$shortmonth} {$shortday}" id="{$shortmonth}{$date}">
                    <div class="date">{$date}</div>
                    <div class="event"></div>
                </td>
DAY;
                }
                $dayindex++;            
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
    
?>
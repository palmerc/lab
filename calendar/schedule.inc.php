<?
/*  ©2004 Proverbs, LLC. All rights reserved.  */

if (eregi("schedule.inc.php", $_SERVER['PHP_SELF']))
{
	// redirect to calendar page
	header("Location: calendar.php");
	exit;
}

if(!defined("SCHEDULE_PAGE")) 
{ 
	define("SCHEDULE_PAGE", TRUE); 

	require ('base.inc.php');

	class schedule_page extends base_calendar
	{
		// Constructor
		function schedule_page()
		{
			$this->base_calendar();
		}

		function createpage($current_day, $current_month, $current_year)
		{
			// create the setup object
			$conf = new layout_setup;
			$adjustment = 3600 * $conf->time_adjustment;
			if ($conf->use_dst)
				$adjustment += 3600 * gmdate("I", time() + $adjustment);
			if ($current_year > (gmdate("Y", time() + $adjustment) + 10))
				$current_year = gmdate("Y", time() + $adjustment) + 10;
			if ($current_year < (gmdate("Y", time() + $adjustment) - 5))
				$current_year = gmdate("Y", time() + $adjustment) - 5;

			if ($current_year < 1980) 
				$current_year = 1980;

			if (!checkdate($current_month, $current_day, $current_year))
			{
				$current_day = gmdate("j", time() + $adjustment);
				$current_month = gmdate("n", time() + $adjustment);
			}

			$browser_version = $this->makeheader();

			if ($browser_version > 0)
			{
				if ($browser_version == 1)
				{
					$this->pagelayout .= '<link rel="stylesheet" type="text/css" href="navstylesheet.css.php" />
';
					$this->closeheader();
					// require netscape schedule
					require ('schedule.nav.php');
					$altschedule = new nav_schedule_page;
					$this->pagelayout .= $altschedule->createpage($current_day, $current_month, $current_year);
					return;	
				}
				if ($browser_version == 2)
				{
					$scrolltime = 0;
					if (isset($conf->schedule_scroll_time) && $conf->schedule_scroll_time >= 0)
						$scrolltime = (intval($conf->schedule_scroll_time) + 1) * 60;
					$this->pagelayout .= '<link rel="stylesheet" type="text/css" href="iestylesheet.css.php" />
<script type="text/javascript" src="schedjavascript.js"></script>
</head>
<body leftmargin="0" topmargin="0" marginheight="0" marginwidth="0" onload="document.all.eventlist.scrollTop = '.$scrolltime.';">
';
				}
				else
					$this->closeheader();
			}
			else
			{
				$this->closeheader();
				// require alternate schedule
				require ('schedule.alt.php');
				$altschedule = new alt_schedule_page;
				$this->pagelayout .= $altschedule->createpage($current_day, $current_month, $current_year);
				return;					
			}

			$this->activeday = $current_day;
			$this->activemonth = $current_month;
			$this->activeyear = $current_year;

			$this->pagelayout .= '<center>
<form method="post" name="schedulePage" action="calendar.php">
<table cellspacing="0" cellpadding="0" border="0">
<tr>
';

			$this->makeschedule($current_day, $current_month, $current_year);

			$this->makeminicals($current_day, $current_month, $current_year);

			$this->pagelayout .= '</tr>
</table>
</form>
<a class="calmenu" style="font-size: 10px" href="http://www.proverbs.biz" target="_blank">Web Calendar &copy;2004 Proverbs, LLC. All rights reserved.</a>
</center>';
		}

		function makeschedule($current_day, $current_month, $current_year)
		{
			// create the language object
			$lang = new language;
			// create the setup object
			$conf = new layout_setup;

			$twelvehour = false;
			if ($conf->time_format == '12')
				$twelvehour = true;

			$disp_zone = ' '.$conf->time_zone.' ';

			unset($conf);

			$dayofweek = gmdate("w", gmmktime(0, 0, 0, $current_month, $current_day, $current_year));

			$this->pagelayout .= '<td style="padding: 0px; text-align: right">
<table class="schedule" style="width: 100%" cellspacing="0" cellpadding="0" border="0">
<tr>
<td class="schedtop" onclick="showCalendar('.$current_month.', '.$current_year.')">
<b>'.$lang->day_long[$dayofweek].', '.$lang->month_long[$current_month].' '.$current_day.', '.$current_year.'</b>
<input type="hidden" name="view" value="schedule" />
<input type="hidden" name="year" value="'.$current_year.'" />
<input type="hidden" name="month" value="'.$current_month.'" />
<input type="hidden" name="day" value="'.$current_day.'" />
</td>
</tr>
</table>
<div style="height: 508px" id="eventlist">
<table class="schedule" cellspacing="0" cellpadding="0" border="0">
<tr>
<td class="schedtime" rowspan="4">
<b>'.$lang->word_all_day.'</b> -&nbsp;
</td>
<td class="schedone">
&nbsp;
</td>
</tr>
';
			for ($rep = 0; $rep < 3; $rep++)
			{
				$this->pagelayout .= '<tr>
<td class="schedone">
&nbsp;
</td>
</tr>
';	
			}

			for ($i = 0; $i < 24; $i++)
			{
				$display_time = $i;
				if ($twelvehour)
				{
					if ($i == 0)
						$display_time = '12 am'.$disp_zone;
					else
					{
						if ($i < 12)
							$display_time .= ' am'.$disp_zone;
						else
						{
							if ($i == 12)
								$display_time .= ' pm'.$disp_zone;
							else
								$display_time = ($i - 12).' pm'.$disp_zone;
						}
					}
				}
				$this->pagelayout .= '<tr>
<td class="schedtime" rowspan="4">
<b>'.$display_time.'</b> -&nbsp;
</td>
<td class="schedtwo">&nbsp;</td>
</tr>
<tr>
<td class="schedone">&nbsp;</td>
</tr>
<tr>
<td class="schedtwo">&nbsp;</td>
</tr>
<tr>
<td class="schedone">&nbsp;</td>
</tr>
';
			}
			$this->pagelayout .= '</table>
';
			$this->fillschedule($current_day, $current_month, $current_year);

			$this->pagelayout.= '</div>
</td>
';
			unset($lang);
		}

		function fillschedule($current_day, $current_month, $current_year)
		{	
			$blockholder = Array(0 => 0, "left" => 84, "right" => 84);
			$timeblocks = Array("ALL" => $blockholder, "00:00" => $blockholder, "00:15" => $blockholder, "00:30" => $blockholder, "00:45" => $blockholder, "01:00" => $blockholder,
				"01:15" => $blockholder, "01:30" => $blockholder, "01:45" => $blockholder, "02:00" => $blockholder, "02:15" => $blockholder, "02:30" => $blockholder, "02:45" => $blockholder, "03:00" => $blockholder, 
				"03:15" => $blockholder, "03:30" => $blockholder, "03:45" => $blockholder, "04:00" => $blockholder, "04:15" => $blockholder, "04:30" => $blockholder, "04:45" => $blockholder, "05:00" => $blockholder, 
				"05:15" => $blockholder, "05:30" => $blockholder, "05:45" => $blockholder, "06:00" => $blockholder, "06:15" => $blockholder, "06:30" => $blockholder, "06:45" => $blockholder, "07:00" => $blockholder, 
				"07:15" => $blockholder, "07:30" => $blockholder, "07:45" => $blockholder, "08:00" => $blockholder, "08:15" => $blockholder, "08:30" => $blockholder, "08:45" => $blockholder, "09:00" => $blockholder, 
				"09:15" => $blockholder, "09:30" => $blockholder, "09:45" => $blockholder, "10:00" => $blockholder, "10:15" => $blockholder, "10:30" => $blockholder, "10:45" => $blockholder, "11:00" => $blockholder, 
				"11:15" => $blockholder, "11:30" => $blockholder, "11:45" => $blockholder, "12:00" => $blockholder, "12:15" => $blockholder, "12:30" => $blockholder, "12:45" => $blockholder, "13:00" => $blockholder, 
				"13:15" => $blockholder, "13:30" => $blockholder, "13:45" => $blockholder, "14:00" => $blockholder, "14:15" => $blockholder, "14:30" => $blockholder, "14:45" => $blockholder, "15:00" => $blockholder, 
				"15:15" => $blockholder, "15:30" => $blockholder, "15:45" => $blockholder, "16:00" => $blockholder, "16:15" => $blockholder, "16:30" => $blockholder, "16:45" => $blockholder, "17:00" => $blockholder, 
				"17:15" => $blockholder, "17:30" => $blockholder, "17:45" => $blockholder, "18:00" => $blockholder, "18:15" => $blockholder, "18:30" => $blockholder, "18:45" => $blockholder, "19:00" => $blockholder, 
				"19:15" => $blockholder, "19:30" => $blockholder, "19:45" => $blockholder, "20:00" => $blockholder, "20:15" => $blockholder, "20:30" => $blockholder, "20:45" => $blockholder, "21:00" => $blockholder, 
				"21:15" => $blockholder, "21:30" => $blockholder, "21:45" => $blockholder, "22:00" => $blockholder, "22:15" => $blockholder, "22:30" => $blockholder, "22:45" => $blockholder, "23:00" => $blockholder, 
				"23:15" => $blockholder, "23:30" => $blockholder, "23:45" => $blockholder, "24:00" => $blockholder);

			$single_event = Array("start_time" => " ", "duration" => 0, "short_event" => " ", "long_event" => " ", "block_cells" => 0);
			$schedule_events = Array();

			$calaccess = new caldbaccess;

			$conf = new layout_setup;

			$twelvehour = false;
			if ($conf->time_format == '12')
				$twelvehour = true;
			unset($conf);

			$itemcount = $calaccess->GetScheduleDateEvents($current_day, $current_month, $current_year);
			for ($i = 0; $i < $itemcount; $i++)
			{
				$calaccess->next_record();
				$single_event['short_event'] = $calaccess->f('short_description');
				$single_event['long_event'] = '<b>'.$calaccess->f('short_description').'</b><br />';
				if (gmdate("H:i:s", $calaccess->f('event_date')) != '00:00:25')
				{
					if ($twelvehour)
						$single_event['long_event'] .= gmdate("g:i a", $calaccess->f('event_date')).' - '.gmdate("g:i a", intval($calaccess->f('event_date') + (60 * $calaccess->f('duration') * 60))).'<br />';
					else
						$single_event['long_event'] .= gmdate("G:i", $calaccess->f('event_date')).' - '.gmdate("G:i", intval($calaccess->f('event_date') + (60 * $calaccess->f('duration') * 60))).'<br />';
				}

				$single_event['long_event'] .= str_replace('
', '<br />', $calaccess->f('long_description'));
				$single_event['duration'] = $calaccess->f('duration');

				if (gmdate("H:i:s", $calaccess->f('event_date')) == '00:00:25')
				{
					$timeblocks["ALL"][0]++;
					$single_event['start_time'] = 'ALL';
				}
				else
				{
					$single_event['start_time'] = gmdate("H:i", $calaccess->f('event_date'));
					$adding_time = floatval($single_event['duration'] * 60);
					for ($s = 0; $s < $adding_time; $s = $s + 15)
					{
						$holdtime = gmdate("H:i", (intval($calaccess->f('event_date')) + (60 * $s)));
						if ($single_event['start_time'] != "00:00" && $holdtime == "00:00")
							$holdtime = "24:00";
						$timeblocks[$holdtime][0]++;
					}
				}
				$schedule_events[] = $single_event;
			}

			$itemcount = $calaccess->GetScheduleDayEvents($current_day, $current_month, $current_year);
			for ($i = 0; $i < $itemcount; $i++)
			{
				$calaccess->next_record();
				$single_event['short_event'] = $calaccess->f('short_description');
				$breakup = explode(':', $calaccess->f('event_time'));

				$single_event['long_event'] = '<b>'.$calaccess->f('short_description').'</b><br />';
				if ($calaccess->f('event_time') != '00:00:25')
				{
					if ($twelvehour)
						$single_event['long_event'] .= gmdate("g:i a", gmmktime($breakup[0], $breakup[1])).' - '.gmdate("g:i a", intval(gmmktime($breakup[0], $breakup[1]) + (60 * $calaccess->f('duration') * 60))).'<br />';
					else
						$single_event['long_event'] .= gmdate("G:i", gmmktime($breakup[0], $breakup[1])).' - '.gmdate("G:i", intval(gmmktime($breakup[0], $breakup[1]) + (60 * $calaccess->f('duration') * 60))).'<br />';
				}
				$single_event['long_event'] .= str_replace('
', '<br />', $calaccess->f('long_description'));
				$single_event['duration'] = $calaccess->f('duration');

				if ($calaccess->f('event_time') == '00:00:25')
				{
					$timeblocks["ALL"][0]++;
					$single_event['start_time'] = 'ALL';
				}
				else
				{
					$single_event['start_time'] = $breakup[0].':'.$breakup[1];
					$adding_time = intval($single_event['duration'] * 60);
					for ($s = 0; $s < $adding_time; $s = $s + 15)
					{
						$holdtime = gmdate("H:i", (gmmktime($breakup[0], $breakup[1]) + (60 * $s)));
						if ($single_event['start_time'] != "00:00" && $holdtime == "00:00")
							$holdtime = "24:00";
						$timeblocks[$holdtime][0]++;
					}
				}
				$schedule_events[] = $single_event;
			}

			unset($calaccess);

			$blockheight = 0;
			$blockwidth = 0;

			$event_display = '';

			for ($i = 0; $i < count($schedule_events); $i++)
			{
				if ($schedule_events[$i]['start_time'] == 'ALL')
				{
					$schedule_events[$i]['block_cells'] = $timeblocks['ALL'][0];
					$blockheight = 60;
					if ($timeblocks['ALL'][0] > 0)
						$blockwidth = intval(410 / $timeblocks['ALL'][0]);
					else
						$blockwidth = 410;
					if (($timeblocks['ALL']['left'] + $blockwidth) >= ($timeblocks['ALL']['right'] - $schedule_events[$i]['block_cells']))
						$blockleft = $timeblocks['ALL']['right'];
					else
						$blockleft = $timeblocks['ALL']['left'];
					if ($blockleft > $timeblocks['ALL']['left'])
					{
						$timeblocks['ALL']['right'] = $blockleft + $blockwidth;
						for ($r = 1; $r <= $timeblocks['ALL'][0]; $r++)
						{
							$right_side = $blockleft * $r;
							if ($timeblocks[$holdtime]['right'] > $right_side && $timeblocks['ALL']['right'] < ($right_side + $blockwidth))
								$timeblocks['ALL']['right'] = $right_side + $blockwidth;
						}
					}
					else
					{
						if ($timeblocks['ALL']['left'] == $timeblocks['ALL']['right'])
							$timeblocks['ALL']['right'] = $blockleft + $blockwidth;
						$timeblocks['ALL']['left'] = $blockleft + $blockwidth;
					}
					$event_display = $schedule_events[$i]['long_event'];
					$this->pagelayout .= '<div class="event" style="top: 1px; left: '.$blockleft.'px; height: '.$blockheight.'px; width: '.$blockwidth.'px" onclick="setEventDetails(\''.$event_display.'\')">
'.$schedule_events[$i]['short_event'].'
</div>
';
				}
				else
				{
					$breakup = explode(':', $schedule_events[$i]['start_time']);
					$adding_time = intval($schedule_events[$i]['duration'] * 60);
					for ($s = 0; $s < $adding_time; $s = $s + 15)
					{
						$holdtime = gmdate("H:i", (gmmktime($breakup[0], $breakup[1]) + (60 * $s)));
						if ($timeblocks[$holdtime][0] > $schedule_events[$i]['block_cells'])
							$schedule_events[$i]['block_cells'] = $timeblocks[$holdtime][0];
					}
				}
			}

			$timecount = count($timeblocks) - 2;
			$item_count = count($schedule_events);
			for ($i = 0; $i < $timecount; $i++)
			{
				$on_time = gmdate("H:i", (gmmktime(0, 0) + ($i * 15 * 60)));
				if ($timeblocks[$on_time][0] == 0)
					continue;
				for ($j = 0; $j < $item_count; $j++)
				{
					if ($schedule_events[$j]['start_time'] == $on_time)
					{
						$blockheight = 60 * $schedule_events[$j]['duration'];
						if ($schedule_events[$j]['block_cells'] > 0)
							$blockwidth = intval(410 / $schedule_events[$j]['block_cells']);
						else
							$blockwidth = 410;
						$blocktop = 61 + ($i * 15);
						if (($timeblocks[$on_time]['left'] + $blockwidth) >= ($timeblocks[$on_time]['right'] - $schedule_events[$j]['block_cells']))
							$blockleft = $timeblocks[$on_time]['right'];
						else
							$blockleft = $timeblocks[$on_time]['left'];
						$adding_time = intval($schedule_events[$j]['duration'] * 60);
						for ($s = 0; $s < $adding_time; $s = $s + 15)
						{
							$holdtime = gmdate("H:i", (gmmktime(0, 0) + ($i * 15 * 60) + (60 * $s)));
							if ($blockleft > $timeblocks[$holdtime]['left'])
							{
								$timeblocks[$holdtime]['right'] = $blockleft + $blockwidth;
								for ($r = 1; $r <= $timeblocks[$holdtime][0]; $r++)
								{
									$right_side = $blockleft * $r;
									if ($timeblocks[$holdtime]['right'] > $right_side && $timeblocks[$holdtime]['right'] < ($right_side + $blockwidth))
										$timeblocks[$holdtime]['right'] = $right_side + $blockwidth;
								}
							}
							else
							{
								if ($timeblocks[$holdtime]['left'] == $timeblocks[$holdtime]['right'])
									$timeblocks[$holdtime]['right'] = $blockleft + $blockwidth;
								$timeblocks[$holdtime]['left'] = $blockleft + $blockwidth;
							}
						}
						$event_display = $schedule_events[$j]['long_event'];
						$this->pagelayout .= '<div class="event" style="top: '.$blocktop.'px; left: '.$blockleft.'px; height: '.$blockheight.'px; width: '.$blockwidth.'px" onclick="setEventDetails(\''.$event_display.'\')">
'.$schedule_events[$j]['short_event'].'
</div>
';
					}
				}				
			}

			unset ($blockholder);
		}

		function makeminicals($current_day, $current_month, $current_year)
		{
			// create the language object
			$lang = new language;
			$this->pagelayout .= '<td style="padding: 0px">
<table cellspacing="0" cellpadding="0" border="0">
<tr>
<td style="padding: 0px; text-align: center">
<table class="schedule" style="width: 308px" cellspacing="0" cellpadding="0" border="0">
';

			if ($current_year == (gmdate("Y") + 10) && $current_month > 10)
			{
				if ($current_month == 11)
					$onmonth = $current_month - 2;
				else
					$onmonth = $current_month - 3;
			}
			else
			{
				if ($current_year == (gmdate("Y") - 2) && $current_month < 2)
					$onmonth = $current_month;
				else
					$onmonth = $current_month - 1;
			}
			$onyear = $current_year;
			if ($onmonth < 1)
			{
				$onmonth = $onmonth + 12;
				$onyear = $onyear - 1;
			}

			$calaccess = new caldbaccess;

			for ($i = 0; $i < 2; $i++)
			{
				$this->pagelayout .= '<tr>
';
				for ($a = 0; $a < 2; $a++)
				{
					$this->pagelayout .= '<td style="padding: 0px">
<table class="minical" cellspacing="0" cellpadding="0" border="1">
<tr>
<td class="';
					if ($onyear == $current_year && $onmonth == $current_month)
						$this->pagelayout .= 'aminical';
					else
						$this->pagelayout .= 'minical';
					$this->pagelayout .= '" style="width: 154px; max-width: 154px; min-width: 154px; text-align: center; column-span: 7" nowrap colspan="7" onclick="showCalendar('.$onmonth.', '.$onyear.')">
'.$lang->month_long[$onmonth].' '.$onyear.'
</td>
</tr>
<tr>
';
					for ($d = 0; $d < 7; $d++)
					{
						$this->pagelayout .= '<td class="minical" style="text-align: center; cursor: default">
'.$lang->day_init[$d].'
</td>
';	
					}
					$this->pagelayout .= '</tr>
';

					$lastday = 28;
					for ($k = $lastday; $k < 32; $k++)
					{
						if (checkdate($onmonth, $k, $onyear))
							$lastday = $k;
					}

					$calday = 1;
					$rowheight = 0;
					$firstweekday = gmdate("w", gmmktime(0, 0, 0, $onmonth, 1, $onyear));
					for ($r = 0; $r < 6; $r++)
					{
						$this->pagelayout .= '<tr>
';
						for ($j = 0; $j < 7; $j++)
						{
							if (($calday == 1 && $firstweekday == $j) || ($calday > 1 && $calday <= $lastday))
							{
								if ($onyear == $current_year && $onmonth == $current_month && $calday == $current_day)
									$this->pagelayout .= '<td class="aminical" ';
								else
									$this->pagelayout .= '<td class="minical" ';
								$this->pagelayout .= 'onclick="showSchedule('.$calday.', '.$onmonth.', '.$onyear.')">
';
								if ($calaccess->CheckForEvents($calday, $onmonth, $onyear))
									$this->pagelayout .= '<b>'.$calday.'</b>';
								else
									$this->pagelayout .= $calday;
								$this->pagelayout .= '&nbsp;
</td>
';
								$calday++;
							}
							else
								$this->pagelayout .= '<td class="minical" style="cursor: default">
&nbsp;
</td>
';
						}
						$this->pagelayout .= '</tr>
';
					}

					$this->pagelayout .= '</table>
</td>
';

					$onmonth++;
					if ($onmonth > 12)
					{
						$onmonth = 1;
						$onyear++;	
					}
				}

				$this->pagelayout .= '</tr>
';
			}
			unset($caldb);

			$this->pagelayout .= '</table>
</td>
</tr>
<tr>
<td style="padding: 0px; text-align: left">
<div style="height: 246px; width: 314px">
<table style="width: 100%; height: 246px" cellspacing="0" cellpadding="0" border="0">
<tr>
<td id="showEvent" class="event">
&nbsp;
</td>
</tr>
</table>
</div>
</td>
</tr>
</table>
';


			$this->pagelayout .= '</td>
';
			unset($lang);
		}
	}
}
// Create the schedule object.
$page = new schedule_page;
?>
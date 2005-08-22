<?
/*  ©2004 Proverbs, LLC. All rights reserved.  */

if (eregi("calendar.inc.php", $_SERVER['PHP_SELF']))
{
	// redirect to calendar page
	header("Location: calendar.php");
	exit;
}

if(!defined("CALENDAR_PAGE")) 
{ 
	define("CALENDAR_PAGE", TRUE); 

	require ('base.inc.php');

	class calendar_page extends base_calendar
	{
		// Constructor
		function calendar_page()
		{
			$this->base_calendar();
		}

		function createpage($current_day, $current_month, $current_year)
		{
			// create the setup object
			$conf = new layout_setup;

			// create the language object
			$lang = new language;

			$adjustment = 3600 * $conf->time_adjustment;
			if ($conf->use_dst)
				$adjustment += 3600 * gmdate("I", time() + $adjustment);
			if ($current_month == "" || $current_month < 1 || $current_month > 12)
				$current_month = gmdate("n", time() + $adjustment);
			if ($current_year == "" || $current_year == 0)
				$current_year = gmdate("Y", time() + $adjustment);
			if ($current_year > (gmdate("Y", time() + $adjustment) + 10))
				$current_year = gmdate("Y", time() + $adjustment) + 10;
			if ($current_year < (gmdate("Y", time() + $adjustment) - 5))
				$current_year = gmdate("Y", time() + $adjustment) - 5;
			if ($current_year < 1980)
				$current_year = 1980;

			$browser_version = $this->makeheader();

			if ($browser_version > 0)
			{
				if ($browser_version == 1)
					$this->pagelayout .= '<link rel="stylesheet" type="text/css" href="navstylesheet.css.php" />
';
				if ($browser_version == 2)
					$this->pagelayout .= '<link rel="stylesheet" type="text/css" href="iestylesheet.css.php" />
';

				$this->pagelayout .= '<script type="text/javascript" src="caljavascript.js.php"></script>
<noscript><center><b>'.$lang->word_no_javascript.'</b></center></noscript>
';
				$this->closeheader();
			}
			else
			{
				$this->closeheader();
				unset($conf);
				unset($lang);
				// require alternate calendar
				require ('calendar.alt.php');
				$altcalendar = new alt_calendar_page;
				$this->pagelayout .= $altcalendar->createpage($current_day, $current_month, $current_year);
				return;
			}

			$this->activeday = $current_day;
			$this->activemonth = $current_month;
			$this->activeyear = $current_year;

			$nextmonth = $current_month + 1;
			$nextyear = $current_year;
			$prevmonth = $current_month - 1;
			$prevyear = $current_year;

			if ($current_month == 1)
			{
				$prevmonth = 12;
				$prevyear = $current_year - 1;
			}
			if ($current_month == 12)
			{
				$nextmonth = 1;
				$nextyear = $current_year + 1;	
			}

			$this->pagelayout .= '<center>
<form method="post" name="calendarPage" action="calendar.php">
<table class="calendar" cellspacing="0" cellpadding="0" border="0">
<tr>
<td class="calmenu" style="width: 100%; text-align: center" colspan="7">
<table cellspacing="0" cellpadding="0" border="0">
<tr>
<td nowrap class="calmenu" style="width: 250px">
&nbsp;<b>'.$lang->word_today_date.': </b>'.$lang->month_long[gmdate("n", time() + $adjustment)].gmdate(" j, Y", time() + $adjustment).'
</td>
<td class="calmenu" style="width: 460px; text-align: right">
<b>'.$lang->word_month.': </b>
<select name="month">
';
			for ($i = 1; $i <= 12; $i++)
			{
				$this->pagelayout .= '<option value="'.$i.'"';
				if ($i == $this->activemonth)
					$this->pagelayout .= ' selected';
				$this->pagelayout .= '>'.$lang->month_long[$i].'</option>
';
			}

			$this->pagelayout .= '</select>
&nbsp;&nbsp;
<b>'.$lang->word_year.': </b>
<select name="year">
';
			for ($i = (gmdate("Y") - 2); $i <= (gmdate("Y") + 10); $i++)
			{
				$this->pagelayout .= '<option value="'.$i.'"';
				if ($i == $this->activeyear)
					$this->pagelayout .= ' selected';
				$this->pagelayout .= '>'.$i.'</option>
';
			}

			$this->pagelayout .= '</select>&nbsp;&nbsp;
<input type="button" style="width: 80px" name="refresh" value="'.$lang->word_refresh.'" onclick="ensureRefresh();" />&nbsp;&nbsp;&nbsp;
<br />
';
			if ($conf->show_admin_link)
				$this->pagelayout .= '<a class="calmenu" href="caladmin/caladmin.php" target="caladmin"><u>'.$lang->word_administration.'</u></a>
';
			$this->pagelayout .= '</td>
</tr>
</table>
</td>
</tr>
<tr>
<td class="calhead" colspan="7">
<table class="calhead" cellspacing="0" cellpadding="0" border="0">
<tr>
<td class="caltitle" colspan="3">
<input type="hidden" name="day" value="" />
<input type="hidden" name="view" value="calendar" />
';
			if (trim($conf->calendar_title_image) != '')
				$this->pagelayout .= '<img src="'.$conf->calendar_title_image.'" alt="'.$conf->calendar_title_text.'" />';
			else
				$this->pagelayout .= $conf->calendar_title_text;

			$this->pagelayout .= '
</td>
</tr>
<tr>
<td class="calmonths" style="text-align: left">
&nbsp;';
			if ($prevyear > 1979 && $prevyear > (gmdate("Y", time()) - 6))
				$this->pagelayout .= '<a class="calmonths" href="calendar.php?month='.$prevmonth.'&amp;year='.$prevyear.'"><b>&lt;&lt; '.$lang->month_long[$prevmonth].' '.$prevyear.'</b></a>';

			$this->pagelayout .= '
</td>
<td class="calmonths" style="width: 300px; font-size: 16px">
<b>'.$lang->month_long[$current_month].' '.$current_year.'</b>
</td>
<td class="calmonths" style="text-align: right">
';
			if ($nextyear < (gmdate("Y", time()) + 11))
				$this->pagelayout .= '<a class="calmonths" href="calendar.php?month='.$nextmonth.'&amp;year='.$nextyear.'"><b>'.$lang->month_long[$nextmonth].' '.$nextyear.' &gt;&gt;</b></a>';
			else
				$this->pagelayout .= '&nbsp;';

			$this->pagelayout .= '
</td>
</tr>
</table>
</td>
</tr>
<tr>
';
		   if (isset($conf->calendar_start_day) && $conf->calendar_start_day <= 6 && $conf->calendar_start_day >= 0)
		      $n = $conf->calendar_start_day;
		   else
		      $n = 0;

		   for ($i = 0; $i < 7; $i++)
		   {
		      if ($n > 6)
		         $n = 0;

	         $this->pagelayout .= '<td class="calweekday" nowrap>
<b>'.$lang->day_long[$n].'</b>
</td>
';
		      $n++;
		   }
			$this->pagelayout .= '</tr>
';

			$this->makecalendar($current_month, $current_year, $conf->calendar_start_day);

			$this->pagelayout .= '</table>
</form>
<a class="calmenu" style="font-size: 10px" href="http://www.proverbs.biz" target="_blank">Web Calendar &copy;2004 Proverbs, LLC. All rights reserved.</a>
</center>';
			unset($conf);
			unset($lang);
		}

		function makecalendar($current_month, $current_year, $startweekday)
		{
			$firstweekday = gmdate("w", gmmktime(0, 0, 0, $current_month, 1, $current_year));

			$lastday = 28;

			for ($i = $lastday; $i < 32; $i++)
			{
				if (checkdate($current_month, $i, $current_year))
					$lastday = $i;
			}

			$todayday = 0;

			$conf = new layout_setup;
			$adjustment = 3600 * $conf->time_adjustment;
			if ($conf->use_dst)
				$adjustment += 3600 * date("I", time() + $adjustment);

			if ($current_month == gmdate("n", time() + $adjustment) && $current_year == gmdate("Y", time() + $adjustment))
				$todayday = gmdate("j", time() + $adjustment);

			$calday = 1;
			$calaccess = new caldbaccess;
			while ($calday <= $lastday)
			{
				$this->pagelayout .= '<tr>
';
				for ($j = 0; $j < 7; $j++)
				{
					if ($j == 0)
						$n = $startweekday;
					else
					{
						if ($n < 6)
							$n = $n + 1;
						else
							$n = 0;
					}
					if (($calday == 1 && $firstweekday == $n) || ($calday > 1 && $calday <= $lastday))
					{
						$linesoftext = Array();
						$itemcount = $calaccess->GetCalendarDateEvents($calday, $current_month, $current_year);
						for ($x = 0; $x < $itemcount; $x++)
						{
							$calaccess->next_record();
							$linesoftext[] = $calaccess->f('short_description');
						}

						$itemcount = $calaccess->GetCalendarDayEvents($calday, $current_month, $current_year);
						for ($x = 0; $x < $itemcount; $x++)
						{
							$calaccess->next_record();
							$linesoftext[] = $calaccess->f('short_description');
						}

						if ($todayday > 0 && $todayday == $calday)
							$this->addcalday($calday, true, $linesoftext);
						else
							$this->addcalday($calday, false, $linesoftext);
						$calday++;
					}
					else
						$this->pagelayout .= '<td class="calday" style="cursor: default">
&nbsp;
</td>
';
				}
				unset ($caldb);
				$this->pagelayout .= '</tr>
';
			}
			unset($conf);
		}

		function addcalday($curday, $istoday, $textdisplay)
		{
			// create the setup object
			$tempconf = new layout_setup;

			if ($istoday)
				$this->pagelayout .= '<td nowrap class="calday" style="background-color: #'.$tempconf->calendar_today_background_color.'" onmouseover="tdmv(this)" onmouseout="tdtmo(this)" onclick="sendDay('.$curday.', '.$this->activemonth.', '.$this->activeyear.')">
'.$curday.'<br />
';
			else	
				$this->pagelayout .= '<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay('.$curday.', '.$this->activemonth.', '.$this->activeyear.')">
'.$curday.'<br />
';

			unset($tempconf);

			// create the language object
			$templang = new language;

			if ($textdisplay != "" && count($textdisplay) > 0)
			{
				for ($i = 0; $i < count($textdisplay); $i++)
				{
					$displine = '';
					if (strlen(trim($textdisplay[$i])) > 21)
						$displine = trim(substr(trim($textdisplay[$i]), 0, 18)).'...';
					else
						$displine = trim($textdisplay[$i]);
					$displine .= '<br />
';
					if ($i < 4)
						$this->pagelayout .= $displine;
					if ($i == 5)
						$this->pagelayout .= '<center>&lt;&lt; '.$templang->word_more.' &gt;&gt;</center>
';
				}
			}

			unset($templang);

			$this->pagelayout .= '</td>
';
		}
	}
}
// Create the calendar object.
$page = new calendar_page;
?>
<?
/*  ©2004 Proverbs, LLC. All rights reserved.  */

if (eregi("calendar.alt.php", $_SERVER['PHP_SELF']))
{
	// redirect to calendar page
	header("Location: calendar.php");
	exit;
}

if(!defined("ALTERNATE_CALENDAR_PAGE")) 
{ 
	define("ALTERNATE_CALENDAR_PAGE", TRUE); 

	require ('setup.inc.php');
	require ('language.inc.php');
	require ('calaccess.inc.php');

	class alt_calendar_page
	{
		var $alternate_page;
		var $activemonth;
		var $activeyear;

		// Constructor
		function alt_calendar_page()
		{
			$this->alternate_page = '';
			$this->activemonth = 0;
			$this->activeyear = 0;
		}

		function createpage($current_day, $current_month, $current_year)
		{
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

			// create the setup object
			$conf = new layout_setup;
			// create the language object
			$lang = new language;

			$adjustment = 3600 * $conf->time_adjustment;
			if ($conf->use_dst)
				$adjustment += 3600 * date("I", time() + $adjustment);

			$this->alternate_page .= '<center>
<form method="post" action="calendar.php">
<table bordercolor="#'.$conf->calendar_border_color.'" cellspacing="0" cellpadding="0" border="1">
<tr>
<td bordercolor="#'.$conf->html_body_background_color.'" width="100%" height="40" align="center" valign="middle" bgcolor="#'.$conf->html_body_background_color.'" colspan="7">
<table cellspacing="0" cellpadding="0" border="0">
<tr>
<td nowrap align="left" valign="middle" bgcolor="#'.$conf->html_body_background_color.'" width="250">
&nbsp;<font size="2" point-size="12" color="#'.$conf->html_body_text_color.'"><b>'.$lang->word_today_date.': </b>'.$lang->month_long[gmdate("n", time() + $adjustment)].gmdate(" j, Y", time() + $adjustment).'</font>
</td>
<td nowrap align="right" valign="middle" bgcolor="#'.$conf->html_body_background_color.'" width="450">
<font size="2" point-size="12" color="#'.$conf->html_body_text_color.'"><b>'.$lang->word_month.': </b>
<select name="month">
';
			for ($i = 1; $i <= 12; $i++)
			{
				$this->alternate_page .= '<option value="'.$i.'"';
				if ($i == $current_month)
					$this->alternate_page .= ' selected';
				$this->alternate_page .= '>'.$lang->month_long[$i].'</option>
';
			}

			$this->alternate_page .= '</select>
&nbsp;&nbsp;
<b>'.$lang->word_year.': </b>
<select name="year">
';
			for ($i = (gmdate("Y") - 2); $i <= (gmdate("Y") + 10); $i++)
			{
				$this->alternate_page .= '<option value="'.$i.'"';
				if ($i == $current_year)
					$this->alternate_page .= ' selected';
				$this->alternate_page .= '>'.$i.'</option>
';
			}

			$this->alternate_page .= '</select>&nbsp;&nbsp;
<input type="submit" name="refresh" value="'.$lang->word_refresh.'" />&nbsp;&nbsp;&nbsp;</font>
<br />
';
			if ($conf->show_admin_link)
				$this->alternate_page .= '<a href="caladmin/caladmin.php" target="caladmin"><font size="2" point-size="12" color="#'.$conf->html_body_text_color.'">'.$lang->word_administration.'</font></a>
';
			$this->alternate_page .= '
</td>
</tr>
</table>
</td>
</tr>
<tr>
<td width="100%" align="center" valign="middle" bgcolor="#'.$conf->calendar_title_background_color.'" colspan="7">
<table width="100%" cellspacing="0" cellpadding="0" border="0">
<tr>
<td width="100%" height="60" align="center" valign="middle" bgcolor="#'.$conf->calendar_title_background_color.'" colspan="3">
';
			if (trim($conf->calendar_title_image) != '')
				$this->alternate_page .= '<img src="'.$conf->calendar_title_image.'" alt="'.$conf->calendar_title_text.'" border="0" />';
			else
				$this->alternate_page .= '<font size="6" point-size="36" color="#'.$conf->calendar_title_text_color.'">'.$conf->calendar_title_text.'</font>';

			$this->alternate_page .= '
</td>
</tr>
<tr>
<td nowrap width="200" height="24" bgcolor="#'.$conf->calendar_title_background_color.'" valign="middle" align="left">
&nbsp;';
			if ($prevyear > 1979 && $prevyear > (gmdate("Y", time()) - 6))
				$this->alternate_page .= '<a href="calendar.php?month='.$prevmonth.'&amp;year='.$prevyear.'"><font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'"><b>&lt;&lt; '.$lang->month_long[$prevmonth].' '.$prevyear.'</b></font></a>';

			$this->alternate_page .= '
</td>
<td nowrap width="300" height="24" bgcolor="#'.$conf->calendar_title_background_color.'" valign="middle" align="center">
<font size="3" point-size="16" color="#'.$conf->calendar_title_text_color.'"><b>'.$lang->month_long[$current_month].' '.$current_year.'</b></font>
</td>
<td nowrap width="200" height="24" bgcolor="#'.$conf->calendar_title_background_color.'" valign="middle" align="right">
';
			if ($nextyear < (gmdate("Y", time()) + 11))
				$this->alternate_page .= '<a href="calendar.php?month='.$nextmonth.'&amp;year='.$nextyear.'"><font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'"><b>'.$lang->month_long[$nextmonth].' '.$nextyear.' &gt;&gt;</b></font></a>';
			else
				$this->alternate_page .= '&nbsp;';

			$this->alternate_page .= '
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

	         $this->alternate_page .= '<td nowrap width="100" height="24" align="center" valign="middle" bgcolor="#'.$conf->calendar_day_background_color.'">
<font size="2" point-size="11" color="#'.$conf->calendar_day_text_color.'"><b>'.$lang->day_long[$n].'</b></font>
</td>
';
		      $n++;
		   }
			$this->alternate_page .= '</tr>
';

			$this->makecalendar($current_month, $current_year, $conf->calendar_start_day);

			$this->alternate_page .= '</table>
</form>
<a href="http://www.proverbs.biz" target="_blank"><font size="1" point-size="10" color="#'.$conf->html_body_text_color.'">Web Calendar &copy;2004 Proverbs, LLC. All rights reserved.</font></a>
</center>';
			return $this->alternate_page;
		}

		function makecalendar($current_month, $current_year, $startweekday)
		{
			// create the setup object
			$tempconf = new layout_setup;

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
				$this->alternate_page .= '<tr>
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
						$this->alternate_page .= '<td width="100" height="85" bgcolor="#'.$tempconf->calendar_day_background_color.'">
&nbsp;
</td>
';
				}
				unset ($caldb);
				$this->alternate_page .= '</tr>
';
			}
		}

		function addcalday($curday, $istoday, $textdisplay)
		{
			// create the setup object
			$tempconf = new layout_setup;

			if ($istoday)
				$this->alternate_page .= '<td align="left" valign="top" width="100" height="85" bgcolor="#'.$tempconf->calendar_today_background_color.'">
';
			else	
				$this->alternate_page .= '<td align="left" valign="top" width="100" height="85" bgcolor="#'.$tempconf->calendar_day_background_color.'">
';

			$this->alternate_page .= '<a href="calendar.php?view=schedule&amp;day='.$curday.'&amp;month='.$this->activemonth.'&amp;year='.$this->activeyear.'">
<font size="1" point-size="9" color="#'.$tempconf->calendar_day_text_color.'">'.$curday.'<br />
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
						$this->alternate_page .= $displine;
					if ($i == 5)
						$this->alternate_page .= '<center>&lt;&lt; '.$templang->word_more.' &gt;&gt;</center>
';
				}
			}

			unset($templang);

			$this->alternate_page .= '</font></a>
</td>
';
		}
	}
}
?>
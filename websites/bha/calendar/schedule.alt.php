<?
/*  ©2004 Proverbs, LLC. All rights reserved.  */

if (eregi("schedule.alt.php", $_SERVER['PHP_SELF']))
{
	// redirect to calendar page
	header("Location: calendar.php");
	exit;
}

if(!defined("ALTERNATE_SCHEDULE_PAGE")) 
{ 
	define("ALTERNATE_SCHEDULE_PAGE", TRUE); 

	require ('setup.inc.php');
	require ('language.inc.php');
	require ('calaccess.inc.php');

	class alt_schedule_page
	{
		var $alternate_page;
		var $activeday;
		var $activemonth;
		var $activeyear;

		// Constructor
		function alt_schedule_page()
		{
			$this->alternate_page = '';
			$this->activeday = 0;
			$this->activemonth = 0;
			$this->activeyear = 0;
		}

		function createpage($current_day, $current_month, $current_year)
		{
			$this->activeday = $current_day;
			$this->activemonth = $current_month;
			$this->activeyear = $current_year;

			// create the language object
			$lang = new language;
			// create the setup object
			$conf = new layout_setup;

			$dayofweek = gmdate("w", gmmktime(0, 0, 0, $current_month, $current_day, $current_year));

			$this->alternate_page .= '<center>
<form method="post" action="calendar.php">
<table width="95%" cellspacing="0" cellpadding="0" border="1">
<tr>
<td nowrap width="100%" align="right" valign="middle" bgcolor="#'.$conf->schedule_time_background_color.'" height="40" colspan="4">
<center>
<font size="3" point-size="14" color="#'.$conf->schedule_time_text_color.'"><b>'.$lang->day_long[$dayofweek].', '.$lang->month_long[$current_month].' '.$current_day.', '.$current_year.'</b></font>
</center>
<input type="submit" name="tocalendar" value="Calendar" />&nbsp;
<input type="hidden" name="month" value="'.$current_month.'" />
<input type="hidden" name="year" value="'.$current_year.'" />
</td>
</tr>
';

			$this->makeschedule($current_day, $current_month, $current_year);

			$this->alternate_page .= '</table>
</form>
<a href="http://www.proverbs.biz" target="_blank"><font size="1" point-size="10" color="#'.$conf->html_body_text_color.'">Web Calendar &copy;2004 Proverbs, LLC. All rights reserved.</font></a>
</center>';
			return $this->alternate_page;
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

			$eventlist = $this->fillschedule($current_day, $current_month, $current_year);

			$this->alternate_page .= '<tr>
<td nowrap width="10%" height="50" align="right" valign="middle" bgcolor="#'.$conf->schedule_time_background_color.'">
<font size="2" point-size="11" color="#'.$conf->schedule_time_text_color.'"><b>'.$lang->word_all_day.'</b> -&nbsp;</font>
</td>
<td width="90%" height="50" align="left" valign="top" bgcolor="#'.$conf->schedule_one_background_color.'" colspan="3">
<font size="1" point-size="8" color="#'.$conf->schedule_text_color.'">';

		if (trim($eventlist['ALL']) == '')
			$this->alternate_page .= '&nbsp;';
		else
			$this->alternate_page .= $eventlist['ALL'];

$this->alternate_page .= '</font>
</td>
</tr>
';

			$disp_zone = ' '.$conf->time_zone.' ';

			$use_one = false;

			for ($i = 0; $i < 12; $i++)
			{
				$subi = $i + 12;
				$display_time = $i;
				$sub_time = $subi;
				if ($twelvehour)
				{
					if ($i == 0)
						$display_time = '12 am'.$disp_zone;
					else
						$display_time .= ' am'.$disp_zone;

					if ($subi == 12)
						$sub_time .= ' pm'.$disp_zone;
					else
						$sub_time = ($subi - 12).' pm'.$disp_zone;
				}

				$use_bg_color = $conf->schedule_one_background_color;
				if (!$use_one)
					$use_bg_color = $conf->schedule_two_background_color;

				$this->alternate_page .= '<tr>
<td nowrap width="10%" height="50" align="right" valign="middle" bgcolor="#'.$conf->schedule_time_background_color.'">
<font size="2" point-size="11" color="#'.$conf->schedule_time_text_color.'"><b>'.$display_time.'</b> -&nbsp;</font>
</td>
<td width="40%" height="50" align="left" valign="top" bgcolor="#'.$use_bg_color.'">
<font size="1" point-size="8" color="#'.$conf->schedule_text_color.'">';

			if (trim($eventlist[$i]) == '')
				$this->alternate_page .= '&nbsp;';
			else
				$this->alternate_page .= $eventlist[$i];

$this->alternate_page .= '</font>
</td>
<td nowrap width="10%" height="50" align="right" valign="middle" bgcolor="#'.$conf->schedule_time_background_color.'">
<font size="2" point-size="11" color="#'.$conf->schedule_time_text_color.'"><b>'.$sub_time.'</b> -&nbsp;</font>
</td>
<td width="40%" height="50" align="left" valign="top" bgcolor="#'.$use_bg_color.'">
<font size="1" point-size="8" color="#'.$conf->schedule_text_color.'">';

			if (trim($eventlist[$subi]) == '')
				$this->alternate_page .= '&nbsp;';
			else
				$this->alternate_page .= $eventlist[$subi];

$this->alternate_page .= '</font>
</td>
</tr>
';

				$use_one = !$use_one;
			}
		}

		function fillschedule($current_day, $current_month, $current_year)
		{	
			$dayofweek = gmdate("w", gmmktime(0, 0, 0, $current_month, $current_day, $current_year));

			$eventholder = Array("ALL" => "", 0 => "", 1 => "", 2 => "", 3 => "", 4 => "", 5 => "", 6 => "", 7 => "", 8 => "",
				9 => "", 10 => "", 11 => "", 12 => "", 13 => "", 14 => "", 15  => "", 16 => "", 17 => "", 18 => "", 19 => "",
				20 => "", 21 => "", 22 => "", 23 => "");

			$conf = new layout_setup;

			$twelvehour = false;
			if ($conf->time_format == '12')
				$twelvehour = true;
			unset($conf);

			$calaccess = new caldbaccess;
			$itemcount = $calaccess->GetScheduleDateEvents($current_day, $current_month, $current_year);

			for ($i = 0; $i < $itemcount; $i++)
			{
				$calaccess->next_record();

				$on_hour = gmdate("G", $calaccess->f('event_date'));

				if (gmdate("H:i:s", $calaccess->f('event_date')) == '00:00:25')
					$on_hour = "ALL";
				else
				{
					if ($twelvehour)
						$eventholder[$on_hour] .= gmdate("g:i", $calaccess->f('event_date')).' - '.gmdate("g:i", intval($calaccess->f('event_date') + (60 * $calaccess->f('duration') * 60))).' ';
					else
						$eventholder[$on_hour] .= gmdate("G:i", $calaccess->f('event_date')).' - '.gmdate("G:i", intval($calaccess->f('event_date') + (60 * $calaccess->f('duration') * 60))).' ';
				}

				$eventholder[$on_hour] .= '<b>'.$calaccess->f('short_description').'</b><br />
'.$calaccess->f('long_description').'<br />
';
			}

			$itemcount = $calaccess->GetScheduleDayEvents($current_day, $current_month, $current_year);
			for ($i = 0; $i < $itemcount; $i++)
			{
				$calaccess->next_record();
				$breakup = explode(':', $calaccess->f('event_time'));

				$on_hour = gmdate("G", gmmktime($breakup[0], $breakup[1]));

				if ($calaccess->f('event_time') == '00:00:25')
					$on_hour = "ALL";
				else
				{
					if ($twelvehour)
						$eventholder[$on_hour] .= gmdate("g:i", gmmktime($breakup[0], $breakup[1])).' - '.gmdate("g:i", intval(gmmktime($breakup[0], $breakup[1]) + (60 * $calaccess->f('duration') * 60))).' ';
					else
						$eventholder[$on_hour] .= gmdate("G:i", gmmktime($breakup[0], $breakup[1])).' - '.gmdate("G:i", intval(gmmktime($breakup[0], $breakup[1]) + (60 * $calaccess->f('duration') * 60))).' ';
				}

				$eventholder[$on_hour] .= '<b>'.$calaccess->f('short_description').'</b><br />
'.$calaccess->f('long_description').'<br />
';
			}

			unset($calaccess);

			return $eventholder;
		}
	}
}
?>
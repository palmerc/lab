<?
/*  ©2004 Proverbs, LLC. All rights reserved.  */

if (eregi("schedule.nav.php", $_SERVER['PHP_SELF']))
{
	// redirect to calendar page
	header("Location: calendar.php");
	exit;
}

if(!defined("NETSCAPE_SCHEDULE_PAGE")) 
{ 
	define("NETSCAPE_SCHEDULE_PAGE", TRUE); 

	require ('setup.inc.php');
	require ('language.inc.php');
	require ('calaccess.inc.php');

	class nav_schedule_page
	{
		var $alternate_page;
		var $activeday;
		var $activemonth;
		var $activeyear;

		// Constructor
		function nav_schedule_page()
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
<table class="schedule" cellspacing="0" cellpadding="0" border="0">
<tr>
<td nowrap class="schedtop" colspan="4">
<b>'.$lang->day_long[$dayofweek].', '.$lang->month_long[$current_month].' '.$current_day.', '.$current_year.'</b>
<input type="submit" name="tocalendar" value="Calendar" />&nbsp;
<input type="hidden" name="month" value="'.$current_month.'" />
<input type="hidden" name="year" value="'.$current_year.'" />
</td>
</tr>
';

			$this->makeschedule($current_day, $current_month, $current_year);

			$this->alternate_page .= '</table>
</form>
<a class="calmenu" style="font-size: 10px" href="http://www.proverbs.biz" target="_blank">Web Calendar &copy;2004 Proverbs, LLC. All rights reserved.</a>
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
<td nowrap class="schedtime">
<b>'.$lang->word_all_day.'</b> -&nbsp;
</td>
<td class="schedone" style="width: 90%" colspan="3">
<div>';

		if (trim($eventlist['ALL']) == '')
			$this->alternate_page .= '&nbsp;';
		else
			$this->alternate_page .= $eventlist['ALL'];

$this->alternate_page .= '</div>
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

				$use_bg_color = "schedone";
				if (!$use_one)
					$use_bg_color = "schedtwo";

				$this->alternate_page .= '<tr>
<td nowrap class="schedtime">
<b>'.$display_time.'</b> -&nbsp;
</td>
<td class="'.$use_bg_color.'">
<div>';

			if (trim($eventlist[$i]) == '')
				$this->alternate_page .= '&nbsp;';
			else
				$this->alternate_page .= $eventlist[$i];

$this->alternate_page .= '</div>
</td>
<td nowrap class="schedtime">
<b>'.$sub_time.'</b> -&nbsp;
</td>
<td class="'.$use_bg_color.'">
<div>';

			if (trim($eventlist[$subi]) == '')
				$this->alternate_page .= '&nbsp;';
			else
				$this->alternate_page .= $eventlist[$subi];

$this->alternate_page .= '</div>
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
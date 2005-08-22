<?
/*  ©2004 Proverbs, LLC. All rights reserved.  */

if (eregi("caladmin.alt.php", $_SERVER['PHP_SELF']))
{
	// redirect to calendar page
	header("Location: ../calendar.php");
	exit;
}

if(!defined("ALT_ADMINISTRATION_PAGE")) 
{ 
	define("ALT_ADMINISTRATION_PAGE", TRUE); 

	require ('abase.inc.php');

	class alt_administrator_page extends base_administration
	{
		// Constructor
		function alt_administration_page()
		{
			$this->pagelayout = '';
		}

		function createpage($loggedin, $action, $userinfo)
		{
			$this->userid = $userinfo;
			if ($loggedin == 0)
			{
				$this->invalidlogin();
				return $this->pagelayout;
			}

			if ($loggedin == 1)
			{
				$this->showlogin();
				return $this->pagelayout;
			}

			// create the language object
			$lang = new language;

			switch ($action)
			{
			case $lang->word_user_admin:
				$this->useradministration();
				break;
			case $lang->word_create_user:
				$this->actionresults($this->addnewcalendaruser());
				$this->useradministration();
				break;
			case $lang->word_update_user:
				$this->actionresults($this->updatecalendaruser());
				$this->useradministration();
				break;
			case $lang->word_delete_user:
				$this->actionresults($this->deletecalendaruser());
				$this->useradministration();
				break;
			case $lang->word_create_event:
				$this->actionresults($this->addcalendarevent());
				$this->defaultpage();
				break;
			case $lang->word_delete_event:
				$this->actionresults($this->deletecalendarevent());
				$this->defaultpage();
				break;
			case $lang->word_update_event:
				$this->actionresults($this->updatecalendarevent());
				$this->defaultpage();
				break;
			default:
				$this->defaultpage();
			}

			return $this->pagelayout;
		}

		function useradministration()
		{
			// create the language object
			$lang = new language;

			// create the setup object
			$conf = new layout_setup;

			$this->pagelayout .= '<center>
<form method="post" action="caladmin.php">
<table bordercolor="#'.$conf->calendar_border_color.'" width="600" cellspacing="0" cellpadding="4" border="1">
<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" height="50" align="center" width="100%" colspan="2">
<font size="3" point-size="14" color="#'.$conf->calendar_title_text_color.'"><b>'.$lang->word_calendar_administration.'</b><br />
'.$lang->word_user_administration.'</font>
</td>
</tr>
<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="100%" align="center" height="40" colspan="2">
<input type="submit" name="submit" value="'.$lang->word_create_user.'" /> &nbsp;&nbsp;
<input type="submit" name="submit" value="'.$lang->word_update_user.'" /> &nbsp;&nbsp;
<input type="submit" name="submit" value="'.$lang->word_delete_user.'" /> &nbsp;&nbsp;
<input type="submit" name="submit" value="'.$lang->word_events.'" />
</td>
</tr>
<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="400" valign="top">
<table bordercolor="#'.$conf->calendar_border_color.'" width="100%" cellspacing="0" cellpadding="4" border="1">
<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="40%" height="40" align="right">
<font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'">'.$lang->word_username.':</font>
</td>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="60%" height="40">
<input type="text" name="useraccount" maxlength="30" size="30" />
</td>
</tr>
';
			if (!$conf->admin_serverside_login)
				$this->pagelayout .= '<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="40%" height="40" align="right">
<font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'">'.$lang->word_password.':</font>
</td>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="60%" height="40">
<input type="password" name="accountpassword" maxlength="30" size="30" />
</td>
</tr>
';
			$this->pagelayout .= '<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="40%" height="40" align="right">
<font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'">'.$lang->word_access_level.':</font>
</td>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="60%" height="40">
<select name="accountlevel">
<option value="1">'.$lang->word_author.'</option>
<option value="2">'.$lang->word_full.'</option>
</select>
</td>
</tr>
</table>
</td>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="200" valign="top">
<table width="100%" cellspacing="0" cellpadding="4" border="1">
<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" align="center">
<font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'"><b>'.$lang->word_existing_users.'</b></font>
</td>
</tr>
';
			// create the database access object
			$accesscaldb = new caldbaccess;
			$numofusers = $accesscaldb->GetAllUsers();

			for ($i = 0; $i < $numofusers; $i++)
			{
				$accesscaldb->next_record();
				$this->pagelayout .= '<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'">
<input type="radio" name="editaccount" value="'.$accesscaldb->f('userid').'" /><font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'">'.$accesscaldb->f('userid').'</font>
</td>
</tr>
';	
			}
			unset($accesscaldb);
			$this->pagelayout .= '</table>
</td>
</tr>
</table>
</form>
</center>';			
		}

		function actionresults($successtext)
		{
			// create the setup object
			$conf = new layout_setup;

			$this->pagelayout .= '<center>
<table bordercolor="#'.$conf->calendar_border_color.'" width="450" cellspacing="0" cellpadding="4" border="1">
<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" align="center" width="100%">
<font size="3" point-size="14" color="#'.$conf->calendar_title_text_color.'">'.$successtext.'</font>
</td>
</tr>
</table>
</center>
';
		}

		function defaultpage()
		{
			// create the language object
			$lang = new language;

			// create the setup object
			$conf = new layout_setup;

			$adjustment = 3600 * $conf->time_adjustment;
			if ($conf->use_dst)
				$adjustment += 3600 * date("I", time() + $adjustment);
			$current_month = gmdate("n", time() + $adjustment);
			$current_year = gmdate("Y", time() + $adjustment);
			if ($current_year < 1980) 
				$current_year = 1980;

			$this->pagelayout .= '<center>
<form method="post" action="caladmin.php">
<table bordercolor="#'.$conf->calendar_border_color.'" width="750" cellspacing="0" cellpadding="4" border="1">
<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" height="40p" align="center" width="100%" colspan="2">
<font size="3" point-size="14" color="#'.$conf->calendar_title_text_color.'"><b>'.$lang->word_calendar_administration.'</b></font>
</td>
</tr>
<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="100%" align="center" height="40" colspan="2">
<input type="submit" name="submit" value="'.$lang->word_create_event.'" /> &nbsp;&nbsp;&nbsp;
<input type="submit" name="submit" value="'.$lang->word_update_event.'" />  &nbsp;&nbsp;&nbsp;
<input type="submit" name="submit" value="'.$lang->word_delete_event.'" />
</td>
</tr>
<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="450" valign="top">
<table bordercolor="#'.$conf->calendar_border_color.'" width="450" cellspacing="0" cellpadding="4" border="1">
<tr>
';
			if ($this->userid['level'] == 2)
				$this->pagelayout .= '<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="34%">
<font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'">'.$lang->word_username.': '.$this->userid['userid'].'</font>
</td>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="76%" align="center">
<input type="submit" name="submit" value="'.$lang->word_user_admin.'" />';
			else
				$this->pagelayout .= '<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="100%" colspan="2">
<font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'">'.$lang->word_username.': '.$this->userid['userid'].'</font>';		
			$this->pagelayout .= '
</td>
</tr>
<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="100%" align="center" colspan="2">
<font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'"><b>'.$lang->word_events.'</b></font>
</td>
</tr>
<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="34%" align="right">
<font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'">'.$lang->word_event_title.':</font>
</td>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="76%">
<input type="text" name="eventtitle" maxlength="50" size="38" />
</td>
</tr>
<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="34%" align="right" valign="top" height="60">
<font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'">'.$lang->word_event_details.':</font>
</td>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="76%" height="60">
<textarea name="eventdetails" cols="29" rows="4"></textarea>
</td>
</tr>
<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="34%" align="right">
<font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'">'.$lang->word_start_time.':</font>
</td>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="76%">
<select name="starttime">
<option value="00:00:25">'.$lang->word_all_day.'</option>
';
			if ($conf->time_format == '12')
			{
				for ($i = 0; $i < 2; $i++)
				{
					for ($j = 0; $j < 12; $j++)
					{
						for ($k = 0; $k < 4; $k++)
						{
							$onhour = $j;
							if ($j == 0)
								$onhour = 12;
							$onmin = $k * 15;
							$twentyfour = $j;
							$ampm = "am";
							if ($i == 1)
							{
								$ampm = "pm";
								$twentyfour = $j + 12;
							}
							$this->pagelayout .= '<option value="'.sprintf('%02d:%02d:00', $twentyfour, $onmin).'"';
							if (isset($_POST['starttime']))
							{
								if ($_POST['starttime'] == sprintf('%02d:%02d:00', $twentyfour, $onmin))
									$this->pagelayout .= ' selected';
							}
							else
							{
								if ($twentyfour == 8 && $onmin == 30)
									$this->pagelayout .= ' selected';
							}
							$this->pagelayout .= '>'.sprintf('%2d:%02d %s', $onhour, $onmin, $ampm).'</option>
';
						}
					}
				}
			}
			else
			{
				for ($j = 0; $j < 24; $j++)
				{
					for ($k = 0; $k < 4; $k++)
					{
						$onhour = $j;
						$onmin = $k * 15;
						$this->pagelayout .= '<option value="'.sprintf('%02d:%02d:00', $j, $onmin).'"';
							if (isset($_POST['starttime']))
							{
								if ($_POST['starttime'] == sprintf('%02d:%02d:00', $j, $onmin))
									$this->pagelayout .= ' selected';
							}
							else
							{
								if ($j == 8 && $onmin == 30)
									$this->pagelayout .= ' selected';
							}						
						$this->pagelayout .= '>'.sprintf('%02d:%02d', $j, $onmin).'</option>
';
					}
				}
			}
			$this->pagelayout .= '</select>
</td>
</tr>
<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="34%" align="right">
<font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'">'.$lang->word_end_time.':</font>
</td>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="76%">
<select name="endtime">
';
			if ($conf->time_format == '12')
			{
				for ($i = 0; $i < 2; $i++)
				{
					for ($j = 0; $j < 12; $j++)
					{
						for ($k = 0; $k < 4; $k++)
						{
							if ($i == 0 && $j == 0 && $k == 0)
								continue;
							$onhour = $j;
							if ($j == 0)
								$onhour = 12;
							$onmin = $k * 15;
							$twentyfour = $j;
							$ampm = "am";
							if ($i == 1)
							{
								$ampm = "pm";
								$twentyfour = $j + 12;
							}
							$this->pagelayout .= '<option value="'.sprintf('%02d:%02d:00', $twentyfour, $onmin).'"';
							if (isset($_POST['endtime']))
							{
								if ($_POST['endtime'] == sprintf('%02d:%02d:00', $twentyfour, $onmin))
									$this->pagelayout .= ' selected';
							}
							else
							{
								if ($twentyfour == 9 && $onmin == 0)
									$this->pagelayout .= ' selected';
							}
							$this->pagelayout .= '>'.sprintf('%2d:%02d %s', $onhour, $onmin, $ampm).'</option>
';
						}
					}
				}
				$this->pagelayout .= '<option value="00:00:00"';
				if (isset($_POST['endtime']) && $_POST['endtime'] == '00:00:00')
						$this->pagelayout .= ' selected';
				$this->pagelayout .= '>12:00 am</option>
';
			}
			else
			{
				for ($j = 0; $j < 24; $j++)
				{
					for ($k = 0; $k < 4; $k++)
					{
						if ($j == 0 && $k == 0)
							continue;
						$onhour = $j;
						$onmin = $k * 15;
						$this->pagelayout .= '<option value="'.sprintf('%02d:%02d:00', $j, $onmin).'"';
						if (isset($_POST['endtime']))
						{
							if ($_POST['endtime'] == sprintf('%02d:%02d:00', $j, $onmin))
								$this->pagelayout .= ' selected';
						}
						else
						{
							if ($j == 9 && $onmin == 0)
								$this->pagelayout .= ' selected';
						}						
						$this->pagelayout .= '>'.sprintf('%02d:%02d', $j, $onmin).'</option>
';
					}
				}
				$this->pagelayout .= '<option value="00:00:00"';
				if (isset($_POST['endtime']) && $_POST['endtime'] == '00:00:00')
						$this->pagelayout .= ' selected';
				$this->pagelayout .= '>24:00</option>
';
			}
			$this->pagelayout .= '</select>
</td>
</tr>
<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="34%" align="right">
<font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'">'.$lang->word_event_type.':</font>
</td>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="76%">';
			$datetype = FALSE;
			if (!isset($_POST['eventtype']) || $_POST['eventtype'] != 'weekday')
				$datetype = TRUE;

			$this->pagelayout .= '
<input type="radio" name="eventtype" value="date"';
			if ($datetype)
				$this->pagelayout .= ' checked';
			$this->pagelayout .= ' /><font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'">Date Event</font>
<input type="radio" name="eventtype" value="weekday"';
			if (!$datetype)
				$this->pagelayout .= ' checked';
			$this->pagelayout .= ' /><font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'">Weekday Event</font>
</td>
</tr>
<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="34%" align="right">
<font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'">'.$lang->word_date.':</font>
</td>
<td bgcolor="#'.$conf->calendar_title_background_color.'" width="76%">
<font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'">'.$lang->word_month.':</font>
<select name="datemonth">
';
			for ($i = 1; $i < 13; $i++)
			{
				$this->pagelayout .= '<option value="'.$i.'"';
				if (isset($_POST['datemonth']))
				{
					if ($_POST['datemonth'] == $i)
						$this->pagelayout .= ' selected';
				}
				else
				{
					if ($current_month == $i)
						$this->pagelayout .= ' selected';
				}
				$this->pagelayout .= '>'.$lang->month_short[$i].'</option>
';	
			}
			$this->pagelayout .= '<option value="0">'.$lang->word_all.'</option>
</select>
<font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'">'.$lang->word_day.':</font>
<input type="text" name="dateday" size="2" maxlength="2" />
<font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'">'.$lang->word_year.':</font>
<select name="dateyear">';
			for ($i = 0; $i < 6; $i++)
			{
				$onyear = $current_year + $i;
				$this->pagelayout .= '<option value="'.$onyear.'"';
				if (isset($_POST['dateyear']) && $_POST['dateyear'] == $onyear)
					$this->pagelayout .= ' selected';
				$this->pagelayout .= '>'.$onyear.'</option>
';	
			}
			$this->pagelayout .= '<option value="1972">'.$lang->word_all.'</option>
</select>
</td>
</tr>
<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="34%" align="right">
<font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'">'.$lang->word_weekday.':</font>
</td>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="76%">
<select name="dayschedule">
<option value="0">'.$lang->word_every.'</option>
<option value="1">1st</option>
<option value="2">2nd</option>
<option value="3">3rd</option>
<option value="4">4th</option>
<option value="5">5th</option>
</select>
<select name="dayweekday">
';
			for ($i = 0; $i < 7; $i++)
			{
				$this->pagelayout .= '<option value="'.$i.'"';
				if (isset($_POST['dayweekday']) && $_POST['dayweekday'] == $i)
					$this->pagelayout .= ' selected';
				$this->pagelayout .= '>'.$lang->day_short[$i].'</option>
';	
			}
			$this->pagelayout .= '</select>
<font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'"> '.$lang->word_of.' </font>
<select name="daymonth">
<option value="0">'.$lang->word_all_months.'</option>
';
			for ($i = 1; $i < 13; $i++)
			{
				$this->pagelayout .= '<option value="'.$i.'"';
				if (isset($_POST['daymonth']))
				{
					if ($_POST['daymonth'] == $i)
						$this->pagelayout .= ' selected';
				}
				else
				{
					if ($current_month == $i)
						$this->pagelayout .= ' selected';
				}
				$this->pagelayout .= '>'.$lang->month_long[$i].'</option>
';	
			}
			$this->pagelayout .= '</select>
</td>
</tr>
</table>
</td>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="300" valign="top">
<table bordercolor="#'.$conf->calendar_border_color.'" width="300" cellspacing="0" cellpadding="2" border="1">
<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="100%" valign="middle" align="center" colspan="2">
<font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'"><b>'.$lang->word_existing_events.'</b></font>
</td>
</tr>
';

			$existingcount = $this->getexistingevents();
			$this->pagelayout .= '</table>
</td>
</tr>
</table>
</form>
</center>';
		}

		function getexistingevents()
		{
			$datecount = 0;
			$daycount = 0;
			// create the setup object
			$conf = new layout_setup;

			$thold = time() - (3600 * 24 * 14);
			$startdate = gmmktime(0, 0, 0, date("n", $thold), date("j", $thold), date("y", $thold));
			$thold = time() + (3600 * 24 * 14);
			$enddate = gmmktime(23, 59, 59, date("n", $thold), date("j", $thold), date("y", $thold));
			if (isset($_POST['edate']) && $_POST['edate'] != "")
			{
				$endvalue = array();
				$endvalue = explode('/', trim($_POST['edate']));
				if (count($endvalue) == 3)
					$enddate = gmmktime(23, 59, 59, $endvalue[0], $endvalue[1], $endvalue[2]);
			}

			if (isset($_POST['sdate']) && $_POST['sdate'] != "")
			{
				$startvalue = array();
				$startvalue = explode('/', trim($_POST['sdate']));
				if (count($startvalue) == 3)
					$startdate = gmmktime(0, 0, 0, $startvalue[0], $startvalue[1], $startvalue[2]);
			}

			if ($startdate > $enddate)
			{
				$holddate = $enddate;
				$enddate = $startdate;
				$startdate = $holddate;	
			}

			$retvalue = 0;
			// create the database access object
			$accesscaldb = new caldbaccess;
			if ($this->userid['level'] == 2)
				$datecount = $accesscaldb->GetBetweenDates($startdate, $enddate);
			else
				$datecount = $accesscaldb->GetSelfBetweenDates($this->userid['userid'], $startdate, $enddate);

			// create the language object
			$lang = new language;

			if ($datecount > 0)
			{
				$this->pagelayout .= '<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="100%" align="center" colspan="2">
<table bordercolor="#'.$conf->calendar_border_color.'" width="100%" cellspacing="0" cellpadding="2" border="0">
';

				for ($i = 0; $i < $datecount; $i++)
				{
					$accesscaldb->next_record();
					$sthold = gmdate("H:i:s", $accesscaldb->f('event_date'));
					$ethold = gmdate("H:i:s", $accesscaldb->f('event_date') + ($accesscaldb->f('duration') * 3600));
					$emhold = gmdate("n", $accesscaldb->f('event_date'));
					$edhold = gmdate("j", $accesscaldb->f('event_date'));
					$eyhold = gmdate("Y", $accesscaldb->f('event_date'));
					if ($eyhold == 1971)
					{
						$emhold = 0;
						$eyhold = 1972;
					}
					$this->pagelayout .= '<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="100%">
<input type="radio" name="eventid" value="'.$accesscaldb->f('id').'D" />
<font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'">'.$accesscaldb->f('short_description').'</font>
</td>
</tr>
';
				}
			}

			if (isset($_POST['wdevents']) && $_POST['wdevents'] == "weekdayevents")
			{
				if ($this->userid['level'] == 2)
					$daycount = $accesscaldb->GetAllRecurring();
				else
					$daycount = $accesscaldb->GetSelfRecurring($this->userid['userid']);
			}

			if ($daycount > 0)
			{
				if ($datecount == 0)
				{
					$this->pagelayout .= '<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="100%" align="center" colspan="2">
<table bordercolor="#'.$conf->calendar_border_color.'" width="100%" cellspacing="0" cellpadding="2" border="0">
';
				}
				for ($i = 0; $i < $daycount; $i++)
				{
					$accesscaldb->next_record();
					$timebreak = explode(':', trim($accesscaldb->f('event_time')));
					$stoptime = date("H:i:s", mktime($timebreak[0], $timebreak[1], $timebreak[2], 1, 1, 2000) + ($accesscaldb->f('duration') * 3600));
					$this->pagelayout .= '<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="100%">
<input type="radio" name="eventid" value="'.$accesscaldb->f('id').'W" />
<font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'">'.$accesscaldb->f('short_description').'</font>
</td>
</tr>
';
				}
			}

			$retvalue = $datecount + $daycount;
			if ($retvalue > 0)
				$this->pagelayout .= '</table>
</td>
</tr>
';
			$this->pagelayout .= '<tr>
<td bgcolor="#'.$conf->calendar_title_background_color.'" width="35%" align="right">
<font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'">'.$lang->word_show_events.':</font>
</td>
<td bgcolor="#'.$conf->calendar_title_background_color.'" width="65%" valign="middle">
<input type="text" name="sdate" value="'.gmdate("n/j/y", $startdate).'" size="6" /> - <input type="text" name="edate" value="'.gmdate("n/j/y", $enddate).'" size="6" />
</td>
</tr>
<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="100%" align="center" colspan="2">
<input type="checkbox" name="wdevents" value="weekdayevents" ';
			if (isset($_POST['wdevents']) && $_POST['wdevents'] == "weekdayevents")
				$this->pagelayout .= 'checked ';
			$this->pagelayout .= '/><font size="2" point-size="12" color="#'.$conf->calendar_title_text_color.'">'.$lang->word_show_weekday_events.'</font>
</td>
</tr>
<tr>
<td nowrap bgcolor="#'.$conf->calendar_title_background_color.'" width="100%" align="center" colspan="2">
<input type="submit" name="submit" value="'.$lang->word_refresh.'" />
</td>
</tr>
';
			unset($lang);
			return $retvalue;
		}

		function showlogin()
		{
			// create the setup object
			$conf = new layout_setup;

			// create the language object
			$lang = new language;
			$this->pagelayout .= '<br /><br /><br /><br />
<center>
<form method="post" action="caladmin.php">
<table width="400" bordercolor="#'.$conf->schedule_border_color.' "cellspacing="0" cellpadding="4" border="1">
<tr>
<td width="40%" align="right" valign="middle" bgcolor="#'.$conf->schedule_time_background_color.'">
<font size="2" point-size="12" color="#'.$conf->schedule_time_text_color.'"><b>'.$lang->word_username.': </b></font>
</td>
<td  valign="middle" bgcolor="#'.$conf->schedule_time_background_color.'" width="60%" align="left">
<input type="text" name="loginname" size="30" />
</td>
</tr>
<tr>
<td valign="middle" bgcolor="#'.$conf->schedule_time_background_color.'" width="40%" align="right">
<font size="2" point-size="12" color="#'.$conf->schedule_time_text_color.'"><b>'.$lang->word_password.': </b></font>
</td>
<td valign="middle" bgcolor="#'.$conf->schedule_time_background_color.'" width="60%" align="left">
<input type="password" name="loginpass" size="30" />
</td>
</tr>
<tr>
<td align="center" valign="middle" bgcolor="#'.$conf->schedule_time_background_color.'" width="100%" colspan="2">
<input type="submit" name="submit" value="'.$lang->word_login.'" />
</td>
</tr>
</table>
</form>
</center>';
			unset($lang);
		}

		function invalidlogin()
		{
			// create the language object
			$lang = new language;
			$this->pagelayout .= '<br /><br /><br />
<center>
<table width="200" cellspacing="0" cellpadding="0" border="0">
<tr>
<td width="100%" align="center">
<font size="4" point-size="16"><b>'.$lang->word_access_denied.'</b></font>
</td>
</tr>
</table>
</center>
';
			unset($lang);
		}
	}
}
?>
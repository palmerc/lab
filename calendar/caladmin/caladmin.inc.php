<?
/*  ©2004 Proverbs, LLC. All rights reserved.  */

if (eregi("caladmin.inc.php", $_SERVER['PHP_SELF']))
{
	// redirect to calendar page
	header("Location: ../calendar.php");
	exit;
}

if(!defined("ADMINISTRATION_PAGE")) 
{ 
	define("ADMINISTRATION_PAGE", TRUE); 

	require ('abase.inc.php');

	class administration_page extends base_administration
	{
		// Constructor
		function administration_page()
		{
			$this->base_administration();
		}

		function createpage($action)
		{
			// create the language object
			$lang = new language;

			// 0 = invalid login, 1 = sql login needed, 2 = login OK
			$loggedin = 0;

			if ($action == $lang->word_login)
			{
				$user = "";
				$pass = "";
				if (isset($_POST['loginname']) && $_POST['loginname'] != "")
					$user = $_POST['loginname'];
				if (isset($_POST['loginpass']) && $_POST['loginpass'] != "")
					$pass = $_POST['loginpass'];
				$loggedin = $this->loginuser($user, $pass);
			}
			else
				$loggedin = $this->checkforid();

			$this->browser_version = $this->makeheader();

			if ($this->browser_version > 0)
			{
				if ($this->browser_version == 1)
					$this->pagelayout .= '<link rel="stylesheet" type="text/css" href="../navstylesheet.css.php" />
';
				if ($this->browser_version == 2)
					$this->pagelayout .= '<link rel="stylesheet" type="text/css" href="../iestylesheet.css.php" />
';

				$this->pagelayout .= '<script type="text/javascript" src="adminjavascript.js.php"></script>
';
				$this->closeheader();
			}
			else
			{
				$this->closeheader();
				unset($lang);
				// require alternate calendar
				require ('caladmin.alt.php');
				$altadmin = new alt_administrator_page;
				$this->pagelayout .= $altadmin->createpage($loggedin, $action, $this->userid);
				return;
			}

			if ($loggedin == 0)
			{
				$this->invalidlogin();
				return;
			}

			if ($loggedin == 1)
			{
				$this->showlogin();
				return;
			}

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
			unset($lang);
		}

		function useradministration()
		{
			// create the language object
			$lang = new language;

			// create the setup object
			$conf = new layout_setup;

			$this->pagelayout .= '<center>
<form method="post" name="calAdmin" action="caladmin.php">
<table style="width: 600px; border-width: 1px; border-style: ridge" cellspacing="0" cellpadding="0" border="1">
<tr>
<td class="caladmin" style="height: 50px; text-align: center; font-size: 14px; width: 100%" colspan="2">
<b>'.$lang->word_calendar_administration.'</b><br />
'.$lang->word_user_administration.'
</td>
</tr>
<tr>
<td class="caladmin" style="width: 400px; padding: 0px; vertical-align: top">
<table style="width: 100%; border-width: 1px; border-style: ridge" cellspacing="0" cellpadding="0" border="1">
<tr>
<td class="caladmin" style="width: 40%; height: 40px; text-align: right">
'.$lang->word_username.':
</td>
<td class="caladmin" style="width: 60%; height: 40px">
<input type="text" name="useraccount" style="width: 96%" maxlength="30" />
</td>
</tr>
';
			if (!$conf->admin_serverside_login)
				$this->pagelayout .= '<tr>
<td class="caladmin" style="width: 40%; height: 40px; text-align: right">
'.$lang->word_password.':
</td>
<td class="caladmin" style="width: 60%; height: 40px">
<input type="password" name="accountpassword" style="width: 96%" maxlength="30" />
</td>
</tr>
';
			$this->pagelayout .= '<tr>
<td class="caladmin" style="width: 40%; height: 40px; text-align: right">
'.$lang->word_access_level.':
</td>
<td class="caladmin" style="width: 60%; height: 40px">
<select name="accountlevel">
<option value="1">'.$lang->word_author.'</option>
<option value="2">'.$lang->word_full.'</option>
</select>
</td>
</tr>
</table>
</td>
<td class="caladmin" style="width: 200px; padding: 0px; vertical-align: top">
<div style="height: 120px">
<table style="width: 100%; border-width: 1px; border-style: ridge" cellspacing="0" cellpadding="0" border="1">
<tr>
<td class="caladmin" style="text-align: center">
<b>'.$lang->word_existing_users.'</b>
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
<td class="caladmin">
<input type="radio" name="editaccount" value="'.$accesscaldb->f('userid').'" 
	onclick="sai(\''.$accesscaldb->f('userid').'\', \''.$accesscaldb->f('level').'\');" />'.$accesscaldb->f('userid').'
</td>
</tr>
';	
			}
			unset($accesscaldb);
			$this->pagelayout .= '</table>
</div>
</td>
</tr>
<tr>
<td class="caladmin" style="width: 100%; text-align: center; height: 40px" colspan="2">
<input type="submit" name="submit" value="'.$lang->word_create_user.'" style="width: 100px" /> &nbsp;&nbsp;
<input type="submit" name="submit" value="'.$lang->word_update_user.'" style="width: 100px" /> &nbsp;&nbsp;
<input type="submit" name="submit" value="'.$lang->word_delete_user.'" style="width: 100px" /> &nbsp;&nbsp;
<input type="submit" name="submit" value="'.$lang->word_events.'" style="width: 100px" />
</td>
</tr>
</table>
</form>
<script language="JavaScript">
<!--
	function sai(aName, aLevel)
	{
		document.calAdmin.useraccount.value = aName;
		document.calAdmin.accountlevel.value = aLevel;
';
			if (!$conf->admin_serverside_login)
				$this->pagelayout .= '		document.calAdmin.accountpassword.value = "XOX0XOX0XO";
';
			$this->pagelayout .= '
	}
//-->
</script>
</center>
';			
		}

		function actionresults($successtext)
		{
			$this->pagelayout .= '<center>
<table style="width: 450px; border-width: 1px; border-style: ridge" cellspacing="0" cellpadding="0" border="1">
<tr>
<td class="caladmin" style="text-align: center; font-size: 14px; width: 100%">
'.$successtext.'
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
<form method="post" name="calAdmin" action="caladmin.php">
<table style="width: 750px; border-width: 1px; border-style: ridge" cellspacing="0" cellpadding="0" border="1">
<tr>
<td class="caladmin" style="height: 40px; text-align: center; font-size: 14px; width: 100%" colspan="2">
<b>'.$lang->word_calendar_administration.'</b>
</td>
</tr>
<tr>
<td class="caladmin" style="width: 450px; padding: 0px; vertical-align: top">
<table style="width: 450px; border-width: 1px; border-style: ridge" cellspacing="0" cellpadding="0" border="1">
<tr>
';
			if ($this->userid['level'] == 2)
				$this->pagelayout .= '<td class="caladmin" style="width: 34%">
'.$lang->word_username.': '.$this->userid['userid'].'
</td>
<td class="caladmin" style="width: 76%; text-align: center">
<input type="submit" name="submit" value="'.$lang->word_user_admin.'" style="width: 100px" />';
			else
				$this->pagelayout .= '<td class="caladmin" style="width: 100%" colspan="2">
'.$lang->word_username.': '.$this->userid['userid'];		
			$this->pagelayout .= '
</td>
</tr>
<tr>
<td class="caladmin" style="width: 100%; text-align: center" colspan="2">
<b>'.$lang->word_events.'</b>
</td>
</tr>
<tr>
<td class="caladmin" style="width: 34%; text-align: right">
'.$lang->word_event_title.':
</td>
<td class="caladmin" style="width: 76%">
<input type="text" name="eventtitle" style="width: 96%" maxlength="50" />
</td>
</tr>
<tr>
<td class="caladmin" style="width: 34%; text-align: right; vertical-align: top; height: 60px;">
'.$lang->word_event_details.':
</td>
<td class="caladmin" style="width: 76%; height: 60px">
<textarea name="eventdetails" style="width: 96%" cols="29" rows="4"></textarea>
</td>
</tr>
<tr>
<td class="caladmin" style="width: 34%; text-align: right">
'.$lang->word_start_time.':
</td>
<td class="caladmin" style="width: 76%">
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
<td class="caladmin" style="width: 34%; text-align: right">
'.$lang->word_end_time.':
</td>
<td class="caladmin" style="width: 76%">
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
<td class="caladmin" style="width: 34%; text-align: right">
'.$lang->word_event_type.':
</td>
<td class="caladmin" style="width: 76%">';
			$datetype = FALSE;
			$datedis = '';
			$daydis = '';
			if (!isset($_POST['eventtype']) || $_POST['eventtype'] != 'weekday')
			{
				$datetype = TRUE;
				$daydis = 'disabled ';	
			}
			else
				$datedis = 'disabled ';
			$this->pagelayout .= '
<input type="radio" name="eventtype" value="date"';
			if ($datetype)
				$this->pagelayout .= ' checked';
			$this->pagelayout .= ' onclick="en(true);" />Date Event
<input type="radio" name="eventtype" value="weekday"';
			if (!$datetype)
				$this->pagelayout .= ' checked';
			$this->pagelayout .= ' onclick="en(false);" />Weekday Event
</td>
</tr>
<tr>
<td class="caladmin" style="width: 34%; text-align: right">
'.$lang->word_date.':
</td>
<td class="caladmin" style="width: 76%">
'.$lang->word_month.': 
<select name="datemonth" '.$datedis.'>
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
'.$lang->word_day.': 
<input type="text" name="dateday" style="width: 25px" maxlength="2" '.$datedis.'/>
'.$lang->word_year.': 
<select name="dateyear" '.$datedis.'>';
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
<td class="caladmin" style="width: 34%; text-align: right">
'.$lang->word_weekday.':
</td>
<td class="caladmin" style="width: 76%">
<select name="dayschedule" '.$daydis.'>
<option value="0">'.$lang->word_every.'</option>
<option value="1">1st</option>
<option value="2">2nd</option>
<option value="3">3rd</option>
<option value="4">4th</option>
<option value="5">5th</option>
</select>
<select name="dayweekday" '.$daydis.'>
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
 '.$lang->word_of.'  
<select name="daymonth" '.$daydis.'>
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
<td class="caladmin" style="width: 300px; padding: 0px; vertical-align: top">
<table style="width: 300px; border-width: 1px; border-style: ridge" cellspacing="0" cellpadding="0" border="1">
<tr>
<td class="caladmin" style="width: 100%; text-align: center" colspan="2">
<b>'.$lang->word_existing_events.'</b>
</td>
</tr>
';

			$existingcount = $this->getexistingevents();
			$this->pagelayout .= '</table>
</td>
</tr>
<tr>
<td class="caladmin" style="width: 100%; text-align: center; height: 40px" colspan="2">
<input type="submit" name="submit" value="'.$lang->word_create_event.'" style="width: 100px" onclick="return val();" /> &nbsp;&nbsp;&nbsp;
<input type="submit" name="submit" value="'.$lang->word_update_event.'" style="width: 100px"';
			if ($existingcount == 0)
				$this->pagelayout .= ' disabled';
			$this->pagelayout .= ' onclick="return val();" />  &nbsp;&nbsp;&nbsp;
<input type="submit" name="submit" value="'.$lang->word_delete_event.'" style="width: 100px"';
			if ($existingcount == 0)
				$this->pagelayout .= ' disabled';
			$this->pagelayout .= ' />
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

			$sqheight = 204;

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
<td class="caladmin" style="width: 100%; text-align: center; padding: 0px" colspan="2">
<div style="height: '.$sqheight.'px">
<table style="width: 100%" cellspacing="0" cellpadding="0" border="0">
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
<td class="caladmin" style="width: 100%">
<input type="radio" name="eventid" value="'.$accesscaldb->f('id').'D" onclick="sed(\''.$accesscaldb->f('short_description').'\', 
	\''.$this->ConvertBackJava($accesscaldb->f('long_description')).'\', 
	\''.$sthold.'\', \''.$ethold.'\', \''.$emhold.'\', \''.$edhold.'\', \''.$eyhold.'\')" />
'.$accesscaldb->f('short_description').'
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
<td class="caladmin" style="width: 100%; text-align: center; padding: 0px" colspan="2">
<div style="height: '.$sqheight.'px">
<table style="width: 100%" cellspacing="0" cellpadding="0" border="0">
';
				}
				for ($i = 0; $i < $daycount; $i++)
				{
					$accesscaldb->next_record();
					$timebreak = explode(':', trim($accesscaldb->f('event_time')));
					$stoptime = date("H:i:s", mktime($timebreak[0], $timebreak[1], $timebreak[2], 1, 1, 2000) + ($accesscaldb->f('duration') * 3600));
					$this->pagelayout .= '<tr>
<td class="caladmin" style="width: 100%">
<input type="radio" name="eventid" value="'.$accesscaldb->f('id').'W" onclick="sew(\''.$accesscaldb->f('short_description').'\', 
	\''.$this->ConvertBackJava($accesscaldb->f('long_description')).'\', 
	\''.$accesscaldb->f('event_time').'\', \''.$stoptime.'\', \''.$accesscaldb->f('period').'\', \''.$accesscaldb->f('weekday').'\', \''.$accesscaldb->f('month').'\')" />
'.$accesscaldb->f('short_description').'
</td>
</tr>
';
				}
			}

			$retvalue = $datecount + $daycount;
			if ($retvalue > 0)
				$this->pagelayout .= '</table>
</div>
</td>
</tr>
';
			$this->pagelayout .= '<tr>
<td class="caladmin" style="width: 35%; text-align: right">
'.$lang->word_show_events.':
</td>
<td class="caladmin" style="width: 65%; vertical-align: middle">
<input type="text" name="sdate" value="'.gmdate("n/j/y", $startdate).'" style="width: 60px" /> - <input type="text" name="edate" value="'.gmdate("n/j/y", $enddate).'" style="width: 60px" />
</td>
</tr>
<tr>
<td class="caladmin" style="width: 100%; text-align: center" colspan="2">
<input type="checkbox" name="wdevents" value="weekdayevents" ';
			if (isset($_POST['wdevents']) && $_POST['wdevents'] == "weekdayevents")
				$this->pagelayout .= 'checked ';
			$this->pagelayout .= '/>'.$lang->word_show_weekday_events.'
</td>
</tr>
<tr>
<td class="caladmin" style="width: 100%; text-align: center" colspan="2">
<input type="submit" name="submit" value="'.$lang->word_refresh.'" style="width: 100px" />
</td>
</tr>
';
			unset($lang);
			return $retvalue;
		}

		function showlogin()
		{
			// create the language object
			$lang = new language;
			$this->pagelayout .= '<br /><br /><br /><br />
<center>
<form method="post" action="caladmin.php">
<table class="login" cellspacing="0" cellpadding="0">
<tr>
<td class="login" style="width: 40%; text-align: right">
<b>'.$lang->word_username.': </b>
</td>
<td class="login" style="width: 60%; text-align: left">
<input type="text" name="loginname" style="width: 220px" />
</td>
</tr>
<tr>
<td class="login" style="width: 40%; text-align: right">
<b>'.$lang->word_password.': </b>
</td>
<td class="login" style="width: 60%; text-align: left">
<input type="password" name="loginpass" style="width: 220px" />
</td>
</tr>
<tr>
<td class="login" style="width: 100%" colspan="2">
<input type="submit" name="submit" value="'.$lang->word_login.'" style="width: 100px" />
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
<table style="width: 200px" cellspacing="0" cellpadding="0" border="0">
<tr>
<td style="width: 100%; font-size: 16px; text-align: center">
<b>'.$lang->word_access_denied.'</b>
</td>
</tr>
</table>
</center>
';
			unset($lang);
		}
	}
}
// Create the administration object.
$page = new administration_page;
?>
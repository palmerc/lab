<?
/* ©2004 Proverbs, LLC. All rights reserved. */ 

if (eregi("calaccess.inc.php", $_SERVER['PHP_SELF']))
{
	// redirect to calendar page
	header("Location: calendar.php");
	exit;
}

if(!defined("CALENDAR_DATABASE")) 
{ 
	define("CALENDAR_DATABASE", TRUE); 

	require('db_mysql.inc.php');
	require('setup.inc.php');

	class caldbaccess extends DB_Sql
	{
		// - host & database info
		var $Host;
		var $Database;
		var $User;
		var $Password;

		// - table names - used to avoid having to change a tablename in multiple places
		var $tbl_bydate 					= 'calendardate';
		var $tbl_recurring 				= 'calendarday';
		var $tbl_users 					= 'calendarusers';

		// Constructor
		function caldbaccess()
		{
			require('config.inc.php');
			$config = new cal_config;

			$this->Host			= $config->databasehost;
			$this->Database	= $config->databasename;
			$this->User			= $config->databaseuser;
			$this->Password	= $config->databasepassword;
		}

		function TableName($var)
		{
			return $this->$var;
		}

		function RemoveJunkTags($oldtext)
		{
			// create the setup object
			$conf = new layout_setup;
			$retvalue = str_replace("'", "`", str_replace('"', '', str_replace('\\', '', strip_tags($oldtext, $conf->allowed_html_tags))));
			return $retvalue;
		}

		function CheckForEvents($checkday, $checkmonth, $checkyear)
		{
			$a = $this->tbl_bydate;
			$b = $this->tbl_recurring;

			$startofday = gmmktime(0, 0, 0, $checkmonth, $checkday, $checkyear);
			$endofday = gmmktime(23, 59, 59, $checkmonth, $checkday, $checkyear);
			$mstartofday = gmmktime(0, 0, 0, 7, $checkday, 1971);
			$mendofday = gmmktime(23, 59, 59, 7, $checkday, 1971);
			$ystartofday = gmmktime(0, 0, 0, $checkmonth, $checkday, 1972);
			$yendofday = gmmktime(23, 59, 59, $checkmonth, $checkday, 1972);

			$dayofweek = gmdate("w", $startofday);
			$weeknumber = intval($checkday / 7.1) + 1;

			$query  = "SELECT COUNT(*) FROM $a WHERE ($a.event_date >= $startofday AND $a.event_date <= $endofday) OR ";
			$query .= "($a.event_date >= $mstartofday AND $a.event_date <= $mendofday) OR ";
			$query .= "($a.event_date >= $ystartofday AND $a.event_date <= $yendofday)";

			$arr = $this->getrows($query);

			$hasid = 0;
			if (isset($arr[0]['COUNT(*)'] ))
				$hasid = $arr[0]['COUNT(*)'] ;

			if ($hasid > 0)
				return TRUE;

			$query  = "SELECT COUNT(*) FROM $b WHERE ($b.weekday = $dayofweek AND ($b.month = $checkmonth OR $b.month = 0) AND ";
			$query .= "($b.period = $weeknumber OR $b.period = 0))";

			$arr = $this->getrows($query);

			$hasid = 0;
			if (isset($arr[0]['COUNT(*)'] ))
				$hasid = $arr[0]['COUNT(*)'] ;

			if ($hasid > 0)
				return TRUE;
			return FALSE;
		}

		function GetCalendarDateEvents($checkday, $checkmonth, $checkyear)
		{
			$a = $this->tbl_bydate;

			$startofday = gmmktime(0, 0, 0, $checkmonth, $checkday, $checkyear);
			$endofday = gmmktime(23, 59, 59, $checkmonth, $checkday, $checkyear);
			$mstartofday = gmmktime(0, 0, 0, 7, $checkday, 1971);
			$mendofday = gmmktime(23, 59, 59, 7, $checkday, 1971);
			$ystartofday = gmmktime(0, 0, 0, $checkmonth, $checkday, 1972);
			$yendofday = gmmktime(23, 59, 59, $checkmonth, $checkday, 1972);

			$query  = "SELECT short_description FROM $a WHERE (event_date >= $startofday AND event_date <= $endofday) OR ";
			$query .= "(event_date >= $mstartofday AND event_date <= $mendofday) OR ";
			$query .= "(event_date >= $ystartofday AND event_date <= $yendofday) ORDER BY event_date";

			$this->query($query);
			return $this->affected_rows();
		}

		function GetCalendarDayEvents($checkday, $checkmonth, $checkyear)
		{
			$a = $this->tbl_recurring;

			$dayofweek = gmdate("w", gmmktime(0, 0, 0, $checkmonth, $checkday, $checkyear));
			$weeknumber = intval($checkday / 7.1) + 1;

			$query  = "SELECT short_description FROM $a WHERE weekday = $dayofweek AND ";
			$query .= "(month = $checkmonth OR month = 0) AND (period = $weeknumber OR period = 0) ORDER BY event_time";

			$this->query($query);
			return $this->affected_rows();
		}

		function GetScheduleDateEvents($checkday, $checkmonth, $checkyear)
		{
			$a = $this->tbl_bydate;

			$startofday = gmmktime(0, 0, 0, $checkmonth, $checkday, $checkyear);
			$endofday = gmmktime(23, 59, 59, $checkmonth, $checkday, $checkyear);
			$mstartofday = gmmktime(0, 0, 0, 7, $checkday, 1971);
			$mendofday = gmmktime(23, 59, 59, 7, $checkday, 1971);
			$ystartofday = gmmktime(0, 0, 0, $checkmonth, $checkday, 1972);
			$yendofday = gmmktime(23, 59, 59, $checkmonth, $checkday, 1972);

			$query  = "SELECT event_date, duration, short_description, long_description FROM $a WHERE ";
			$query .= "(event_date >= $startofday AND event_date <= $endofday) OR ";
			$query .= "(event_date >= $mstartofday AND event_date <= $mendofday) OR ";
			$query .= "(event_date >= $ystartofday AND event_date <= $yendofday) ORDER BY event_date";

			$this->query($query);
			return $this->affected_rows();
		}

		function GetScheduleDayEvents($checkday, $checkmonth, $checkyear)
		{
			$a = $this->tbl_recurring;

			$dayofweek = gmdate("w", gmmktime(0, 0, 0, $checkmonth, $checkday, $checkyear));
			$weeknumber = intval($checkday / 7.1) + 1;

			$query  = "SELECT event_time, duration, short_description, long_description FROM $a WHERE weekday = $dayofweek ";
			$query .= "AND (month = $checkmonth OR month = 0) AND (period = $weeknumber OR period = 0) ORDER BY event_time";

			$this->query($query);
			return $this->affected_rows();
		}

		function GetBetweenDates($startdate, $enddate)
		{
			$a = $this->tbl_bydate;

			$sday = gmdate("j", $startdate);
			$eday = gmdate("j", $enddate);
			$smonth = gmdate("n", $startdate);
			$emonth = gmdate("n", $enddate);
			$mdiff = $emonth - $smonth + ((gmdate("Y", $enddate) - gmdate("Y", $startdate)) * 12);
			if ($mdiff != 0)
			{
				$sday = 1;
				$eday = 31;
				if ($mdiff > 11)
				{
					$smonth = 1;
					$emonth = 12;
				}
				else
				{
					while (!checkdate($emonth, $eday, 1972) && $eday > 28)
					{
						$emonth--;
					}
				}
			}

			$mstartofday = gmmktime(0, 0, 0, 7, $sday, 1971);
			$mendofday = gmmktime(23, 59, 59, 7, $eday, 1971);
			$ystartofday = gmmktime(0, 0, 0, $smonth, $sday, 1972);
			$yendofday = gmmktime(23, 59, 59, $emonth, $eday, 1972);

			$query  = "SELECT * FROM $a WHERE ";
			$query .= "(event_date >= $startdate AND event_date <= $enddate) OR ";
			$query .= "(event_date >= $mstartofday AND event_date <= $mendofday) OR ";
			$query .= "(event_date >= $ystartofday AND event_date <= $yendofday) ORDER BY event_date";

			$this->query($query);
			return $this->affected_rows();
		}

		function GetAllRecurring()
		{
			$a = $this->tbl_recurring;
			$query = "SELECT * FROM $a";
			$this->query($query);
			return $this->affected_rows();
		}

		function GetSelfBetweenDates($userid, $startdate, $enddate)
		{
			$a = $this->tbl_bydate;

			$sday = gmdate("j", $startdate);
			$eday = gmdate("j", $enddate);
			$smonth = gmdate("n", $startdate);
			$emonth = gmdate("n", $enddate);
			$mdiff = $emonth - $smonth + ((gmdate("Y", $enddate) - gmdate("Y", $startdate)) * 12);
			if ($mdiff != 0)
			{
				$sday = 1;
				$eday = 31;
				if ($mdiff > 11)
				{
					$smonth = 1;
					$emonth = 12;
				}
				else
				{
					while (!checkdate($emonth, $eday, 1972) && $eday > 28)
					{
						$emonth--;
					}
				}
			}

			$mstartofday = gmmktime(0, 0, 0, 7, $sday, 1971);
			$mendofday = gmmktime(23, 59, 59, 7, $eday, 1971);
			$ystartofday = gmmktime(0, 0, 0, $smonth, $sday, 1972);
			$yendofday = gmmktime(23, 59, 59, $emonth, $eday, 1972);

			$query  = "SELECT * FROM $a WHERE userid = '$userid' AND ";
			$query .= "(event_date >= $startdate AND event_date <= $enddate) OR ";
			$query .= "(event_date >= $mstartofday AND event_date <= $mendofday) OR ";
			$query .= "(event_date >= $ystartofday AND event_date <= $yendofday) ORDER BY event_date";

			$this->query($query);
			return $this->affected_rows();
		}

		function GetSelfRecurring($userid)
		{
			$a = $this->tbl_recurring;
			$query = "SELECT * FROM $a WHERE userid = '$userid'";
			$this->query($query);
			return $this->affected_rows();
		}

		function AddByDate($evdate, $title, $detail, $duration, $userid)
		{
			$a = $this->tbl_bydate;
			$title = $this->RemoveJunkTags($title);
			$detail = $this->RemoveJunkTags($detail);
			$query  = "INSERT INTO $a (event_date, short_description, long_description, duration, userid) ";
			$query .= "VALUES ($evdate, '$title', '$detail', $duration, '$userid')";
			return $this->query($query);
		}

		function AddRecurring($weekday, $evtime, $period, $month, $title, $detail, $duration, $userid)
		{
			$a = $this->tbl_recurring;
			$title = $this->RemoveJunkTags($title);
			$detail = $this->RemoveJunkTags($detail);
			$query  = "INSERT INTO $a (weekday, event_time, period, month, short_description, long_description, duration, userid) ";
			$query .= "VALUES ($weekday, '$evtime', $period, $month, '$title', '$detail', $duration, '$userid')";
			return $this->query($query);
		}

		function DeleteDateEvent($id)
		{
			$a = $this->tbl_bydate;
			$query = "DELETE FROM $a WHERE id=$id";
			$this->query($query);
		}

		function DeleteDayEvent($id)
		{
			$a = $this->tbl_recurring;
			$query = "DELETE FROM $a WHERE id=$id";
			$this->query($query);
		}

		function SQLLogin($userid, $password)
		{
			$a = $this->tbl_users;
			$userid = $this->RemoveJunkTags($userid);
			$password = $this->RemoveJunkTags($password);
			$query = "SELECT userid, level FROM $a WHERE userid='$userid' AND password='$password'";

			$arr = Array();
			$arr = $this->getrows($query);
			if (isset($arr[0]))
				$retuser = $arr[0];
			else
				$retuser = false;

			return $retuser;
		}

		function GetUserData($userid)
		{
			$a = $this->tbl_users;
			$userid = $this->RemoveJunkTags($userid);
			$query = "SELECT userid, level FROM $a WHERE userid='$userid'";

			$arr = Array();
			$arr = $this->getrows($query);
			if (isset($arr[0]))
				$retuser = $arr[0];
			else
				$retuser = false;

			return $retuser;
		}

		function GetAllUsers()
		{
			$a = $this->tbl_users;
			$query = "SELECT userid, level FROM $a";
			$this->query($query);
			return $this->affected_rows();
		}

		function AddNewUser($accountid, $accesslevel, $accountpassword)
		{
			$a = $this->tbl_users;
			$accountid = $this->RemoveJunkTags($accountid);

			if (!$accountpassword)
				$query = "INSERT INTO $a (userid, level) VALUES ('$accountid', $accesslevel)";
			else
			{
				$accountpassword = $this->RemoveJunkTags($accountpassword);
				$query = "INSERT INTO $a (userid, password, level) VALUES ('$accountid', '$accountpassword', $accesslevel)";
			}
			return $this->query($query);
		}

		function UpdateUser($accountid, $existingid, $accesslevel, $accountpassword)
		{
			$a = $this->tbl_users;
			$accountid = $this->RemoveJunkTags($accountid);
			$existingid = $this->RemoveJunkTags($existingid);

			if (!$accountpassword)
				$query = "UPDATE $a SET userid='$accountid', level=$accesslevel WHERE userid='$existingid'";
			else
			{
				$accountpassword = $this->RemoveJunkTags($accountpassword);
				$query = "UPDATE $a SET userid='$accountid', password='$accountpassword', level=$accesslevel WHERE userid='$existingid'";
			}
			$this->query($query);
		}

		function DeleteUser($accountid)
		{
			$a = $this->tbl_users;
			$accountid = $this->RemoveJunkTags($accountid);

			$query = "DELETE FROM $a WHERE userid='$accountid'";
			$this->query($query);
		}
	}
}
?>
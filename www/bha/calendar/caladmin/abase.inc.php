<?
/*  ©2004 Proverbs, LLC. All rights reserved.  */

if (eregi("abase.inc.php", $_SERVER['PHP_SELF']))
{
	// redirect to calendar page
	header("Location: ../calendar.php");
	exit;
}

if(!defined("BASE_ADMINISTRATION")) 
{ 
	define("BASE_ADMINISTRATION", TRUE); 

	require ('../setup.inc.php');
	require ('../language.inc.php');
	require ('../calaccess.inc.php');

	class base_administration
	{
		var $pagelayout;
		var $userid;
		var $browser_version;

		// Constructor
		function base_administration()
		{
			$this->pagelayout = '';
			$this->userid = Array();
			$this->browser_version = 0;
		}

		function checkforid()
		{
			// create the setup object
			$conf = new layout_setup;
			$username = "";
			if ($conf->admin_serverside_login)
			{
				if(!isset($_SERVER['PHP_AUTH_USER']) || $_SERVER['PHP_AUTH_USER'] == "")
					return 0;
				else
					$username = strtolower($_SERVER['PHP_AUTH_USER']);
			}
			else
			{
				if (!isset($_COOKIE['usercookie']) || $_COOKIE['usercookie'] == "")
					return 1;
				else
					$username = strtolower($_COOKIE['usercookie']);
				header('P3P: CP="NOI NON ADMi OUR NOR UNI"');
				setcookie("usercookie", $username, time() + 800);
			}
			unset($conf);
			// create the database access object
			$accesscaldb = new caldbaccess;
			$this->userid = $accesscaldb->GetUserData($username);
			if ($this->userid == false)
				return 0;
			return 2;
		}

		function loginuser($username, $userpass)
		{
			// create the setup object
			$conf = new layout_setup;
			if ($conf->admin_serverside_login)
				return 0;
			unset($conf);
			// create the database access object
			$accesscaldb = new caldbaccess;
			$this->userid = $accesscaldb->SQLLogin(strtolower($username), $userpass);
			if ($this->userid == false)
				return 0;
			header('P3P: CP="NOI NON ADMi OUR NOR UNI"');
			setcookie("usercookie", strtolower($this->userid['userid']), time() + 800);
			return 2;	
		}

		function checkbrowser()
		{
			// create the setup object
			$conf = new layout_setup;
			$javaallowed = $conf->enable_javacss;
			unset($conf);
			if (!isset($_SERVER['HTTP_USER_AGENT']) || !$javaallowed)
				return 0;
			if (eregi("Opera", $_SERVER['HTTP_USER_AGENT']) || eregi("Firefox", $_SERVER['HTTP_USER_AGENT']))
				return 1;
			if (eregi("MSIE", $_SERVER['HTTP_USER_AGENT']))
			{
				if (intval(trim(substr($_SERVER['HTTP_USER_AGENT'], 4 + strpos($_SERVER['HTTP_USER_AGENT'], "MSIE"), 2))) >= 4)
					return 2;
			}
			if (eregi("Netscape", $_SERVER['HTTP_USER_AGENT']))
			{
				if (eregi("Netscape6", $_SERVER['HTTP_USER_AGENT']))
					return 1;
				if (intval(trim(substr($_SERVER['HTTP_USER_AGENT'], 9 + strpos($_SERVER['HTTP_USER_AGENT'], "Netscape"), 2))) >= 6)
					return 1;
			}
			return 0;
		}

		function ConvertBackJava($vartext)
		{
			$retvalue = str_replace("
", "\\n", $vartext);
			if (trim($retvalue) == "")
				$retvalue = '&nbsp;';
			return $retvalue;
		}

		function makeheader()
		{
			// create the setup object
			$conf = new layout_setup;
			// create the language object
			$lang = new language;

			$retversion = $this->checkbrowser();

			$this->pagelayout = '<?xml version="1.0" encoding="ISO-8859-1"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="'.$lang->lang_value.'" lang="'.$lang->lang_value.'">
<head>
<title>'.$conf->calendar_title_text.' '.$lang->word_administration.'</title>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1" />
<meta http-equiv="Content-Script-Type" content="text/javascript" />
<meta name="Description" content="'.$conf->calendar_title_text.' '.$lang->word_administration.'" />
';
			unset($conf);
			unset($lang);
			return $retversion;
		}

		function closeheader()
		{
			$this->pagelayout .= '</head>
<body leftmargin="0" topmargin="0" marginheight="0" marginwidth="0">
';
		}

		function displaypage()
		{
			$this->pagelayout .= '
</body>
</html>';

			echo $this->pagelayout;
		}

		function addcalendarevent()
		{
			// create the language object
			$lang = new language;

			$itemtitle = "";
			$itemdetails = "";
			$itemtype = "";
			if (isset($_POST['eventtitle']) && trim($_POST['eventtitle']) != "")
				$itemtitle = trim($_POST['eventtitle']);
			else
				return $lang->word_create_fail;
			if (isset($_POST['eventdetails']) && trim($_POST['eventdetails']) != "")
				$itemdetails = trim($_POST['eventdetails']);
			else
				$itemdetails = $itemtitle;
			if (isset($_POST['eventtype']) && trim($_POST['eventtype']) != "")
				$itemtype = $_POST['eventtype'];
			else
				return $lang->word_create_fail;
			$itemstart = "";
			$itemend = "";
			if (isset($_POST['starttime']) && $_POST['starttime'] != "")
				$itemstart = $_POST['starttime'];
			else
				return $lang->word_create_fail;
			if (isset($_POST['endtime']) && $_POST['endtime'] != "")
				$itemend = $_POST['endtime'];
			else
			{
				if ($itemstart != "00:00:25")
					return $lang->word_create_fail;
			}
			if ($itemstart == $itemend)
				return $lang->word_create_fail;
			$itemduration = 0;
			if ($itemstart != "00:00:25")
			{
				$startholder = strtotime($itemstart);
				if ($itemend == "00:00:00")
					$endholder = strtotime("23:59:59") + 1;
				else
					$endholder = strtotime($itemend);

				if ($endholder < $startholder)
				{
					$tempholder = $itemstart;
					$itemstart = $itemend;
					$itemend = $tempholder;
				}
				if ($endholder == $startholder)
					return $lang->word_create_fail;
				$itemduration = ($endholder - $startholder) / 3600;
				if ($itemduration < 0)
					$itemduration *= -1;
			}

			if ($itemtype == "date")
			{
				$itemmonth = 1;
				$itemday = 1;
				$itemyear = 1980;
				if (isset($_POST['datemonth']) && is_numeric($_POST['datemonth']))
					$itemmonth = $_POST['datemonth'];
				else
					return $lang->word_create_fail;
				if (isset($_POST['dateday']) && is_numeric($_POST['dateday']) && $_POST['dateday'] > 0 && $_POST['dateday'] <= 31)
					$itemday = $_POST['dateday'];
				else
					return $lang->word_create_fail;
				if (isset($_POST['dateyear']) && is_numeric($_POST['dateyear']))
					$itemyear = $_POST['dateyear'];
				else
					return $lang->word_create_fail;

				if ($itemmonth == 0)
				{
					$itemmonth = 7;
					$itemyear = 1971;
				}
				if (!checkdate($itemmonth, $itemday, $itemyear))
					return $lang->word_create_fail;

				$datetoadd = strtotime($itemyear.'-'.$itemmonth.'-'.$itemday.' '.$itemstart.' GMT');

				// create the database access object
				$accesscaldb = new caldbaccess;
				$isvalid = $accesscaldb->AddByDate($datetoadd, $itemtitle, $itemdetails, $itemduration, $this->userid['userid']);
			}
			else
			{
				$itemschedule = 0;
				$itemweekday = 0;
				$itemmonth = 1;
				if (isset($_POST['dayschedule']) && is_numeric($_POST['dayschedule']))
					$itemschedule = $_POST['dayschedule'];
				else
					return $lang->word_create_fail;
				if (isset($_POST['dayweekday']) && is_numeric($_POST['dayweekday']))
					$itemweekday = $_POST['dayweekday'];
				else
					return $lang->word_create_fail;
				if (isset($_POST['daymonth']) && is_numeric($_POST['daymonth']) && $_POST['daymonth'] >=0 && $_POST['daymonth'] < 13)
					$itemmonth = $_POST['daymonth'];
				else
					return $lang->word_create_fail;

				// create the database access object
				$accesscaldb = new caldbaccess;
				$isvalid = $accesscaldb->AddRecurring($itemweekday, $itemstart, $itemschedule, $itemmonth, $itemtitle, $itemdetails, $itemduration, $this->userid['userid']);
			}
			if (!$isvalid)
				return $lang->word_create_unknown;
			return $lang->word_create_ok;
		}

		function deletecalendarevent()
		{
			// create the language object
			$lang = new language;

			if (isset($_POST['eventid']))
			{
				// create the database access object
				$accesscaldb = new caldbaccess;
				$itemid = $_POST['eventid'];
				if (strstr($itemid, "D"))
				{
					$itemid = str_replace('D', '', $itemid);
					$accesscaldb->DeleteDateEvent($itemid);
				}
				else
				{
					if (strstr($itemid, "W"))
					{
						$itemid = str_replace('W', '', $itemid);
						$accesscaldb->DeleteDayEvent($itemid);
					}
					else
						return $lang->word_fail_select;
				}
			}
			else
				return $lang->word_fail_select;

			return $lang->word_delete_ok;
		}

		function updatecalendarevent()
		{
			// create the language object
			$lang = new language;

			$tempret = $this->addcalendarevent();
			if ($tempret != $lang->word_create_ok)
				return $tempret;
			$tempret = $this->deletecalendarevent();
			if ($tempret != $lang->word_delete_ok)
				return $tempret;

			return $lang->word_update_ok;
		}

		function addnewcalendaruser()
		{
			// create the language object
			$lang = new language;

			if (isset($_POST['useraccount']) and trim($_POST['useraccount']) != "")
			{
				$accountid = strtolower(trim($_POST['useraccount']));
				// create the setup object
				$conf = new layout_setup;
				if ((!isset($_POST['accountpassword']) && !$conf->admin_serverside_login) || !isset($_POST['accountlevel']))
					return $lang->word_create_fail;
				$accesslevel = $_POST['accountlevel'];
				$accesspass = FALSE;
				if (!$conf->admin_serverside_login)
					$accesspass = trim($_POST['accountpassword']);
				unset($conf);

				// create the database access object
				$accesscaldb = new caldbaccess;
				if ($accesscaldb->GetUserData($accountid))
					return $lang->word_fail_duplicate;
				if (!$accesscaldb->AddNewUser($accountid, $accesslevel, $accesspass))
					return $lang->word_create_unknown;
			}
			else
				return $lang->word_fail_nouser;

			return $lang->word_createuser_ok;
		}

		function updatecalendaruser()
		{
			// create the language object
			$lang = new language;

			if (isset($_POST['editaccount']) and trim($_POST['editaccount']) != "")
			{
				$existingid = strtolower(trim($_POST['editaccount']));
				// create the setup object
				$conf = new layout_setup;
				if ((!isset($_POST['accountpassword']) && !$conf->admin_serverside_login) || !isset($_POST['accountlevel']))
					return $lang->word_create_fail;
				$accesslevel = $_POST['accountlevel'];
				$accesspass = FALSE;
				if (!$conf->admin_serverside_login)
				{
					$accesspass = trim($_POST['accountpassword']);
					if ($accesspass == "XOX0XOX0XO")
						$accesspass = FALSE;					
				}
				unset($conf);
				if ($existingid == $this->userid['userid'] && $accesslevel < 2)
					return $lang->word_fail_selflower;
				$accountid = $existingid;
				// create the database access object
				$accesscaldb = new caldbaccess;
				if (isset($_POST['useraccount']) && trim($_POST['useraccount']) != "")
					$accoundid = $accesscaldb->RemoveJunkTags(strtolower(trim($_POST['useraccount'])));

				if ($accountid != $existingid && !$accesscaldb->GetUserData($accountid))
					return $lang->word_fail_duplicate;
				$accesscaldb->UpdateUser($accountid, $existingid, $accesslevel, $accesspass);
			}
			else
				return $lang->word_fail_nouser;

			return $lang->word_updateuser_ok;
		}

		function deletecalendaruser()
		{
			// create the language object
			$lang = new language;

			if (isset($_POST['editaccount']) and trim($_POST['editaccount']) != "")
			{
				// create the database access object
				$accesscaldb = new caldbaccess;
				$accountid = $accesscaldb->RemoveJunkTags(strtolower(trim($_POST['editaccount'])));
				if ($accountid == $this->userid['userid'])
					return $lang->word_fail_selfdelete;

				$accesscaldb->DeleteUser($accountid);
			}
			else
				return $lang->word_fail_nouser;

			return $lang->word_deleteuser_ok;
		}
	}
}
?>
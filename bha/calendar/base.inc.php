<?
/*  ©2004 Proverbs, LLC. All rights reserved.  */

if (eregi("base.inc.php", $_SERVER['PHP_SELF']))
{
	// redirect to calendar page
	header("Location: calendar.php");
	exit;
}

if(!defined("BASE_CALENDAR")) 
{ 
	define("BASE_CALENDAR", TRUE); 

	require ('setup.inc.php');
	require ('language.inc.php');
	require ('calaccess.inc.php');

	class base_calendar
	{
		var $pagelayout;
		var $activeday;
		var $activemonth;
		var $activeyear;

		// Constructor
		function base_calendar()
		{
			$this->pagelayout = '';
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
<title>'.$conf->calendar_title_text.'</title>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1" />
<meta http-equiv="Content-Script-Type" content="text/javascript" />
<meta name="Description" content="'.$conf->calendar_title_text.'" />
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
	}
}
?>
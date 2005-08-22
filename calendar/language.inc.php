<?
/*  ©2004 Proverbs, LLC. All rights reserved.  */

if (eregi("language.inc.php", $_SERVER['PHP_SELF']))
{
	// redirect to calendar page
	header("Location: calendar.php");
	exit;
}

if(!defined("USED_LANGUAGE")) 
{
	define("USED_LANGUAGE", TRUE); 

	require ('setup.inc.php');

	// create the setup object
	$conf = new layout_setup;

	switch (strtolower($conf->display_language))
	{
		case "dutch":
			require('dutch.lng.php'); 
			break;
		case "english":
			require('english.lng.php'); 
			break;
		case "french":
			require('french.lng.php'); 
			break;
		case "german":
			require('german.lng.php'); 
			break;
		case "italian":
			require('italian.lng.php'); 
			break;
		case "spanish":
			require('spanish.lng.php'); 
			break;
		default:
			require('english.lng.php');
	}

	class language extends languageset
	{
		// Constructor
		function language()
		{
			$this->languageset();
		}
	}
}
?>
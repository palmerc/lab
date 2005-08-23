<?
/*  ©2004 Proverbs, LLC. All rights reserved.  */

if (eregi("baselang.inc.php", $_SERVER['PHP_SELF']))
{
	// redirect to calendar page
	header("Location: calendar.php");
	exit;
}

if(!defined("BASE_LANGUAGE")) 
{
	define("BASE_LANGUAGE", TRUE); 

	class baselanguage
	{
		var $lang_value;
		var $day_long;
		var $day_short;
		var $day_init;
		var $month_long;
		var $month_short;
		var $word_today_date;
		var $word_day;
		var $word_month;
		var $word_year;
		var $word_all_day;
		var $word_no_javascript;
		var $word_administration;
		var $word_full;
		var $word_author;
		var $word_submit;
		var $word_refresh;
		var $word_more;
		var $word_username;
		var $word_password;
		var $word_login;
		var $word_access_denied;
		var $word_calendar_administration;
		var $word_user_admin;
		var $word_user_administration;
		var $word_access_level;
		var $word_events;
		var $word_event_title;
		var $word_event_details;
		var $word_start_time;
		var $word_end_time;
		var $word_event_type;
		var $word_date;
		var $word_all;
		var $word_weekday;
		var $word_every;
		var $word_of;
		var $word_all_months;
		var $word_show_events;
		var $word_show_weekday_events;
		var $word_create_event;
		var $word_delete_event;
		var $word_update_event;
		var $word_create_ok;
		var $word_update_ok;
		var $word_delete_ok;
		var $word_fail_select;
		var $word_create_fail;
		var $word_create_unknown;
		var $word_existing_events;
		var $word_create_user;
		var $word_delete_user;
		var $word_update_user;
		var $word_existing_users;
		var $word_fail_nouser;
		var $word_fail_duplicate;
		var $word_fail_selflower;
		var $word_fail_selfdelete;
		var $word_createuser_ok;
		var $word_deleteuser_ok;
		var $word_updateuser_ok;
		var $word_emptyfield;

		// Constructor
		function baselanguage()
		{
		}
	}
}
?>
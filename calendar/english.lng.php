<?
/*  ©2004 Proverbs, LLC. All rights reserved.  */

if (eregi("english.lng.php", $_SERVER['PHP_SELF']))
{
	// redirect to calendar page
	header("Location: calendar.php");
	exit;
}

if(!defined("ENGLISH_LANGUAGE")) 
{
	define("ENGLISH_LANGUAGE", TRUE); 

	require ('baselang.inc.php');

	class languageset extends baselanguage
	{
		// Constructor
		function languageset()
		{
			$this->lang_value = "en";
			$this->day_long = Array('Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday');
			$this->day_short = Array('Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat');
			$this->day_init = Array('S', 'M', 'T', 'W', 'T', 'F', 'S');
			$this->month_long = Array(1 => 'January', 'February', 'March', 'April', 'May', 'June', 'July', 
				'August', 'September', 'October', 'November', 'December');
			$this->month_short = Array(1 => 'Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 
				'Nov', 'Dec');
			$this->word_today_date = "Today's Date";
			$this->word_day = "Day";
			$this->word_month = "Month";
			$this->word_year = "Year";
			$this->word_all_day = "All Day";
			$this->word_no_javascript = "This calendar will only function correctly with JavaScript enabled";
			$this->word_administration = "Administration";
			$this->word_full = "Full";
			$this->word_author = "Author";
			$this->word_submit = "Submit";
			$this->word_refresh = "Refresh";
			$this->word_more = "more";
			$this->word_username = "Username";
			$this->word_password = "Password";
			$this->word_login = "Login";
			$this->word_access_denied = "Access Denied";
			$this->word_calendar_administration = "Calendar Administration";
			$this->word_user_admin = "User Admin";
			$this->word_user_administration = "User Administration";
			$this->word_access_level = "Access Level";
			$this->word_events = "Events";
			$this->word_event_title = "Event Title";
			$this->word_event_details = "Event Details";
			$this->word_start_time = "Start Time";
			$this->word_end_time = "End Time";
			$this->word_event_type = "Event Type";
			$this->word_date = "Date";
			$this->word_all = "All";
			$this->word_weekday = "Weekday";
			$this->word_every = "Every";
			$this->word_of = "of";
			$this->word_all_months = "All Months";
			$this->word_show_events = "Show Events Between";
			$this->word_show_weekday_events = "Show Weekday Events";
			$this->word_create_event = "Create Event";
			$this->word_delete_event = "Delete Event";
			$this->word_update_event = "Update Event";
			$this->word_create_ok = "Calendar event created successfully";
			$this->word_update_ok = "Calendar event updated successfully";
			$this->word_delete_ok = "Calendar event has been removed";
			$this->word_fail_select = "FAILED: No event selected";
			$this->word_create_fail = "FAILED: A required field has been left empty or is invalid";
			$this->word_create_unknown = "FAILED: An unknown error has occurred";
			$this->word_existing_events = "Existing Events";
			$this->word_create_user = "Create User";
			$this->word_delete_user = "Delete User";
			$this->word_update_user = "Update User";
			$this->word_existing_users = "Existing Accounts";
			$this->word_fail_nouser = "FAILED: User account name is empty or invalid";
			$this->word_fail_duplicate = "FAILED: A user account with that name exists";
			$this->word_fail_selflower = "FAILED: You can not lower your own access level";
			$this->word_fail_selfdelete = "FAILED: You can not remove your own account";
			$this->word_createuser_ok = "User account created successfully";
			$this->word_deleteuser_ok = "User account has been removed";
			$this->word_updateuser_ok = "User account updated successfully";
			$this->word_emptyfield = "A required field is empty or invalid";
		}
	}
}
?>
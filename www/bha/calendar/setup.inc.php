<?
/*  ©2004 Proverbs, LLC. All rights reserved.  */

if (eregi("setup.inc.php", $_SERVER['PHP_SELF']))
{
	// redirect to calendar page
	header("Location: calendar.php");
	exit;
}

if(!defined("LAYOUT_SETUP")) 
{
	define("LAYOUT_SETUP", TRUE);

	class layout_setup
	{
		// Image fields can be entered as a complete http address or relative to the calendar directory

		/* Title Options */
		// Image fields left blank will be replaced with the corresponding text field
		var $calendar_title_text = 'Proverbs Web Calendar 2.0';
		var $calendar_title_image = '';

		/* Display Options */
		// All colors must be in hexadecimal format
		// Image fields left blank will not be used
		var $html_body_background_color = 'FFFFFF';
		var $html_body_background_image = '';
		var $html_body_text_color = '000000';
		var $calendar_title_background_color = 'ECECEC';
		var $calendar_title_background_image = '';
		var $calendar_title_text_color = '000000';
		var $calendar_border_color = 'AAAAAA';
		var $calendar_day_background_color = 'ECECEC';
		var $calendar_today_background_color = 'FFFFFF';
		var $calendar_day_bgmouseover_color = 'C0C0C0';
		var $calendar_day_text_color = '000000';
		var $schedule_one_background_color = 'ECECEC';
		var $schedule_two_background_color = 'C0C0C0';
		var $schedule_border_color = '000000';
		var $schedule_text_color = '000000';
		var $schedule_time_background_color = 'AAAAAA';
		var $schedule_time_text_color = '000000';

		// The following fields are only used with CSS2 and JavaScript enabled browsers
		var $event_background_color = 'FFFFFF';
		var $event_border_color = '000000';
		var $event_text_color = '000000';
		var $mini_calendar_background_color = 'FFFFFF';
		var $mini_calendar_bgactive_color = 'C0C0C0';
		var $mini_calendar_text_color = '000000';
		var $mini_calendar_border_color = '000000';

		/* Language Selection */
		// Sets the language used for built in calendar text.
		// Available options: 'dutch', 'english', 'french', 'german', 'italian', 'spanish'
		var $display_language = 'english';

		/* Configuration Options */
		// Sets the displayed time zone for event items.
		var $time_zone = '';

		// Offsets the calendar date/time by the number of hours entered from GMT.
		// For example, a setting of -5 equates to EST US & Canada.
		var $time_adjustment = -5;

		// Sets whether the calendar will adjust for daylight savings time.
		// true = adjust for daylight savings time, false = do not adjust
		var $use_dst = true;

		// Sets the time format used for entering and displaying event items.
		// Available options: '12', '24'
		var $time_format = '12';

		// Sets the hour for the schedule page to scroll to on load.  This variable only applies
		// to the Internet Explorer version of schedule.  Set this variable to an integer value in
		// 24 hour format.  Use -1 to not scroll the schedule.
		var $schedule_scroll_time = 8;

		// Sets the starting day of the week for calendar display.
		// 0 = Sunday, 1 = Monday, 2 = Tuesday, 3 = Wednesday, 4 = Thursday, 5 = Friday, 6 = Saturday
		var $calendar_start_day = 0;

		// Sets the HTML tags allowed to be used for event details
		var $allowed_html_tags = '<b><i><u>';

		// Sets whether Javascript and CSS will be enabled for the calendar
		// true = Allow Javascript and CSS, false = Turns off Javascript and CSS
		var $enable_javacss = true;

		// Sets whether the hyperlink to the calendar administration page is displayed
		// on the calendar/schedule.  true = display the link, false = hide the link
		var $show_admin_link = true;

		// Sets the type of authentification used to login to calendar administration. 
		// true = use server side login (HTACCESS or NT User Authentification), false =  use 
		// sql authentification with cookies.
		var $admin_serverside_login = false;

		function layout_setup()
		{
		}
	}
}
?>
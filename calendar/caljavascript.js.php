<?
/*  ©2004 Proverbs, LLC. All rights reserved.  */

header('Content-type: text/javascript');

require ('setup.inc.php');

// create the setup object
$conf = new layout_setup;

echo '
function tdmv(aItem)
{
	aItem.style.backgroundColor = \'#'.$conf->calendar_day_bgmouseover_color.'\';
}

function tdmu(aItem)
{
	aItem.style.backgroundColor = \'#'.$conf->calendar_day_background_color.'\';
}

function tdtmo(aItem)
{
	aItem.style.backgroundColor = \'#'.$conf->calendar_today_background_color.'\';	
}

function ensureRefresh()
{
	document.calendarPage.day.value = "";
	document.calendarPage.view.value = "calendar";
	document.calendarPage.submit();
}

function sendDay(schedDay, schedMonth, schedYear)
{
	document.calendarPage.day.value = schedDay;
	document.calendarPage.month.value = schedMonth;
	document.calendarPage.year.value = schedYear;
	document.calendarPage.view.value = "schedule";
	document.calendarPage.submit();
}
';
?>
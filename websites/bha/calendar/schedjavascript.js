/*  ©2004 Proverbs, LLC. All rights reserved.  */

function showCalendar(calMonth, calYear)
{
	document.schedulePage.day.value = "";
	document.schedulePage.month.value = calMonth;
	document.schedulePage.year.value = calYear;
	document.schedulePage.view.value = "calendar";
	document.schedulePage.submit();
}

function showSchedule(schedDay, schedMonth, schedYear)
{
	document.schedulePage.day.value = schedDay;
	document.schedulePage.month.value = schedMonth;
	document.schedulePage.year.value = schedYear;
	document.schedulePage.view.value = "schedule";
	document.schedulePage.submit();
}

function setEventDetails(scheduleEvent)
{
	document.all.showEvent.innerHTML = scheduleEvent;	
}
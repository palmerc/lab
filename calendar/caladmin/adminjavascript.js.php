<?
/*  ©2004 Proverbs, LLC. All rights reserved.  */

header('Content-type: text/javascript');

require ('../language.inc.php');

// create the language object
$lang = new language;

echo '
function sed(eTitle, eDetails, eSTime, eETime, eMonth, eDay, eYear)
{
	en(true);
	document.calAdmin.eventtype[0].checked = true;
	document.calAdmin.eventtitle.value = eTitle;
	document.calAdmin.eventdetails.value = eDetails;
	document.calAdmin.starttime.value = eSTime;
	document.calAdmin.endtime.value = eETime;
	document.calAdmin.datemonth.value = eMonth;
	document.calAdmin.dateday.value = eDay;
	document.calAdmin.dateyear.value = eYear;
}

function sew(eTitle, eDetails, eSTime, eETime, eDSched, eDDay, eDMonth)
{
	en(false);
	document.calAdmin.eventtype[1].checked = true;
	document.calAdmin.eventtitle.value = eTitle;
	document.calAdmin.eventdetails.value = eDetails;
	document.calAdmin.starttime.value = eSTime;
	document.calAdmin.endtime.value = eETime;
	document.calAdmin.dayschedule.value = eDSched;
	document.calAdmin.dayweekday.value = eDDay;
	document.calAdmin.daymonth.value = eDMonth;
}

function en(dt)
{
	document.calAdmin.datemonth.disabled = !dt;
	document.calAdmin.dateday.disabled = !dt;
	document.calAdmin.dateyear.disabled = !dt;
	document.calAdmin.dayschedule.disabled = dt;
	document.calAdmin.dayweekday.disabled = dt;
	document.calAdmin.daymonth.disabled = dt;
}

function val()
{
	if (document.calAdmin.eventtitle.value == "")
	{
		alert("'.$lang->word_emptyfield.'");
		document.calAdmin.eventtitle.focus();
		return false;
	}

	if (document.calAdmin.starttime.value == "")
	{
		alert("'.$lang->word_emptyfield.'");
		document.calAdmin.starttime.focus();
		return false;
	}

	if (document.calAdmin.starttime.value != "00:00:25" && document.calAdmin.endtime.value == "")
	{
		alert("'.$lang->word_emptyfield.'");
		document.calAdmin.endtime.focus();
		return false;
	}

	if (document.calAdmin.starttime.value == document.calAdmin.endtime.value)
	{
		alert("'.$lang->word_emptyfield.'");
		document.calAdmin.endtime.focus();
		return false;
	}

	if (document.calAdmin.eventtype.value == "")
	{
		alert("'.$lang->word_emptyfield.'");
		document.calAdmin.eventtype.focus();
		return false;
	}

	if (document.calAdmin.dayschedule.disabled)
	{
		if (document.calAdmin.datemonth.value == "")
		{
			alert("'.$lang->word_emptyfield.'");
			document.calAdmin.datemonth.focus();
			return false;
		}
		if (document.calAdmin.dateday.value == "")
		{
			alert("'.$lang->word_emptyfield.'");
			document.calAdmin.dateday.focus();
			return false;
		}
		if (document.calAdmin.dateyear.value == "")
		{
			alert("'.$lang->word_emptyfield.'");
			document.calAdmin.dateyear.focus();
			return false;
		}
	}
	else
	{
		if (document.calAdmin.dayschedule.value == "")
		{
			alert("'.$lang->word_emptyfield.'");
			document.calAdmin.dayschedule.focus();
			return false;
		}
		if (document.calAdmin.dayweekday.value == "")
		{
			alert("'.$lang->word_emptyfield.'");
			document.calAdmin.dayweekday.focus();
			return false;
		}
		if (document.calAdmin.daymonth.value == "")
		{
			alert("'.$lang->word_emptyfield.'");
			document.calAdmin.daymonth.focus();
			return false;
		}
	}

	return true;
}
';
?>
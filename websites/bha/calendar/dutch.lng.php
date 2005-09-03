<?
/*  ©2004 Proverbs, LLC. All rights reserved.  */

if (eregi("dutch.lng.php", $_SERVER['PHP_SELF']))
{
	// redirect to calendar page
	header("Location: calendar.php");
	exit;
}

if(!defined("DUTCH_LANGUAGE")) 
{
	define("DUTCH_LANGUAGE", TRUE); 

	require ('baselang.inc.php');

	class languageset extends baselanguage
	{
		// Constructor
		function languageset()
		{
			$this->lang_value = "nl";
			$this->day_long = Array('Zondag', 'Maandag', 'Dinsdag', 'Woensdag', 'Donderdag', 'Vrijdag', 'Zaterdag');
			$this->day_short = Array('Zon', 'Ma', 'Din', 'Woe', 'Don', 'Vri', 'Zat');
			$this->day_init = Array('Z', 'M', 'D', 'W', 'D', 'V', 'Z');
			$this->month_long = Array(1 => 'Januari', 'Februari', 'Maart', 'April', 'Mei', 'Juni', 'Juli', 
				'Augustus', 'September', 'Oktober', 'November', 'December');
			$this->month_short = Array(1 => 'Jan', 'Feb', 'Maa', 'Apr', 'Mei', 'Jun', 'Jul', 'Aug', 'Sep', 'Okt', 
				'Nov', 'Dec');
			$this->word_today_date = "Vandaag Datum";
			$this->word_day = "Dag";
			$this->word_month = "Maand";
			$this->word_year = "Jaar";
			$this->word_all_day = "Al Dag";
			$this->word_no_javascript = "Deze kalender zal slechts correct met toegelaten JavaScript functioneren";
			$this->word_administration = "Beleid";
			$this->word_full = "Hoogtepunt";
			$this->word_author = "Auteur";
			$this->word_submit = "Verzoek";
			$this->word_refresh = "Verfris me";
			$this->word_more = "meer";
			$this->word_username = "Gebruikersbenaming";
			$this->word_password = "Wachtwoord";
			$this->word_login = "Login";
			$this->word_access_denied = "Ontkende toegang";
			$this->word_calendar_administration = "het Beleid van de Kalender";
			$this->word_user_admin = "Gebruiker Beleid";
			$this->word_user_administration = "het Beleid van de Gebruiker";
			$this->word_access_level = "het Niveau van de Toegang";
			$this->word_events = "Gebeurtenissen";
			$this->word_event_title = "De Titel van de gebeurtenis";
			$this->word_event_details = "De Details van de gebeurtenis";
			$this->word_start_time = "De Tijd van het begin";
			$this->word_end_time = "De Tijd van het eind";
			$this->word_event_type = "Het Type van gebeurtenis";
			$this->word_date = "Datum";
			$this->word_all = "Allen";
			$this->word_weekday = "Weekdag";
			$this->word_every = "Elk";
			$this->word_of = "van";
			$this->word_all_months = "Alle Maanden";
			$this->word_show_events = "Toon Gebeurtenissen tussen";
			$this->word_show_weekday_events = "Toon de Gebeurtenissen van de Weekdag";
			$this->word_create_event = "Creëer Gebeurtenis";
			$this->word_delete_event = "Schrap Gebeurtenis";
			$this->word_update_event = "Verandering Gebeurtenis";
			$this->word_create_ok = "Met succes gecreeerde de gebeurtenis van de kalender";
			$this->word_update_ok = "Met succes bijgewerkte de gebeurtenis van de kalender";
			$this->word_delete_ok = "De gebeurtenis van de kalender is verwijderd";
			$this->word_fail_select = "ONTBROKEN: Geen geselecteerde gebeurtenis";
			$this->word_create_fail = "ONTBROKEN: Een vereist gebied is verlaten leeg of geweest ongeldig";
			$this->word_create_unknown = "ONTBROKEN: Een onbekende fout is voorgekomen";
			$this->word_existing_events = "Bestaande Gebeurtenissen";
			$this->word_create_user = "Creëer Gebruiker";
			$this->word_delete_user = "Schrap Gebruiker";
			$this->word_update_user = "Verandering Gebruiker";
			$this->word_existing_users = "Bestaande Rekeningen";
			$this->word_fail_nouser = "ONTBROKEN: De naam van de rekening van de gebruiker is leeg of ongeldig";
			$this->word_fail_duplicate = "ONTBROKEN: Dubbele naam van de rekening";
			$this->word_fail_selflower = "ONTBROKEN: U kunt niet uw eigen toegangsniveau verminderen";
			$this->word_fail_selfdelete = "ONTBROKEN: U kunt niet uw eigen rekening verwijderen";
			$this->word_createuser_ok = "Met succes gecreeerde de rekening van de gebruiker";
			$this->word_deleteuser_ok = "De rekening van de gebruiker is verwijderd";
			$this->word_updateuser_ok = "Met succes bijgewerkte de rekening van de gebruiker";
			$this->word_emptyfield = "Een vereist gebied is leeg of ongeldig";
		}
	}
}
?>
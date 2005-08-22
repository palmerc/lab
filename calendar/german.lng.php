<?
/*  ©2003 Proverbs, LLC. All rights reserved.  */

if (eregi("german.lng.php", $_SERVER['PHP_SELF']))
{
	// redirect to calendar page
	header("Location: calendar.php");
	exit;
}

if(!defined("GERMAN_LANGUAGE")) 
{
	define("GERMAN_LANGUAGE", TRUE); 

	require ('baselang.inc.php');

	class languageset extends baselanguage
	{
		// Constructor
		function languageset()
		{
			$this->lang_value = "de";
			$this->day_long = Array('Sonntag', 'Montag', 'Dienstag', 'Mittwoch', 'Donnerstag', 'Freitag', 'Samstag');
			$this->day_short = Array('Son', 'Mon', 'Die', 'Mitt', 'Don', 'Fre', 'Sam');
			$this->day_init = Array('S', 'M', 'D', 'M', 'D', 'F', 'S');
			$this->month_long = Array(1 => 'Januar', 'Februar', 'März', 'April', 'Mai', 'Juni', 'Juli', 
				'August', 'September', 'Oktober', 'November', 'Dezember');
			$this->month_short = Array(1 => 'Jan', 'Feb', 'Mär', 'Apr', 'Mai', 'Jun', 'Jul', 'Aug', 'Sep', 'Okt', 
				'Nov', 'Dez');
			$this->word_today_date = "Tag-Datum";
			$this->word_day = "Tag";
			$this->word_month = "Monat";
			$this->word_year = "Jahr";
			$this->word_all_day = "Aller Tag";
			$this->word_no_javascript = "Dieser Kalender arbeitet nur richtig mit dem ermöglichten Javascript";
			$this->word_administration = "Leitung";
			$this->word_full = "Voll";
			$this->word_author = "Autor";
			$this->word_submit = "Antrag";
			$this->word_refresh = "Erneuern Sie";
			$this->word_more = "mehr";
			$this->word_username = "Benutzername";
			$this->word_password = "Kennwort";
			$this->word_login = "LOGON";
			$this->word_access_denied = "Machen Sie Verweigert";
			$this->word_calendar_administration = "Kalender-Leitung";
			$this->word_user_admin = "Benutzer-Leitung";
			$this->word_user_administration = "Benutzer-Leitung";
			$this->word_access_level = "Zugriffsebene Zugänglich";
			$this->word_events = "Fälle";
			$this->word_event_title = "Fall-Titel";
			$this->word_event_details = "Fall-Details";
			$this->word_start_time = "Anlaßzeit";
			$this->word_end_time = "Ende Zeit";
			$this->word_event_type = "Fall-Art";
			$this->word_date = "Datum";
			$this->word_all = "Alle";
			$this->word_weekday = "Wochentag";
			$this->word_every = "Jedes";
			$this->word_of = "von";
			$this->word_all_months = "Alle Monate";
			$this->word_show_events = "Zeigen Sie Fälle Zwischen";
			$this->word_show_weekday_events = "Zeigen Sie Wochentag-Fälle";
			$this->word_create_event = "Bilden Sie Fall";
			$this->word_delete_event = "Löschung-Fall";
			$this->word_update_event = "Ändern Sie Fall";
			$this->word_create_ok = "Kalenderfall verursachte erfolgreich";
			$this->word_update_ok = "Kalenderfall erfolgreich aktualisiert";
			$this->word_delete_ok = "Kalenderfall ist entfernt worden";
			$this->word_fail_select = "AUSGEFALLEN: Kein Fall wählte vor";
			$this->word_create_fail = "AUSGEFALLEN: Erfordert fangen ist verlassen worden leer oder ist unzulässig auf";
			$this->word_create_unknown = "AUSGEFALLEN: Eine unbekannte Störung ist aufgetreten";
			$this->word_existing_events = "Vorhandene Fälle";
			$this->word_create_user = "Bilden Sie Benutzer";
			$this->word_delete_user = "Löschung-Benutzer";
			$this->word_update_user = "Änderung Benutzer";
			$this->word_existing_users = "Vorhandene Konten";
			$this->word_fail_nouser = "AUSGEFALLEN: Benutzerkontoname ist leer oder Invalides";
			$this->word_fail_duplicate = "AUSGEFALLEN: Doppelter Kontoname";
			$this->word_fail_selflower = "AUSGEFALLEN: Sie können nicht Ihre eigene Zugriffsebene senken";
			$this->word_fail_selfdelete = "AUSGEFALLEN: Sie können nicht Ihr eigenes Konto entfernen";
			$this->word_createuser_ok = "Benutzerkonto erfolgreich verursacht";
			$this->word_deleteuser_ok = "Benutzerkonto ist entfernt worden";
			$this->word_updateuser_ok = "Benutzerkonto erfolgreich aktualisiert";
			$this->word_emptyfield = "Erfordert fangen ist leer oder Invalides auf";
		}
	}
}
?>
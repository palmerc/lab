<?
/*  ©2003 Proverbs, LLC. All rights reserved.  */

if (eregi("italian.lng.php", $_SERVER['PHP_SELF']))
{
	// redirect to calendar page
	header("Location: calendar.php");
	exit;
}

if(!defined("ITALIAN_LANGUAGE")) 
{
	define("ITALIAN_LANGUAGE", TRUE); 

	require ('baselang.inc.php');

	class languageset extends baselanguage
	{
		// Constructor
		function languageset()
		{
			$this->lang_value = "it";
			$this->day_long = Array('Domenica', 'Lunedì', 'Martedì', 'Mercoledì', 'Giovedì', 'Venerdì', 'Sabato');
			$this->day_short = Array('Dom', 'Lun', 'Mar', 'Mer', 'Gio', 'Ven', 'Sab');
			$this->day_init = Array('D', 'L', 'M', 'M', 'G', 'V', 'S');
			$this->month_long = Array(1 => 'Gennaio', 'Febbraio', 'Marzo', 'Aprile', 'Maggio', 'Giugno', 'Luglio', 
				'Agosto', 'Settembre', 'Ottobre', 'Novembre', 'Dicembre');
			$this->month_short = Array(1 => 'Gen', 'Feb', 'Mar', 'Apr', 'Mag', 'Giu', 'Lug', 'Ago', 'Set', 'Ott', 
				'Nov', 'Dic');
			$this->word_today_date = "Data Di Oggi";
			$this->word_day = "Giorno";
			$this->word_month = "Mese";
			$this->word_year = "Anno";
			$this->word_all_day = "Tutto il giorno";
			$this->word_no_javascript = "Questo calendario funzionerà soltanto correttamente con il Javascript permesso";
			$this->word_administration = "Gestione";
			$this->word_full = "In pieno";
			$this->word_author = "Autore";
			$this->word_submit = "Presenti";
			$this->word_refresh = "Rinfreschi";
			$this->word_more = "più";
			$this->word_username = "Nome dell'utente";
			$this->word_password = "Parola D'Accesso";
			$this->word_login = "Inizio Attività";
			$this->word_access_denied = "Accedi a Negato";
			$this->word_calendar_administration = "Gestione Del Calendario";
			$this->word_user_admin = "Utente Gestione";
			$this->word_user_administration = "La Gestione Dell'Utente";
			$this->word_access_level = "Livello Di Accesso";
			$this->word_events = "Eventi";
			$this->word_event_title = "Titolo Di Evento";
			$this->word_event_details = "Particolari Di Evento";
			$this->word_start_time = "Tempo Di Inizio";
			$this->word_end_time = "Tempo Di Conclusione";
			$this->word_event_type = "Tipo Di Evento";
			$this->word_date = "Data";
			$this->word_all = "Tutti";
			$this->word_weekday = "Giorno della settimana";
			$this->word_every = "Ogni";
			$this->word_of = "di";
			$this->word_all_months = "Tutti i Mesi";
			$this->word_show_events = "Mostri Gli Eventi In mezzo";
			$this->word_show_weekday_events = "Mostri Gli Eventi Di Giorno della settimana";
			$this->word_create_event = "Generi L'Evento";
			$this->word_delete_event = "Rimuova L'Evento";
			$this->word_update_event = "Cambi L'Evento";
			$this->word_create_ok = "L'evento del calendario ha generato con successo";
			$this->word_update_ok = "Evento del calendario aggiornato con successo";
			$this->word_delete_ok = "L'evento del calendario è stato rimosso";
			$this->word_fail_select = "VENUTO A MANCARE: Nessun evento ha selezionato";
			$this->word_create_fail = "VENUTO A MANCARE: Un campo richiesto è stato lasciato vuoto o è non valido";
			$this->word_create_unknown = "VENUTO A MANCARE: Un errore sconosciuto ha accaduto";
			$this->word_existing_events = "Eventi Attuali";
			$this->word_create_user = "Generi Il Cliente";
			$this->word_delete_user = "Rimuova Il Cliente";
			$this->word_update_user = "Cambi Il Cliente";
			$this->word_existing_users = "Clienti Attuali";
			$this->word_fail_nouser = "VENUTO A MANCARE: Il nome di cliente dell'utente è vuoto o invalid";
			$this->word_fail_duplicate = "VENUTO A MANCARE: Nome di cliente duplicato";
			$this->word_fail_selflower = "VENUTO A MANCARE: Non potete abbassare il vostro proprio livello di accesso";
			$this->word_fail_selfdelete = "VENUTO A MANCARE: Non potete rimuovere il vostro proprio cliente";
			$this->word_createuser_ok = "Cliente dell'utente generato con successo";
			$this->word_deleteuser_ok = "Il cliente dell'utente è stato rimosso";
			$this->word_updateuser_ok = "Il cliente dell'utente ha aggiornato con successo";
			$this->word_emptyfield = "Un campo richiesto è vuoto o invalid";
		}
	}
}
?>
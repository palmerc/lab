<?
/*  ©2003 Proverbs, LLC. All rights reserved.  */

if (eregi("french.lng.php", $_SERVER['PHP_SELF']))
{
	// redirect to calendar page
	header("Location: calendar.php");
	exit;
}

if(!defined("FRENCH_LANGUAGE")) 
{
	define("FRENCH_LANGUAGE", TRUE); 

	require ('baselang.inc.php');

	class languageset extends baselanguage
	{
		// Constructor
		function languageset()
		{
			$this->lang_value = "fr";
			$this->day_long = Array('Dimanche', 'Lundi', 'Mardi', 'Mercredi', 'Jeudi', 'Vendredi', 'Samedi');
			$this->day_short = Array('Dim', 'Lun', 'Mar', 'Mer', 'Jeu', 'Ven', 'Sam');
			$this->day_init = Array('D', 'L', 'M', 'M', 'J', 'V', 'S');
			$this->month_long = Array(1 => 'Janvier', 'Février', 'Mars', 'Avril', 'Mai', 'Juin', 'Juillet', 
				'Août', 'Septembre', 'Octobre', 'Novembre', 'Décembre');
			$this->month_short = Array(1 => 'Jan', 'Fév', 'Mar', 'Avr', 'Mai', 'Juin', 'Juil', 'Août', 'Sep', 'Oct', 
				'Nov', 'Déc');
			$this->word_today_date = "Aujourd'hui est";
			$this->word_day = "Jour";
			$this->word_month = "Mois";
			$this->word_year = "Année";
			$this->word_all_day = "Toute la journée";
			$this->word_no_javascript = "Ce calendrier fonctionnera seulement correctement avec le Javascript permis";
			$this->word_administration = "Administration";
			$this->word_full = "Complètement";
			$this->word_author = "Auteur";
			$this->word_submit = "Soumettez";
			$this->word_refresh = "Régénérez";
			$this->word_more = "plus";
			$this->word_username = "Nom d'utilisateur";
			$this->word_password = "Mot de passe";
			$this->word_login = "Ouverture";
			$this->word_access_denied = "Accédez Nié";
			$this->word_calendar_administration = "Administration De Calendrier";
			$this->word_user_admin = "L'Utilisateur Admin";
			$this->word_user_administration = "Administration D'Utilisateur";
			$this->word_access_level = "Niveau d'accès";
			$this->word_events = "Événements";
			$this->word_event_title = "Titre D'Événement";
			$this->word_event_details = "Détails D'Événement";
			$this->word_start_time = "Heure De Départ";
			$this->word_end_time = "Temps De Fin";
			$this->word_event_type = "Type D'Événement";
			$this->word_date = "Date";
			$this->word_all = "Tous";
			$this->word_weekday = "Jour de la semaine";
			$this->word_every = "Chaque";
			$this->word_of = "de";
			$this->word_all_months = "Tous les Mois";
			$this->word_show_events = "Montrez Les Événements Entre";
			$this->word_show_weekday_events = "Montrez Les Événements De Jour de la semaine";
			$this->word_create_event = "Créez L'Événement";
			$this->word_delete_event = "Enlevez L'Événement";
			$this->word_update_event = "Changez L'Événement";
			$this->word_create_ok = "Événement de calendrier créé avec succès";
			$this->word_update_ok = "Événement de calendrier mis à jour avec succès";
			$this->word_delete_ok = "L'événement de calendrier a été enlevé";
			$this->word_fail_select = "ÉCHOUÉ : Aucun événement choisi";
			$this->word_create_fail = "ÉCHOUÉ : Un champ exigé a été laissé vide ou est inadmissible";
			$this->word_create_unknown = "ÉCHOUÉ : Une erreur inconnue s'est produite";
			$this->word_existing_events = "Événements Existants";
			$this->word_create_user = "Créez Le Compte";
			$this->word_delete_user = "Enlevez Le Compte";
			$this->word_update_user = "Changez Le Compte";
			$this->word_existing_users = "Comptes Existants";
			$this->word_fail_nouser = "ÉCHOUÉ : Le nom de compte d'utilisateur est vide ou invalide";
			$this->word_fail_duplicate = "ÉCHOUÉ : Nom de compte double";
			$this->word_fail_selflower = "ÉCHOUÉ : Vous ne pouvez pas abaisser votre propre niveau d'accès";
			$this->word_fail_selfdelete = "ÉCHOUÉ : Vous ne pouvez pas enlever votre propre compte";
			$this->word_createuser_ok = "Compte d'utilisateur créé avec succès";
			$this->word_deleteuser_ok = "Le compte d'utilisateur a été enlevé";
			$this->word_updateuser_ok = "Compte d'utilisateur mis à jour avec succès";
			$this->word_emptyfield = "Un champ exigé est vide ou invalide";
		}
	}
}
?>
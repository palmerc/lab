<?
/*  ©2003 Proverbs, LLC. All rights reserved.  */

if (eregi("spanish.lng.php", $_SERVER['PHP_SELF']))
{
	// redirect to calendar page
	header("Location: calendar.php");
	exit;
}

if(!defined("SPANISH_LANGUAGE")) 
{
	define("SPANISH_LANGUAGE", TRUE); 

	require ('baselang.inc.php');

	class languageset extends baselanguage
	{
		// Constructor
		function languageset()
		{
			$this->lang_value = "es";
			$this->day_long = Array('Domingo', 'Lunes', 'Martes', 'Miércoles', 'Jueves', 'Viernes', 'Sábado');
			$this->day_short = Array('Dom', 'Lun', 'Mar', 'Mié', 'Jue', 'Vie', 'Sáb');
			$this->day_init = Array('D', 'L', 'M', 'M', 'J', 'V', 'S');
			$this->month_long = Array(1 => 'Enero', 'Febrero', 'Marcha', 'Abril', 'Mayo', 'Junio', 'Julio', 
				'Agosto', 'Septiembre', 'Octubre', 'Noviembre', 'Diciembre');
			$this->month_short = Array(1 => 'Ene', 'Feb', 'Mar', 'Abr', 'May', 'Jun', 'Jul', 'Ago', 'Sep', 'Oct', 
				'Nov', 'Dic');
			$this->word_today_date = "Hoy es";
			$this->word_day = "Día";
			$this->word_month = "Mes";
			$this->word_year = "Año";
			$this->word_all_day = "Todo el dia";
			$this->word_no_javascript = "Este calendario funcionará solamente correctamente con el Javascript permitido";
			$this->word_administration = "Administración";
			$this->word_full = "Por completo";
			$this->word_author = "Autor";
			$this->word_submit = "Someta";
			$this->word_refresh = "Restaure";
			$this->word_more = "más";
			$this->word_username = "Nombre del usuario";
			$this->word_password = "Contraseña";
			$this->word_login = "Conexión";
			$this->word_access_denied = "Tenga acceso Negado";
			$this->word_calendar_administration = "Administración Del Calendario";
			$this->word_user_admin = "Usuario Admin";
			$this->word_user_administration = "Administración Del Usuario";
			$this->word_access_level = "Nivel De Acceso";
			$this->word_events = "Acontecimientos";
			$this->word_event_title = "Título Del Acontecimiento";
			$this->word_event_details = "Detalles Del Acontecimiento";
			$this->word_start_time = "Hora De Salida";
			$this->word_end_time = "Tiempo Del Final";
			$this->word_event_type = "Tipo Del Acontecimiento";
			$this->word_date = "Fecha";
			$this->word_all = "Todos";
			$this->word_weekday = "Día laborable";
			$this->word_every = "Cada";
			$this->word_of = "de";
			$this->word_all_months = "Todos los Meses";
			$this->word_show_events = "Demuestre Los Acontecimientos En medio";
			$this->word_show_weekday_events = "Demuestre Los Acontecimientos Del Día laborable";
			$this->word_create_event = "Cree La Acontecimiento";
			$this->word_delete_event = "Quite La Acontecimiento";
			$this->word_update_event = "Cambie La Acontecimiento";
			$this->word_create_ok = "El acontecimiento del calendario creó con éxito";
			$this->word_update_ok = "Acontecimiento del calendario puesto al día con éxito";
			$this->word_delete_ok = "Se ha quitado el acontecimiento del calendario";
			$this->word_fail_select = "FALLADO: Ningún acontecimiento seleccionado";
			$this->word_create_fail = "FALLADO: Un campo requerido se ha dejado vacío o es inválido";
			$this->word_create_unknown = "FALLADO: Un error desconocido ha ocurrido";
			$this->word_existing_events = "Acontecimientos Existentes";
			$this->word_create_user = "Cree La Cuenta";
			$this->word_delete_user = "Quite La Cuenta";
			$this->word_update_user = "Cambie La Cuenta";
			$this->word_existing_users = "Cuentas Existentes";
			$this->word_fail_nouser = "FALLADO: El nombre de cuenta del usuario es vacío o invalid";
			$this->word_fail_duplicate = "FALLADO: Nombre de cuenta duplicado";
			$this->word_fail_selflower = "FALLADO: Usted no puede bajar su propio nivel de acceso";
			$this->word_fail_selfdelete = "FALLADO: Usted no puede quitar su propia cuenta";
			$this->word_createuser_ok = "La cuenta del usuario creó con éxito";
			$this->word_deleteuser_ok = "Se ha quitado la cuenta del usuario";
			$this->word_updateuser_ok = "La cuenta del usuario se puso al día con éxito";
			$this->word_emptyfield = "Un campo requerido es vacío o invalid";
		}
	}
}
?>
<?php
// This code is meant to handle very basic database issues but mainly to centralize.
define ('DB_USER', 'gradedb'); 
define ('DB_PASSWORD', 'gradedbpw');
define ('DB_HOST', 'localhost');
define ('DB_NAME', 'gradebook');

function database_connect()
{
   $dbc = @mysql_connect (DB_HOST, DB_USER, DB_PASSWORD) OR die ('Could not connect to MySQL: ' . mysql_error() );
   mysql_select_db (DB_NAME) OR die ('Could not select the database: ' . mysql_error() );
}

function database_disconnect()
{
   mysql_close();
}

?>
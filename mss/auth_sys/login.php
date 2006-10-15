<?php
$authorized = FALSE;  // Initialize a variable.

// Check for authentication submission.
if ( (isset($_SERVER['PHP_AUTH_USER']) AND isset($_SERVER['PHP_AUTH_PW'])) ) {

	// Set the database access information as constants.
	define ('DB_USER', 'stcdb');
	define ('DB_PASSWORD', 'stcdbpw');
	define ('DB_HOST', 'localhost');
	define ('DB_NAME', 'stc');
	
	// Make the connnection and then select the database.
	$dbc = @mysql_connect (DB_HOST, DB_USER, DB_PASSWORD) OR die ('Could not connect to MySQL: ' . mysql_error() );
	mysql_select_db (DB_NAME) OR die ('Could not select the database: ' . mysql_error() );
	
	// Query the database.
	$query = "SELECT first_name FROM users WHERE email='{$_SERVER['PHP_AUTH_USER']}' and password=SHA('{$_SERVER['PHP_AUTH_PW']}')";
	$result = mysql_query ($query);
	$row = @mysql_fetch_array ($result);
	if ($row) { // If a record was returned...
		$authorized = TRUE;
	}	
} 

// If they haven't been authorized, create the pop-up window.
if (!$authorized) {
	header('WWW-Authenticate: Basic realm="STC"');
	header('HTTP/1.0 401 Unauthorized'); // For cancellations.
} else {

?>
<?php
if ($_SERVER['REQUEST_METHOD'] == 'POST') 
{
	define ('DB_USER', 'stcdb');
	define ('DB_PASSWORD', 'stcdbpw');
	define ('DB_HOST', 'localhost');
	define ('DB_NAME', 'stc');
   $email = $_POST['email'];
   $first_name = $_POST['first_name'];
   $last_name = $_POST['last_name'];
   $password = $_POST['password'];
   
   $dbc = @mysql_connect (DB_HOST, DB_USER, DB_PASSWORD) OR die ('Could not connect to MySQL: ' . mysql_error() );
   mysql_select_db (DB_NAME) OR die ('Could not select the database: ' . mysql_error() );
   if (!$password) {
      $query = "UPDATE users SET first_name='{$first_name}', last_name='{$last_name}' WHERE email='{$email}' LIMIT 1";
   } else {
      $pw_query = "UPDATE users SET first_name='{$first_name}', last_name='{$last_name}', password=SHA('{$password}') WHERE email='{$email}' LIMIT 1";
   }
   echo $query . "\n"; // DIAGNOSTIC
   $result = mysql_query($query);
   echo $result;
   echo mysql_affected_rows();
   mysql_close();
}

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
	$query = "SELECT email, first_name, last_name FROM users WHERE email='{$_SERVER['PHP_AUTH_USER']}' and password=SHA('{$_SERVER['PHP_AUTH_PW']}')";
	$result = mysql_query ($query);
	$row = @mysql_fetch_array ($result);
   $email = $row[0];
   $first_name = $row[1];
   $last_name = $row[2];
   mysql_close();
} 

?>

<?php
   include('includes/header.html');
?>
<body>
	<div id="container">
		<div id="header">
         <div id="topbanner">
            <img src="i/orangetopwtext2.gif" alt="The Lone Star Community" />
         </div>
         <div id="topnav">
            <ul id="topbar">
               <li><a href="index.php">Go Home</a></li>
               <li>Rock the Vote</li>
               <li>Create a Report</li>
               <li>View Calendar</li>
               <li>Contact Us</li>
            </ul>
         </div>
      </div>
		<div id="main">
			<div id="leftbar">
			</div>
			<div id="rightbar">
            <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
            <table>
            <tr>
            <td>First Name:</td><td><input type="text" name="first_name" value="<?php echo $first_name; ?>" /></td>
            </tr>
            <tr>
            <td>Last Name:</td><td><input type="text" name="last_name" value="<?php echo $last_name; ?>" /></td>
            </tr>
            <tr>
            <td>Email:</td><td><input type="text" name="email" value="<?php echo $email; ?>" /></td>
            </tr>
            <tr>
            <td>Password:</td><td><input type="text" name="password" value="" /></td>
            </tr>
            </table>
            <input type="submit" value="Submit" />
            </form>
         </div>
      </div>
<?php
   include('includes/footer.html');
?>

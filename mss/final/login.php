<?php
if ($_SERVER['REQUEST_METHOD'] == "POST")
{
   require('database.php');
   require('user.php');

   $email = $_POST['email'];
   $password = $_POST['password'];

   database_connect();
   if (auth_user($email, $password)) 
   {
      $row = retrieve_user($email);
      session_start();
      $_SESSION['email'] = $email;
      $first = $row[0]['first_name'];
      $_SESSION['first_name'] = $first;
      $_SESSION['agent'] = md5($_SERVER['HTTP_USER_AGENT']);
      header('location:main.php');
   }
   else
   {
      $error = "Username or password incorrect";
   }
   database_disconnect();
}
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf8" />
<link rel="stylesheet" type="text/css" href="c/main.css" />
<title>Lone Star Community</title>
</head>

<body>
	<div id="container">
		<div id="header"> 
         <div id="topbanner">
			<img src="i/purpletopwtextslim.gif" alt="The Lone Star Community" width="770" height="90"/>
         </div>
         <div id="topnav">
         </div>
      </div>
		<div id="main">
			<div id="leftbar">

			</div>
			<div id="rightbar">
         
<? if ($error) echo '<p>'.$error.'</p>'; ?>
            <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
               <table>        
                  <tr>
                     <td>Email:</td>
                     <td><input type="text" name="email" value="" /></td>
                  </tr>
                  <tr>
                     <td>Password:</td>
                     <td><input type="password" name="password" value="" /></td>
                  </tr>
               </table>
               <input type="submit" value="Submit" />
            </form>
         </div>
      </div>
      <div id="footer">
	  	<p>      Lone Star Community, lonestar@stc.org</p>
      </div>
   </div>
</body>
</html>


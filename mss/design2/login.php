<?php
   include('includes/header.html');
?>
<body>
	<div id="container">
		<div id="header">
         <div id="topbanner">
            <img src="i/orangetopwtext2.gif" alt="The Lone Star Community" />
         </div>
      </div>
      <div id="main">
			<div id="leftbar">
         </div>
         <div id="rightbar">
            <pre>
<?php
   if (isset($_POST['submitted'])) {
      require_once('../mysql_connect.php');
      $errors = array();
      if (empty($_POST['email'])) {
         $errors[] = 'You forgot to enter your email';
         } else {
            $e = escape_data($_POST['email']);
         }

      if (empty($errors)) {
         $query = "SELECT user_id, first_name FROM users WHERE email='$e' AND password=SHA('p')";
         $result = @mysql_query ($query);
         $row = mysql_fetch_array($result, MYSQL_NUM);
         
         if ($row) {
            setcookie('user_id', $row[0]);
            setcookie('first_name', $row[1]);
            $url = 'http://'.$_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']);
            if ((substr($url, -1) == '/') OR (substr($url, -1) == '\\') ) {
               $url = substr($url, 0, -1);
            }
            $url .= '/loggedin.php';
            
            header("Location: $url");
            exit();
         } else {
            $errors[] = 'The username and password entered does not match those on file.';
            $errors[] = mysql_error() . '<br /><br />Query: ' . $query;
         }
      }
      mysql_close();
   } else {
      $errors = NULL;
   }
   
   print_r($_POST);
   echo "
            </pre>
            <form action=\"{$_SERVER['PHP_SELF']}\" method=\"post\">";
   echo'
               <table>
                  <tr>
                     <td>Email:</td><td><input type="text" name="email" /></td>
                  </tr>
                  <tr>
                     <td>Password:</td><td><input type="password" name="password" /><td>
                  </tr>
               </table>
               <input type="submit" value="Submit" />
               <input type="hidden" name="submitted" value="true" />
            </form>
         </div> <!-- END div rightbar -->
      </div> <!-- END div main -->
   ';
?>
<?php
   include('includes/footer.html');
?>

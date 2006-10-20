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
      header('location:index.php');
   }
   else
   {
      $error = "Username or password incorrect";
   }
   database_disconnect();
}
?>
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
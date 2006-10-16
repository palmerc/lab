<?php
if ($_SERVER['REQUEST_METHOD'] == "POST")
{
   require('database.php');
   require('user.php');

   $email = $_POST['email'];
   $password = $_POST['password'];

   database_connect();
   if ($row = auth_user($email, $password)) 
   {
      session_start();
      $_SESSION['email'] = $email;
      $first = $row[1];
      $_SESSION['first_name'] = $first;
      $_SESSION['agent'] = md5($_SERVER['HTTP_USER_AGENT']);
      session_write_close();
      header('location:index.php');
   }
   database_disconnect();
}
?>
   <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
      <table>
         <tr>
            <td>Email:</td>
            <td><input type="text" name="email" value="" /></td>
         </tr>
         <tr>
            <td>Password:</td>
            <td><input type="text" name="password" value="" /></td>
         </tr>
      </table>
      <input type="submit" value="Submit" />
   </form>
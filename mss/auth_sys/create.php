<?php
if ($_SERVER['REQUEST_METHOD'] == "POST")
{
   require('database.php');
   require('user.php');

   $email = $_POST['email'];
   $first = $_POST['first_name'];
   $last = $_POST['last_name'];
   $password = $_POST['password'];
   database_connect();
   $result = create_user($email, $first, $last, $password);
   database_disconnect();
   header('location:manage.php');
}
?>
   <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
   <table>
      <tr>
         <td>First Name:</td>
         <td><input type="text" name="first_name" value="" /></td>
      </tr>
      <tr>
         <td>Last Name:</td>
         <td><input type="text" name="last_name" value="" /></td>
      </tr>
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
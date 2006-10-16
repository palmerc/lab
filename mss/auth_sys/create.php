<?php
require('database.php');
require('user.php');

session_start();
$user_id = $_SESSION['email'];
database_connect();
$result = retrieve_user($user_id);
database_disconnect();
if ($result[0]['admin'] == 1)
{
   if ($_SERVER['REQUEST_METHOD'] == "POST")
   {
      $email = $_POST['email'];
      $first = $_POST['first_name'];
      $last = $_POST['last_name'];
      $password = $_POST['password'];
      $admin = $_POST['admin'];
      database_connect();
      $result = create_user($email, $first, $last, $password, $admin);
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
         <tr>
            <td>Admin Privileges:</td>
            <td><input type="checkbox" name="admin" value="1" /></td>
         </tr>
      </table>
      <input type="submit" value="Submit" />
      </form>
<?php
}
else
{
   echo '<p>Access Denied</p>';
}
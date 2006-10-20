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
      $password1 = $_POST['password1'];
      $password2 = $_POST['password2'];
      $admin = isset($_POST['admin']) ? 1 : 0;
      if ($password1 == $password2)
      {
         database_connect();
         $result = create_user($email, $first, $last, $admin, $password);
         database_disconnect();
         header('location:manage.php');
      }
      else
         echo '<p>Passwords do not match</p>';
   }
   ?>
      <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
      <table>
         <tr>
            <td>First Name:</td>
            <td><input type="text" name="first_name" value="<?php echo $first; ?>" /></td>
         </tr>
         <tr>
            <td>Last Name:</td>
            <td><input type="text" name="last_name" value="<?php echo $last; ?>" /></td>
         </tr>
         <tr>
            <td>Email:</td>
            <td><input type="text" name="email" value="<?php echo $email; ?>" /></td>
         </tr>
         <tr>
            <td>Password:</td>
            <td><input type="password" name="password1" value="" /></td>
         </tr>
         <tr>
            <td>Re-enter Password:</td>
            <td><input type="password" name="password2" value="" /></td>
         </tr>
         <tr>
            <td>Admin Privileges:</td>
            <td><input type="checkbox" name="admin" <?php if (isset($admin)) echo 'checked="checked"' ?> /></td>
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
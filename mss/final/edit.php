<?php
require('database.php');
require('user.php');
session_start();
if (!$_SESSION['email']) 
   header('location:login.php');
$user_id = $_SESSION['email'];
$first = $_SESSION['first_name'];

database_connect();
$result = retrieve_user($user_id);
$admin = $result[0]['admin'];
database_disconnect();

   if ($_SERVER['REQUEST_METHOD'] == "POST")
   {
      $record_id = $_POST['record_id'];
      $email = $_POST['email'];
      $first = $_POST['first_name'];
      $last = $_POST['last_name'];
      $password1 = $_POST['password1'];
      $password2 = $_POST['password2'];
         
      if ($password1 != "" && $password1 == $password2)
         $new_password = $password1;
      else
         $password_error = "<p>Password mismatch</p>";
         
      if ($admin && $user_id != $email)
         $admin_priv = isset($_REQUEST['admin']) ? 1 : 0;
      else if ($admin && $user_id == $email)
         $admin_priv = 1;
      else if (!$admin && $_REQUEST['admin'])
      {
         $admin_priv = 0;
         $admin_error = "<p>Only an admin can change admin field</p>";
      }
      
      if ($admin || $email == $user_id) {
         database_connect();
         modify_user($record_id, $email, $first, $last, $admin_priv, $new_password);
         database_disconnect();
         if ($admin) header('location:manage.php');
         else
            header('location:logout.php');
      }
   }
   require('stc-template.php');
   database_connect();
   $result = retrieve_user($user_id);
   $admin = $result[0]['admin']; // Is the logged in user an admin?
   $email = $_REQUEST['email']; // The user to edit.
   $record_id = isset($_REQUEST['record_id'])?$_REQUEST['record_id']:$_REQUEST['email'];
   if ($_SERVER['REQUEST_METHOD'] == "GET")
   {
      $result = retrieve_user($email); // Get the user to edit's detail.
      $row = $result[0];
      $first = $row['first_name'];
      $last = $row['last_name'];
      $admin_priv = $row['admin'];
   }
   database_disconnect();

?>
      <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
      <input type="hidden" name="record_id" value="<?php echo $email; ?>" />
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
<?php if ($user_id != $email)
      { 
?>
         <tr>
            <td>Admin Privileges:</td>
            <td><input type="checkbox" name="admin" <?php if ($admin_priv) echo 'checked="checked"';?> /></td>
         </tr>
<?php
      }
?>
      </table>
      <input type="submit" value="Submit" />
      </form>
<?php
   require('database.php');
   require('user.php');
   session_start();
   $user_id = $_SESSION['email'];
   database_connect();
   $result = retrieve_user($user_id);
   $admin = $result[0]['admin'];
   database_disconnect();
   
   if (($admin == 1) || ($_GET['email'] == $session_user))
   {
      if ($_SERVER['REQUEST_METHOD'] == "POST")
      {        
         $user_id = $_POST['user_id'];
         $email = $_POST['email'];
         $first = $_POST['first_name'];
         $last = $_POST['last_name'];
         $password = $_POST['password'];
         $admin = $_POST['admin'];

         database_connect();
         modify_user($user_id, $email, $first, $last, $password, $admin);
         database_disconnect();
         header('location:manage.php');
      }
      else
      {
         database_connect();
         $result = retrieve_user($_GET['email']);
         $row = $result[0];
         $email = $_GET['email'];
         $first = $row['first_name'];
         $last = $row['last_name'];
         database_disconnect();
      }
?>
         <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
         <input type="hidden" name="user_id" value="<?php echo $email; ?>" />
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
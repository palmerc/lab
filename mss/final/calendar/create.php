<?php
require('../database.php');
require('../user.php');
require('event.php');
session_start();
if (!$_SESSION['email']) 
   header("location:{$ptr}login.php");
$user_id = $_SESSION['email'];
$first = $_SESSION['first_name'];

database_connect();
$result = retrieve_user($user_id);
database_disconnect();

   if ($_SERVER['REQUEST_METHOD'] == "POST")
   {
      $date = $_POST['date'];
      $title = $_POST['title'];
      database_connect();
      $result = create_event($date, $title);
      database_disconnect();
      header('location:manage.php');
   }
   
   require('../stc-template.php');
   ?>
      <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
      <table>
         <tr>
            <td>Date:</td>
            <td><input type="text" name="date" value="" /></td>
         </tr>
         <tr>
            <td>Title:</td>
            <td><input type="text" name="title" value="" /></td>
         </tr>
      </table>
      <input type="submit" value="Submit" />
      </form>

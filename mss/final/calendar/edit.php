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
$admin = $result[0]['admin'];

$calid = $_REQUEST['calid'];
$result = retrieve_event($calid);
$odate = $result[0]['date'];
$otitle = $result[0]['title'];
database_disconnect();

   if ($_SERVER['REQUEST_METHOD'] == "POST")
   {
      $date = $_POST['date'];
      $title = $_POST['title'];
      database_connect();
      $result = modify_event($calid, $date, $title);
      database_disconnect();
      header('location:manage.php');
   }
   
   require('../stc-template.php');
   ?>
      <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
      <input type="hidden" name="calid" value="<? echo $calid ?>" />
      <table>
         <tr>
            <td>Date:</td>
            <td><input type="text" name="date" value="<? echo $odate ?>" /></td>
         </tr>
         <tr>
            <td>Title:</td>
            <td><input type="text" name="title" value="<? echo $otitle ?>" /></td>
         </tr>
      </table>
      <input type="submit" value="Submit" />
      </form>

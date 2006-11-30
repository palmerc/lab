<?php
require('../database.php');
require('../user.php');
require('event.php');
session_start();
if (!$_SESSION['email']) 
   header('location:login.php');
$user_id = $_SESSION['email'];
$first = $_SESSION['first_name'];

database_connect();
$result = retrieve_user($user_id);
database_disconnect();

require('../stc-template.php');

?>
   <a href="create.php">Create Event</a>
   <table>
      <tr>
         <th>Date</th><th>Title</th><th>Delete</th>
      </tr>
<?php
   database_connect();
   $events = retrieve_all_events();
   foreach ($events as $row)
   {
      $calid = $row['calid'];
      $date = $row['date'];
      $title = $row['title'];
      echo"
      <tr>
         <td><a href=\"edit.php?calid={$calid}\">{$date}</a></td>
         <td>{$title}</td>
         ";
?>
         <td>
         <?php echo "<a href=\"delete.php?calid={$calid}\">Delete</a>"; ?>
         </td>
      </tr>
<?php
   }
   database_disconnect();
?>
   </table>

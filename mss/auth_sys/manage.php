<?php
require('database.php');
require('user.php');
?>
   <a href="create.php">Create User</a>
   <table>
      <tr>
         <th>Email</th><th>First</th><th>Last</th><th>Delete</th>
      </tr>
<?php
   database_connect();
   $users = retrieve_all_users();
   foreach ($users as $row)
   {
      $email = $row['email'];
      $first = $row['first_name'];
      $last = $row['last_name'];
      echo"
      <tr>
         <td><a href=\"edit.php?email={$email}\">{$email}</a></td>
         <td>{$first}</td>
         <td>{$last}</td>
         <td><a href=\"delete.php?email={$email}\">Delete</a></td>
      </tr>
         ";
   }
   database_disconnect();
?>
   </table>

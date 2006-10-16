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
?>
   <a href="create.php">Create User</a>
   <table>
      <tr>
         <th>Email</th><th>First</th><th>Last</th><th>Admin</th><th>Delete</th>
      </tr>
<?php
   database_connect();
   $users = retrieve_all_users();
   foreach ($users as $row)
   {
      $email = $row['email'];
      $first = $row['first_name'];
      $last = $row['last_name'];
      ($row['admin'])?$admin = 'yes':$admin = 'no';
      echo"
      <tr>
         <td><a href=\"edit.php?email={$email}\">{$email}</a></td>
         <td>{$first}</td>
         <td>{$last}</td>
         <td>{$admin}</td>
         <td><a href=\"delete.php?email={$email}\">Delete</a></td>
      </tr>
         ";
   }
   database_disconnect();
?>
   </table>
<?php
}
else
{
echo '<p>Access Denied</p>';
}
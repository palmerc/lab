<?php
require('../database.php');
require('../user.php');
require('vote.php');
session_start();
if (!$_SESSION['email']) 
   header("location:{$ptr}login.php");
$user_id = $_SESSION['email'];
$first = $_SESSION['first_name'];

database_connect();
$result = retrieve_user($user_id);
$admin = $result[0]['admin'];
$results = vote_results();
database_disconnect();
$title = "Lone Star Community - Vote Tally";
require('../stc-template.php');
?>
<h1>Vote Tally</h1>
<table>
<tr>
   <th>User</th><th>Date</th><th>Vote</th>
</tr>
<?
if ($admin)
{
   foreach ($results as $v)
      if ($v['vote_value'] == 0)
         $voted = "Yes";
      else
         $voted = "No";
   echo "
   <tr>
      <td>{$v['user']}</td><td>{$v['vote_date']}</td><td>{$voted}</td>
   </tr>
   ";
}
?>
</table>


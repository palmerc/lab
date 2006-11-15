<?php
require('database.php');
require('user.php');
session_start();
$user_id = $_SESSION['email'];
database_connect();
$result = retrieve_user($user_id);
database_disconnect();

$email = $_REQUEST['email'];
if (($result[0]['admin'] == 1) && ($email != $user_id))
{
   database_connect();
   $result = delete_user($email);
   database_disconnect();
   header('location:manage.php');
}
else
{
   echo '<p>Access Denied!</p>';
}
?>


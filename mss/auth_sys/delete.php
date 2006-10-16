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
   $email = $_GET['email'];
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


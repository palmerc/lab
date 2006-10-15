<?php
require('database.php');
require('user.php');

$email = $_GET['email'];
database_connect();
$result = delete_user($email);
database_disconnect();
header('location:manage.php');
?>

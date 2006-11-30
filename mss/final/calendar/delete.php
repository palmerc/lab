<?php
require('../database.php');
require('../user.php');
require('event.php');
session_start();
$user_id = $_SESSION['email'];
database_connect();
$result = retrieve_user($user_id);
database_disconnect();

$calid = $_REQUEST['calid'];

database_connect();
$result = delete_event($calid);
database_disconnect();
header('location:manage.php');
?>


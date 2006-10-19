<?php
require('database.php');
require('user.php');
session_start();
if (!$_SESSION['email']) 
   header('location:login.php');
$user_id = $_SESSION['email'];
$first = $_SESSION['first_name'];

database_connect();
$result = retrieve_user($user_id);
$admin = $result[0]['admin'];
database_disconnect();

?>

<p>Hello, <a href="edit.php?email=<?php echo $user_id; ?>"><?php echo $first; ?></a>!</p>
<p><a href="logout.php">[Logout]</a></p>

<?php
if ($admin == 1)
   echo '<p><a href="manage.php">Manage Users</a></p>';
?>
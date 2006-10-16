<?php
session_start();
$email = $_SESSION['email'];
$first = $_SESSION['first_name'];
?>
<p>Hello, <a href="edit.php?email=<?php echo $email; ?>"><?php echo $first; ?></a>!</p>
<p><a href="logout.php">[Logout]</a></p>
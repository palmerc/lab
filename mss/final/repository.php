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

   $title = "Lone Star Community - Main Page";
   $leftbar = "leftbar.php";
   require("stc-template.php");

// Where the file is going to be placed 
$target_path = "uploads/";

/* Add the original filename to our target path.  
Result is "uploads/filename.extension" */
$target_path = $target_path . basename( $_FILES['uploadedfile']['name']); 
$_FILES['uploadedfile']['tmp_name'];  


?>
   <p>
   <a href="uploads/">Browse uploads</a>
   </p>

   <form enctype="multipart/form-data" action="uploader.php" method="post">
      <input type="hidden" name="MAX_FILE_SIZE" value="100000" />
      <p>
      Choose a file to upload: <input name="uploadedfile" type="file" />
      </p>
      <p>
      <input type="submit" value="Upload File" />
      </p>
   </form>

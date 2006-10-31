<?php
   require_once('database.php');
   require_once('student.php');
   
   database_connect();
   $student = student_get($_REQUEST['student_key']);
   $photo = $student[0]['photo'];
   header("Content-type: image/jpeg");
   echo $student[0]['photo'];
   database_disconnect();
?>

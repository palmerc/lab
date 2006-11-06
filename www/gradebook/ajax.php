<?php
   require_once('database.php');
   require_once('student.php');
   
   database_connect();
   # if link dies echo error
   echo "hover_box||";
   echo '<pre>';
   $student = student_get($_REQUEST['student_key']);
   print_r($student[0]);
   echo '</pre>';
   database_disconnect();
?>
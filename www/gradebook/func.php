<?php
   require('database.php');
   require('student.php');
   
   database_connect();
   # if link dies echo error
   echo "showDiv||";
   echo '<pre>';
   $student = student_get($_REQUEST['student_key']);
   print_r($student[0]);
   echo '</pre>';
   database_disconnect();
?>
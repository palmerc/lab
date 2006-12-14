<?php
   require('database.php');
   require('student.php');
   if ($_SERVER['REQUEST_METHOD'] == "POST")
   {
      $tmp_name = $_FILES['student_file']['tmp_name'];
      $file = file_get_contents($tmp_name);

      database_connect();      
      foreach($file as $line)
      {
         $student_info = preg_split(",", $line);
         student_create($student_info[0], $student_info[1], $student_info[2], $student_info[3], $student_info[4], $student_info[5], $student_info[6], $student_info[7], 1);
      }
      // Validate submission
      //print_r($_REQUEST);     
      
      database_disconnect();
      //header('location:studentMain.php');
   }
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
   <title>The Gradebook - Import students</title>
   <meta http-equiv="Content-Type" content="text/html; charset=utf8" />
   <link rel="stylesheet" type="text/css" href="c/main.css" />
   <link rel="icon" href="i/favicon.ico" type="image/vnd.microsoft.icon" />
</head>

<body>
   <div id="container">
   <div id="header">
   <h2>Import students</h2>
   </div>
   <div id="page">
   <form enctype="multipart/form-data" action="<?php echo $_SERVER['PHP_SELF'] ?>" method="post">
   File:<input type="file" name="student_file" value="" />
   <input type="submit" value="Submit" />
   </form>
   </div>
   </div>
</body>
</html>
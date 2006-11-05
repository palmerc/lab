<?
require('database.php');
require('course.php');
require('dept.php');
if ($_SERVER['REQUEST_METHOD'] == "POST")
{
   database_connect();
   // Validate submission
   //print_r($_REQUEST);
   $dept_key = $_REQUEST['dept_key'];
   $course_no = $_REQUEST['course_no'];
   $title = $_REQUEST['title'] != null ? $_REQUEST['title'] : '';
   if (course_create($dept_key, $course_no, $title))
      // If all goes well take them back to the studentMain page
      header('location:courseMain.php');
   database_disconnect();
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
   <title>The Gradebook - Create a course</title>
   <meta http-equiv="Content-Type" content="text/html; charset=utf8" />
   <link rel="stylesheet" type="text/css" href="c/main.css" />
   <link rel="icon" href="i/favicon.ico" type="image/vnd.microsoft.icon" />
</head>

<body>
   <div id="container">
   <div id="header">
      <h2>Create a course</h2>
   </div>
   <div id="page">
      <form action="<?php echo $_SERVER['PHP_SELF']?>" method="post">
      <h3>Course basics</h3>
      <div class="indent">
      <table>
      <tr>
         <td>
         Department:
         </td>
         <td>
         <select name="dept_key">
         <?php
            database_connect();
            print_r($results);
            $results = dept_get_all();
            foreach ($results as $option)
            {
               echo"
            <option>{$option['dept_key']}</option>
                  ";
            }
            database_disconnect();
         ?>
         </select>
         </td>
      </tr>
      <tr>
         <td>
         Number:
         </td>
         <td>
         <input type="text" id="course_no" name="course_no" size="4" value="" />
         </td>
      </tr>
      <tr>
         <td>
         Title:
         </td>
         <td>
         <input type="text" id="title" name="title" size="50" value="" />
         </td>
      </tr>
      </table>
      <input type="submit" id="submit" value="Submit" />
      </div>
      </form>
   </div>
   </div>
</body>
</html>
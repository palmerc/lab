<?php
require('database.php');
require('student.php');

database_connect();
if ($_SERVER['REQUEST_METHOD'] == "POST")
{
   //print_r($_POST);
   foreach ($_POST['studentUndelete'] as $student_key)
   {
      $query = "UPDATE student SET is_active='1' WHERE student_key='{$student_key}'";
      $result = mysql_query($query);
   }
}

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
   <title>The Gradebook - Student archive management</title>
   <meta http-equiv="Content-Type" content="text/html; charset=utf8" />
   <link rel="stylesheet" type="text/css" href="c/main.css" />
   <link rel="icon" href="i/favicon.ico" type="image/vnd.microsoft.icon" />
   <script type="text/javascript" src="js/ajax-tooltip.js" />
   <script type="text/javascript" src="js/ajax-dynamic-content.js" />
   <script type="text/javascript" src="js/ajax.js" />
</head>

<body>
   <div id="container">
   <div id="header">
   <h2>Student archive management</h2>
   </div>
   <div id="page">
   <h3>Student managment options</h3>
   <ul>
      <li><a href="index.php">Return to Main</a></li>
      <li><a href="studentDeleteMain.php">Delete a student</a></li>
   </ul>
   <h3>Edit students</h3>
   <form action="<?php echo $_SERVER['PHP_SELF'] ?>" method="post">
   <table id="students">
      <tbody>
      <tr>
      <th>Name</th><th>Course</th><th>EmployeeID</th><th>Comments</th><th>Undelete</th>
      </tr>
<?php

   $users = student_get_all();
   foreach ($users as $row)
   {
      if ($row['is_active'] == 0)
      {
         $student_key = $row['student_key'];
         $email = $row['email'];
         $first_name = $row['first_name'];
         $last_name = $row['last_name'];
         $empl_id = $row['empl_id'];
         // We are going to populate this section with the students in the class
?>
      <tr>
      <td>
         <!--<a id="s<? echo $student_key ?>" onmouseover="javascript:getStudentData(this);" -->
         <a id="s<? echo $student_key ?>" onmouseover="ajax_showTooltip('<? echo "getStudent.php?student_key={$student_key}" ?>',this);"
            onmouseout="ajax_hideTooltip();"
            href="studentEdit.php?student_key=<? echo $student_key ?>">
            <? echo $last_name ?>, <? echo $first_name ?></a>
      </td>
<?php
   $results = null;
   $query = "SELECT course.dept_key, course.course_no, class.section 
            FROM student_class, class, course
            WHERE student_class.class_key=class.class_key 
               AND class.course_key=course.course_key
               AND student_class.student_key={$student_key}";
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   //echo "<pre>";
   //print_r($results);
   //echo "</pre>";
?>
      <td>
<? 
   if ($results)
      foreach ($results as $row)
         printf("%s %d.%03d<br />",$row['dept_key'],$row['course_no'],$row['section']); 

?>
      </td>
      <td><?echo $empl_id ?></td>
      <td></td>
      <td><input type="checkbox" name="studentUndelete[]" value="<? echo $student_key ?>" /></td>
      </tr>
<?php
      }
   }
   database_disconnect();
?>
      </tbody>
   </table>
   <input type="submit" value="Submit" />
   </form>
   <div style="position:absolute;" id="popup">
      <table id="sData" class="studentDetail" border="0" cellspacing="2" cellpadding="2">
         <tbody id="sDataBody"></tbody>
      </table>
   </div>
   </div>
   </div>
</body>
</html>
<?php
require('database.php');
require('class.php');
database_connect();
if ($_SERVER['REQUEST_METHOD'] == "POST")
{
   //print_r($_POST);
   foreach ($_POST['classDelete'] as $class_key)
   {
      $query = "UPDATE class SET is_active='0' WHERE class_key='{$class_key}'";
      $result = mysql_query($query);
   }
}


?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
   <title>The Gradebook - Class management</title>
   <meta http-equiv="Content-Type" content="text/html; charset=utf8" />
   <link rel="stylesheet" type="text/css" href="c/main.css" />
   <link rel="icon" href="i/favicon.ico" type="image/vnd.microsoft.icon" />
</head>

<body>
   <div id="container">
   <div id="header">
   <h2>Class management</h2>
   </div>
   <div id="page">
   <h3>Class managment options</h3>
   <ul>
      <li><a href="index.php">Return to Main</a></li>
      <li><a href="termCreate.php">Add a Term</a></li>
      <li><a href="deptCreate.php">Add a Department</a></li>
      <li><a href="courseCreate.php">Add a Course</a></li>
      <li><a href="classCreate.php">Add a Class</a></li>
      <li><a href="classUndeleteMain.php">Undelete a Class</a></li>
   </ul>
   <h3>Edit classes</h3>
   <form action="<?php echo $_SERVER['PHP_SELF'] ?>" method="post">
   <table id="students">
      <tbody>
      <tr>
      <th>Class</th><th>Name</th><th>Term</th><th>Delete</th>
      </tr>
<?php
   $results = array();
   $query = "SELECT class_key, course.dept_key, course.course_no, course.title, section, term.semester, term.year, is_active
            FROM course, term, class WHERE class.course_key=course.course_key AND class.term_key=term.term_key AND class.is_active='1'";
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   //print_r($results);
   foreach ($results as $row)
   {
      if ($row['is_active'] == 1)
      {
         $class_key = $row['class_key'];
         $dept_key = $row['dept_key'];
         $course_no = $row['course_no'];
         $section = $row['section'];
         $title = $row['title'];
         $semester = $row['semester'];
         $year = $row['year'];
?>
      <tr>
      <td>
         <a href="classEdit.php?class_key=<? echo $class_key ?>">
            <? printf("%s %d.%03d", $dept_key, $course_no, $section); ?></a>
      </td>
      <td><? echo $title ?></td>
      <td><? echo "{$semester} {$year}"; ?></td>
      <td><input type="checkbox" name="classDelete[]" value="<? echo $class_key ?>" /></td>
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
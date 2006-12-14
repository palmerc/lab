<?php
require('database.php');
require('student.php');

database_connect();
// Hard coded 1 should be replaced by something that makes sense
$class_key = isset($_REQUEST['class_key']) ? $_REQUEST['class_key'] : 1;

// This should get you the official name of the class
$query = "SELECT course.dept_key, course.course_no, class.section
         FROM class, course
         WHERE course.course_key=class.course_key
            AND class.class_key={$class_key}";
$result = mysql_query($query);
while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   $results[] = $row;
$class_title = $results[0];

//print_r($_POST);
if ($_SERVER['REQUEST_METHOD'] == "POST")
{ 
   $student_key = $_POST['student'];
   //print_r($_POST);
   if ($_POST['add'])
      $query = "INSERT INTO student_class VALUES('{$class_key}','{$student_key}')";
   else if ($_POST['delete'])
      $query = "DELETE FROM student_class WHERE class_key={$class_key} AND student_key={$student_key} LIMIT 1";
   $result = mysql_query($query);
}


?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
   <title>The Gradebook - Assignment management</title>
   <meta http-equiv="Content-Type" content="text/html; charset=utf8" />
   <link rel="stylesheet" type="text/css" href="c/main.css" />
   <link rel="icon" href="i/favicon.ico" type="image/vnd.microsoft.icon" />
</head>

<body>
   <div id="container">
   <div id="header">
   <h2>Student management</h2>
   </div>
   <div id="page">
   <h3>Student managment options</h3>
   <ul>
      <li><a href="gradebook.php?class_key=<? echo $class_key ?>">Go to gradebook</a></li>
   </ul>
   <h3>Edit <? printf("%s %d.%03d", $class_title['dept_key'], $class_title['course_no'], $class_title['section']); ?> students</h3>
   <form action="<?php echo $_SERVER['PHP_SELF'] ?>" method="post">
   <input type="hidden" name="class_key" value="<? echo $class_key ?>" />
   <h4>Add to the class</h4>
      <select name="student">
<?php
      $students = student_get_all();
      foreach ($students as $student)
         echo"
         <option value=\"{$student['student_key']}\">{$student['last_name']}, {$student['first_name']}</option>
            ";
?>
      </select>
      <input type="submit" name="add" value="Add" />
   </form>
   <h4>Students in the class</h4>
   <table>
<?php
   $class_students = student_class_get_all($class_key);
   //print_r($class_students);
   foreach ($class_students as $student)
   {
   ?>
      <form action="<?php echo $_SERVER['PHP_SELF'] ?>" method="post">
      <input type="hidden" name="class_key" value="<? echo $class_key ?>" />
      <input type="hidden" name="student" value="<? echo $student['student_key'] ?>" />
      <tr><td><?php echo "{$student['last_name']}, {$student['first_name']}"?></td><td><input type="submit" name="delete" value="Delete" /></td></tr>
      </form>
   <?
   }
?>
   </table>
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
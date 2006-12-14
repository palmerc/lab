<?
require('database.php');
require('grades.php');
require('gradebook_functions.php');

$class_key = $_REQUEST['class_key'];

// If no class_key is provided default to somewhere else
if ($class_key == null)
   header("location:index.php");

database_connect();

$assignment_list = get_assignments_list($class_key);
$category_list = get_category_list($class_key);

$student_array = get_students($class_key);
$student_grade_array = get_students_grades();

if ($_SERVER['REQUEST_METHOD'] == "POST")
{
   //echo "<pre>";
   //print_r($_POST);
   //echo "</pre>";
   
   foreach ($_POST['grade'] as $student_key => $assignmentData)
      foreach ($assignmentData as $assignment_key => $grade)
      {
         if ($grade)
         {        
            if ($grade_key = grade_key_get($assignment_key, $student_key))
               grade_edit($grade_key, $grade);
            else
               grade_create($assignment_key, $student_key, $grade);
         }
      }
   
}

$results = class_info($class_key);
$dept_key = $results['dept_key'];
$course_no = $results['course_no'];
$section = $results['section'];


?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
   <title>The Grade Book - <? printf("%s %d.%03d",$dept_key,$course_no,$section); ?></title>
   <meta http-equiv="Content-Type" content="text/html; charset=utf8" />
   <link rel="stylesheet" type="text/css" href="c/main.css" />
   <link rel="icon" href="i/favicon.ico" type="image/vnd.microsoft.icon" />
</head>

<body>
   <div id="container">
      <div id="page">
      <div id="header">
         <h2>Grade Book - <? printf("%s %d.%03d",$dept_key,$course_no,$section); ?></h2>
      </div>
      <br />
      <p id="fields">Currently hidden categories: none</p>
      <div id="gradebook">
         <form action="<? echo $_SERVER['PHP_SELF'] ?>" method="post">
         <input type="hidden" name="class_key" value="<? echo $class_key ?>" />

<?php
table_header($class_key, $category_list, $assignment_list);
student_rows($class_key);
subcategory_averages($class_key);
category_averages($class_key);
table_footer();
?>

         <input type="submit" id="submit" value="Submit Grades" />
         <input type="button" id="reset" value="Undo Changes" />
         <input type="button" id="reset" value="Edit Assignments" />
         </form>
      </div>
      </div>
   </div>
</body>
</html>
<?php
   database_disconnect();
?>
<?
require('database.php');
//require('grades.php');
$class_key = $_REQUEST['class_key'];

// If no class_key is provided default to somewhere else
if ($class_key == null)
   header("location:assignmentMain.php");

// Before we can layout the gradebook we need to know the following:
//   The number and names of the categories
//   The number and names of assignments under each category
//   The number and names of the students
//   The grade fields need to be tied to the student and assignment
//   Don't show a hidden category
//   Phase One - Create the header

database_connect();

// This query gets the class DEPT NUM.SEC information
$query = "SELECT course.dept_key, course.course_no, class.section
         FROM class, course
         WHERE class.course_key=course.course_key
            AND class.class_key={$class_key}";
$result = mysql_query($query);
while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   $results[] = $row;
$results = $results[0];
$dept_key = $results['dept_key'];
$course_no = $results['course_no'];
$section = $results['section'];
$results = null;

// Query to select all assignments in a given class
$query = "SELECT assignment.categoryKey, 
            category.categoryTitle,
            category.classKey,
            category.categoryPercentage,
            category.categoryRank,
            assignment.assignmentKey,
            assignment.assignmentTitle,
            assignment.assignmentMaxPoints,
            assignment.assignmentDueDate,
            assignment.assignmentRank
         FROM category, assignment
         WHERE category.categoryKey=assignment.categoryKey
            AND assignment.classKey={$class_key}
         ORDER BY category.categoryRank,
            assignment.categoryKey,
            assignment.assignmentRank,
            assignment.assignmentKey";
$result = mysql_query($query);
while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   $results[] = $row;
$assignment_total = sizeof($results);

echo "<pre>";
foreach ($results as $record)
{
   /*if (array_key_exists($record['categoryKey'], $assignment_array))
   {
      //echo "HERE";
      $assignment_array[$record['categoryKey']]['assignments'] =
         array(
         $record['assignmentKey'] =>
            array(
            'assignmentTitle' => $record['assignmentTitle'],
            'assignmentMaxPoints' => $record['assignmentMaxPoints'],
            'assignmentDueDate' => $record['assignmentDueDate'],
            'assignmentRank' => $record['assignmentRank']
            )
         );
   }
   else
   {*/
   $assignment_array[$record['categoryKey']] = 
      array(
      'categoryTitle' => $record['categoryTitle'],
      'categoryPercentage' => $record['categoryPercentage'],
      'categoryRank' => $record['categoryRank'],
      'assignments' =>
         array(
         $record['assignmentKey'] =>
            array(
            'assignmentTitle' => $record['assignmentTitle'],
            'assignmentMaxPoints' => $record['assignmentMaxPoints'],
            'assignmentDueDate' => $record['assignmentDueDate'],
            'assignmentRank' => $record['assignmentRank']
            )
         )
      );
   //}
}
print_r($results);
print_r($assignment_array);
echo "</pre>";

//$assignments_rows = sizeof($results); // This is the total number of assignments

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
      <table width="200" border="1">
      <tr>
         <!-- equal the the categories rows -->
         <th rowspan="3" scope="col">Student</th>
         <!-- categories colspan=number of assignments -->
         <th colspan="<? echo $assignments_total ?>" scope="col">Categories</th>
         <!-- equal the the categories rows -->
         <th rowspan="3" scope="col">Student Average</th>
      </tr>

<?
//To draw the table header:
// We need to pull all the categories for a class which we then determine the rowspan=2 if no assignments
// colspan should be equal to the number of assignments
$query = "SELECT category_key, title
         FROM category
         WHERE class_key={$class_key}";
$result = mysql_query($query);
while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
{
   $categories[] = $row;
}
$cat_size = sizeof($categories);
//echo "<pre>";
//print_r($categories);
//echo "</pre>";

// Create the header section
foreach ($categories as $category)
{
   $category_key = $category['category_key'];
   $category_title = $category['title'];
   $query = "SELECT title
            FROM assignment
            WHERE category_key={$category_key} AND class_key={$class_key}";
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $assignments[] = $row;
   }
   $assignment_size = sizeof($assignments);
//   echo "<pre>";
//   print_r($assignments);
//   echo "</pre>";

?>
   <tr>
      <!-- colspan should equal the number of assignments -->
      <th colspan="<? echo $assignment_total ?>"><? echo $category_title ?><input type="checkbox" alt="hide" checked="checked" /></th>
      <!-- if colspan=1 then rowspan=2 -->
   </tr>
   <tr>
<?
   foreach ($assignments as $assignment)
   {
      $assignment_title = $assignment['title'];
      echo"
      <th>{$assignment_title}</th>
         ";
   }
?>
   </tr>
   
<?php
}

$query="SELECT student_class.student_key, student.first_name, student.last_name
      FROM student, student_class
      WHERE student_class.student_key=student.student_key
         AND student_class.class_key={$class_key}
         AND student.is_active=1";
$result = mysql_query($query);
$results = null;
while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
{
   $results[] = $row;
}
$students = $results;

//echo "<pre>";
//print_r($students);
//echo "</pre>";

foreach ($students as $student)
{
   $first_name = $student['first_name'];
   $last_name =  $student['last_name'];
   
?>
     <tr class="student">
         <th scope="col"><? echo "{$last_name}, {$first_name}" ?></th>
<?php
      for ($i = 0; $i < $assignments_total; $i++)
         echo "<td><input type=\"text\" size=\"3\" value=\"\" /></td>";
?>
         <td>avg</td>
      </tr>
<?php
}
database_disconnect();
?>      
      <tr>
         <th scope="col"><p>Subcategory Averages </p>
         </th>
<?php
      for ($i = 0; $i < $assignments_total; $i++)
         echo "<td>avg</td>";
?>
         <td>avg</td>
      </tr>
      </tr>
      <tr>
         <th scope="col">Category Averages </th>
         <td>avg</td>
      </tr>
      
      </table>
         <input type="submit" id="submit" value="Submit Grades" />
         <input type="button" id="reset" value="Undo Changes" />
         <input type="button" id="reset" value="Edit Assignments" />
         </form>
      </div>
      </div>
   </div>
</body>
</html>
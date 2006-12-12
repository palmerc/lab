<?

//
//
// Function declarations
//
function table_header($assignment_array)
{
   global $assignment_count;
   ?>
      <table width="200" border="1">
      <tr>
         <!-- equal the the categories rows -->
         <th rowspan="3" scope="col">Student</th>
         <!-- categories colspan=number of assignments -->
         <th colspan="<? echo $assignment_count ?>" scope="col">Categories</th>
         <!-- equal the the categories rows -->
         <th rowspan="3" scope="col">Student Average</th>
      </tr>
         <tr>
   <?
   // Create the header section
   foreach ($assignment_array as $category)
   {
      $category_title = $category['categoryTitle'];
      $category_assignment_count = sizeof($category['assignments']);
   ?>
         <!-- colspan should equal the number of assignments -->
         <th colspan="<? echo $category_assignment_count ?>"><? echo $category_title ?><input type="checkbox" alt="hide" checked="checked" /></th>
         <!-- if colspan=1 then rowspan=2 -->
   <?php
   }
   ?>
      </tr>
      <tr>
   <?
   foreach ($assignment_array as $category)
   {
      foreach ($category['assignments'] as $assignment)
      {
         $assignment_title = $assignment['assignmentTitle'];
         echo"
         <th>{$assignment_title}</th>
            ";
      }
   }
   ?>
      </tr>
   <?
}

function table_footer()
{
   ?>
   </table>
   <?
}

function student_rows($assignment_array, $student_array, $student_grade_array)
{
   global $class_key, $assignment_count;
   
   //echo "<pre>";
   //print_r($students);
   //echo "</pre>";

   foreach ($student_array as $student)
   {
      $student_key = $student['student_key'];
      $first_name = $student['first_name'];
      $last_name =  $student['last_name'];
      
   ?>
   
         <tr class="student">
            <th scope="col"><? echo "{$last_name}, {$first_name}" ?></th>
   <?php
   //echo "<pre>";
   //print_r($assignment_array);
   //echo "</pre>";
   foreach ($assignment_array as $categoryKey => $categoryData)
   {
      foreach ($categoryData['assignments'] as $assignmentKey => $assignmentData)
      {
         if ($student_grade_array[$student_key]['categories'][$categoryKey]['assignments'][$assignmentKey])
            $grade = $student_grade_array[$student_key]['categories'][$categoryKey]['assignments'][$assignmentKey]['gradesGrade'];
         else
            $grade = "";
         // Check if the student has a grade for this entry and set the field
         // name to be a combination of studentKey and assignmentKey
         echo"
         <td><input name=\"grade[{$student_key}][{$assignmentKey}]\" type=\"text\" size=\"3\" value=\"{$grade}\" /></td>
            ";
      }
   }
   $average = get_student_avg($student_key, $assignment_array, $student_grade_array);
   echo"
            <td>{$average}</td>
         </tr>
      ";
   }
}

function subcategory_averages($assignment_array, $student_grade_array)
{
   global $assignment_count;
   ?>
         <tr>
            <th scope="col"><p>Subcategory Averages</p></th>
   <?php
   //print_r($assignment_array);
   foreach ($assignment_array as $categoryKey => $categoryData)
   {
      foreach ($categoryData[assignments] as $assignmentKey => $assignmentData)
      {
         $average = get_subcategory_avg($assignmentKey, $student_grade_array);
         if ($average)
            echo "<td>{$average}</td>";
         else
            echo "<td>&nbsp;</td>";
      }
   }
   ?>
            <td>avg</td>
         </tr>
   <?
}

function category_averages($assignment_array)
{
   ?>
         <tr>
            <th scope="col">Category Averages </th>
   <?php
   foreach ($assignment_array as $category)
   {
      $category_assignment_count = sizeof($category['assignments']);
      $category
   ?>
         <!-- colspan should equal the number of assignments -->
         <td colspan="<? echo $category_assignment_count ?>"></td>
         <!-- if colspan=1 then rowspan=2 -->
   <?php
   }
   ?>
            <td>avg</td>
         </tr>
   <?
}

function get_assignments()
{
   global $class_key, $assignment_count, $category_count;
   $results = array();

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
      
   $assignment_count = sizeof($results);
   $assignment_array = array();
   //echo "<pre>";
   foreach ($results as $record)
   {
      if (!array_key_exists($record['categoryKey'], $assignment_array))
      {
         $assignment_array[$record['categoryKey']] = 
         array(
         'categoryTitle' => $record['categoryTitle'],
         'categoryPercentage' => $record['categoryPercentage'],
         'categoryRank' => $record['categoryRank'],
         'assignments' => array()
         );
      }

      $assignment_array[$record['categoryKey']]['assignments'][$record['assignmentKey']] = 
         array(
         'assignmentTitle' => $record['assignmentTitle'],
         'assignmentMaxPoints' => $record['assignmentMaxPoints'],
         'assignmentDueDate' => $record['assignmentDueDate'],
         'assignmentRank' => $record['assignmentRank']
         );
   }
   //echo "<pre>";
   //print_r($results);
   //echo "</pre>";
   $category_count = sizeof($assignment_array);
   return $assignment_array;
}

function get_students()
{
   global $class_key;
   // This query is need to generate the table with all the students
   $query="SELECT student_class.student_key, student.first_name, student.last_name
         FROM student, student_class
         WHERE student_class.student_key=student.student_key
            AND student_class.class_key={$class_key}
            AND student.is_active=1";
   $result = mysql_query($query);
   $results = array();
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
      $results[] = $row;
   $students = $results;
   return $students;
}

function get_students_grades()
{
   global $class_key;
   // Query for all students grades
   $query = "SELECT assignment.categoryKey, 
               category.categoryTitle,
               category.classKey,
               category.categoryPercentage,
               category.categoryRank,
               assignment.assignmentKey,
               assignment.assignmentTitle,
               assignment.assignmentMaxPoints,
               assignment.assignmentRank,
               student.student_key,
               student.first_name,
               student.last_name,
               grades.gradeKey,
               grades.grade
            FROM category, assignment, grades, student
            WHERE category.categoryKey=assignment.categoryKey
               AND assignment.assignmentKey=grades.assignmentKey
               AND student.student_key=grades.studentKey
               AND assignment.classKey={$class_key}
            ORDER BY category.categoryRank,
               assignment.categoryKey,
               assignment.assignmentRank,
               assignment.assignmentKey";

   $results = array();
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
      $results[] = $row;
   $student_grade_array = array();

   //echo "<pre>";
   //print_r($results);
   //echo "</pre>";


   // This creates an array of students and their grades, it does not include the
   // information about assignments without grades.
   foreach ($results as $record)
   {
      if (!array_key_exists($record['student_key'], $student_grade_array))
      {
         $student_grade_array[$record['student_key']] =
         array(
         'studentFirst' => $record['first_name'],
         'studentLast' => $record['last_name'],
         'categories' => array()
         );
      }
      
      if (!array_key_exists($record['categoryKey'], $student_grade_array[$record['student_key']]['categories']))
      {
         $student_grade_array[$record['student_key']]['categories'][$record['categoryKey']] =
         array(
         'categoryTitle' => $record['categoryTitle'],
         'categoryPercentage' => $record['categoryPercentage'],
         'categoryRank' => $record['categoryRank'],
         'assignments' => array()
         );
      }
      
      $student_grade_array[$record['student_key']]['categories'][$record['categoryKey']]['assignments'][$record['assignmentKey']] =
         array(
         'assignmentTitle' => $record['assignmentTitle'],
         'assignmentMaxPoints' => $record['assignmentMaxPoints'],
         'assignmentRank' => $record['assignmentRank'],
         'gradesGrade' => $record['grade'],
         'gradesGradeKey' => $record['gradeKey']
         );
   }

   //echo "<pre>";
   //print_r($student_grade_array);
   //echo "</pre>";
   
   return $student_grade_array;
}

function get_student_avg($student_key, $assignment_array, $student_grade_array)
{
   // category_avg = sum of assignments in category / # of items
   // points_attained in category = category percentage * category_avg
   // student_average = sum of points attained in each category
   //echo "<pre>";
   //print_r($student_grade_array);
   //print_r($assignment_array);
   //echo "</pre>";
      
   if ($student_grade_array[$student_key])
   {
      foreach ($student_grade_array[$student_key]['categories'] as $categoryKey => $categoryData)
      {
         $categorySum = 0;
         $count = 0;
         $categoryPercentage = $categoryData['categoryPercentage'];
         foreach ($categoryData['assignments'] as $assignment)
         {
            $categorySum += $assignment['gradesGrade'];
            $count++;
         }
         $categoryAvg = $categorySum / $count;
         $studentCategories[$categoryKey] = $categoryAvg * ($categoryPercentage / 100);
      }
      foreach ($assignment_array as $categoryKey => $categoryData)
      {
         if (!$studentCategories[$categoryKey])
            $studentCategories[$categoryKey] = $categoryData['categoryPercentage'];
      }
      foreach ($studentCategories as $categoryPoints)
         $average += $categoryPoints;
   }
   return $average;
}

function get_subcategory_avg($key, $student_grade_array)
{
   global $class_key, $subcategoryAverages;
   //echo "<pre>";
   //print_r($student_grade_array);
   //echo "</pre>";
   $query = "SELECT grade
         FROM grades
         WHERE assignmentKey={$key}";
   $results = array();
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
      $results[] = $row;
   
   //echo "<pre>";
   //print_r($results);
   //echo "</pre>";
   $count = 0;
   foreach ($results as $grade)
   {
      $sum += $grade['grade'];
      $count++;
   }
   if ($count > 0)
   {
      $average = $sum / $count;
      $subcategoryAverages[$key] = $average;
      return $average;
   }
   else
      return;
}

function get_category_avg($key)
{
   foreach ($assignment_array as $categoryKey => $categoryData)
      foreach ($categoryData['assignments'] as $assignmentKey => $assignmentData)
         $subcategoryAverages[$key] = $assignmentData[''];
}

//
//
// Main program
//

require('database.php');
//require('grades.php');
$class_key = $_REQUEST['class_key'];

// If no class_key is provided default to somewhere else
if ($class_key == null)
   header("location:index.php");
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

$assignment_array = get_assignments();
$student_array = get_students();
$student_grade_array = get_students_grades();

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
table_header($assignment_array);
student_rows($assignment_array, $student_array, $student_grade_array);
subcategory_averages($assignment_array, $student_grade_array);
category_averages($assignment_array);
table_footer();
database_disconnect();
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
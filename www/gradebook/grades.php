<?php
function grade_create($assignment_key, $student_key, $grade)
{
   if (!grade_key_get($assignment_key, $student_key))
   {
      // Query string should contain properly formatted SQL
      $query = "INSERT INTO grades VALUES(null, '{$assignment_key}','{$student_key}','{$grade}')";
      $result = mysql_query($query);
      if (!$result)
         return false;
      return true;
   }
   else
      return false;
}

function grade_edit($grade_key, $grade)
{
   if (grade_exists($grade_key))
   {
      // Query string should contain properly formatted SQL, will want to update     
      // only changed information
      $query = "UPDATE grades SET grade='{$grade}' WHERE gradeKey={$grade_key}";
      $result = mysql_query($query);
      if (!$result)
         return false;
      return true;
   }
   else
      return false;
}

function grade_delete($grade_key)
{
   if (grade_exists($grade_key))
   {
      // Query string should contain properly formatted SQL
      $query = "DELETE FROM grades WHERE gradeKey={$grade_key}";
      $result = mysql_query($query);
      if (result != true)
         return false;
      return true;
   }
   else
      return false;
}

function grade_key_get($assignment_key, $student_key)
{
   // Query string should contain properly formatted SQL
   $query = "SELECT gradeKey FROM grades 
   WHERE assignmentKey={$assignment_key} AND studentKey={$student_key}";
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   
   return $results[0]['gradeKey'];
}


function grade_get($grade_key)
{
   // Query string should contain properly formatted SQL
   $query = "SELECT * FROM grades WHERE gradeKey={$grade_key}";
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   return $results;
}

function grade_get_all()
{
   $query = "SELECT * FROM grades";
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   return $results;
}

function grade_exists($grade_key)
{
   $result = grade_get($grade_key);
   if ($result)
      return true;
   else
      return false;
}


function grades_student_get_row($class_key, $student_key)
{
    $query = "SELECT student.student_key,
               student.first_name,
               student.last_name,
               assignment.assignmentTitle,
               grades.assignmentKey,
               grades.grade
            FROM grades, student, assignment, category
            WHERE grades.studentKey=student.student_key AND
               grades.assignmentKey=assignment.assignmentKey AND
               assignment.categoryKey=category.categoryKey AND
               assignment.classKey={$class_key} AND
               student.student_key={$student_key}               
            ORDER BY category.categoryRank,
               assignment.categoryKey,
               assignment.assignmentRank,
               assignment.assignmentKey";
               
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   //echo "<pre>";
   //print_r($results);
   //echo "</pre>";
   return $results;
}

function get_student_avg($class_key, $student_key)
{    
   $query = "SELECT grades.grade,
               assignment.assignmentMaxPoints,
               category.categoryKey,
               category.categoryPercentage
            FROM grades, assignment, category
            WHERE grades.assignmentKey=assignment.assignmentKey AND
               category.categoryKey=assignment.categoryKey AND
               assignment.classKey={$class_key} AND
               grades.studentKey={$student_key}";
               
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   //echo "<pre>";
   //print_r($category_assignments);
   //print_r($results);
   //echo "</pre>";   
   
   foreach ($results as $assignment)
   {
      $category_assignments[$assignment['categoryKey']]['sum'] += ($assignment['grade'] / $assignment['assignmentMaxPoints']) * 100;
      $category_assignments[$assignment['categoryKey']]['count'] += 1;
      $category_assignments[$assignment['categoryKey']]['categoryPercentage'] = $assignment['categoryPercentage'];
   }
   foreach ($category_assignments as $category)
      $average += round(($category['sum'] / $category['count']) * ($category['categoryPercentage'] / 100), 2);
   return $average;
}

function get_category_avg($class_key, $category_key)
{
   //echo "{$class_key} {$category_key}";
   $query = "SELECT grades.grade,
               assignment.assignmentMaxPoints
            FROM grades, assignment
            WHERE
               grades.assignmentKey=assignment.assignmentKey AND
               assignment.categoryKey={$category_key} AND
               assignment.classKey={$class_key}";
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   foreach ($results as $assignment)
   {
      $category_assignments[$assignment['categoryKey']]['sum'] += ($assignment['grade'] / $assignment['assignmentMaxPoints']) * 100;
      $category_assignments[$assignment['categoryKey']]['count'] += 1;
   }
   foreach ($category_assignments as $category)
      $average += round(($category['sum'] / $category['count']), 2); 
   //echo "<pre>";
   //print_r($results);
   //echo "</pre>";
   return $average;
}


function get_category_list($class_key)
{
   $query = "SELECT categoryKey,
               categoryTitle,
               categoryPercentage
            FROM category
            WHERE classKey={$class_key}
            ORDER BY categoryRank,
               categoryKey";
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
//   echo "<pre>";
//   print_r($results);
//   echo "</pre>";
   return $results;   
}

function get_assignments_list($class_key)
{
   $query = "SELECT assignment.assignmentKey,
               assignment.assignmentTitle
            FROM assignment, category
            WHERE assignment.categoryKey=category.categoryKey AND
               assignment.classKey={$class_key}
            ORDER BY category.categoryRank,
               assignment.categoryKey,
               assignment.assignmentRank,
               assignment.assignmentKey";
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
//   echo "<pre>";
//   print_r($results);
//   echo "</pre>";
   return $results;   
}

function get_students($class_key)
{
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

function get_assignment_avg($class_key, $assignment_key)
{
   $query = "SELECT grades.grade, assignment.assignmentMaxPoints
         FROM grades, assignment
         WHERE grades.assignmentKey=assignment.assignmentKey AND
            assignment.assignmentKey={$assignment_key}";
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
      $average = round(($sum / $count), 2);
      $subcategoryAverages[$key] = $average;
      return $average;
   }
   else
      return;
}

?>
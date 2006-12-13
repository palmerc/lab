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


function grades_student_get_row($class_key, $student_key=1)
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
         $studentCategories[$categoryKey] = round(($categoryAvg * ($categoryPercentage / 100)), 2);
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

function assignments_get($class_key)
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

?>
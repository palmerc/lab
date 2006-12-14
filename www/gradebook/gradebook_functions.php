<?php

function class_info($class_key)
{
   // This query gets the class DEPT NUM.SEC information
   $query = "SELECT course.dept_key, course.course_no, class.section
            FROM class, course
            WHERE class.course_key=course.course_key
               AND class.class_key={$class_key}";
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
      $results[] = $row;
   $results = $results[0];
   return $results;
}

function table_header($class_key, $category_list, $assignment_list)
{
   $colspan = sizeof($assignment_list);

   ?>
      
      <table width="200" border="1">
      <tr>
         <!-- equal the the categories rows -->
         <th rowspan="3" scope="col">Student</th>
         
         <!-- categories colspan=number of assignments -->
         <th colspan="<? echo $colspan ?>" scope="col">Categories</th>
         
         <!-- equal the the categories rows -->
         <th rowspan="3" scope="col">Student Average</th>
      </tr>
         <tr>
   <?
   // Create the header section
   foreach ($category_list as $category)
   {
      $category_title = $category['categoryTitle'];
      //echo "Category Key -> {$category['categoryKey']}";
      $assignment_count = category_assignment_count($class_key, $category['categoryKey']);
      //echo "Assignment Count -> $assignment_count";
   ?>
         <!-- colspan should equal the number of assignments -->
         <th colspan="<? echo $assignment_count ?>"><? echo $category_title ?><input type="checkbox" alt="hide" checked="checked" /></th>
         <!-- if colspan=1 then rowspan=2 -->
   <?php
   }
   ?>
      </tr>
      <tr>
   <?
   
   $assignment_list = get_assignments_list($class_key);
   
   foreach ($assignment_list as $assignment)
   {
      $assignment_title = $assignment['assignmentTitle'];
      echo"
         <th>{$assignment_title}</th>
         ";
   }
   echo"
      </tr>
      ";
}

function table_footer()
{
   ?>
   </table>
   <?
}

function student_rows($class_key)
{  
   $assignment_list = get_assignments_list($class_key);
   $student_list = get_students($class_key);
   
   //echo "<pre>";
   //print_r($student_list);
   //echo "</pre>";
   
   foreach ($student_list as $student)
   {
      $student_key = $student['student_key'];
      $first_name = $student['first_name'];
      $last_name =  $student['last_name'];
      
   ?>   
         <tr class="student">
            <th scope="col"><? echo "{$last_name}, {$first_name}" ?></th>
   <?php
      $student_grades = grades_student_get_row($class_key, $student_key);
       
      foreach ($assignment_list as $assignment)
      {
         $grade = "";
         $assignmentKey = $assignment['assignmentKey'];
         //print_r($student_grades);
         
         foreach ($student_grades as $student_assignment)
         {  
            if ($assignmentKey == $student_assignment['assignmentKey'])
               $grade = $student_assignment['grade'];
         }
         echo"
            <td><input name=\"grade[{$student_key}][{$assignmentKey}]\" type=\"text\" size=\"3\" value=\"{$grade}\" /></td>
            ";
      }
      
      $average = get_student_avg($class_key, $student_key);
      echo"
            <td>{$average}</td>
         </tr>
         ";
   }
}

function get_assignment_array($class_key)
{
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
   
   $assignment_array = array();
   //$assignment_count = sizeof($results);
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
   //$category_count = sizeof($assignment_array);
   return $assignment_array;
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

function subcategory_averages($class_key)
{
   ?>
         <tr>
            <th scope="col"><p>Subcategory Averages</p></th>
   <?php
   $assignment_list = get_assignments_list($class_key);
   foreach ($assignment_list as $assignment)
   {
      $average = get_assignment_avg($class_key, $assignment['assignmentKey']);
      if ($average)
         echo "<td>{$average}</td>";
      else
         echo "<td>&nbsp;</td>";
   }
   ?>
            <td>&nbsp;</td>
         </tr>
   <?
}

function category_averages($class_key)
{
   ?>
         <tr>
            <th scope="col">Category Averages </th>
   <?php
     
   //get category list
   $category_list = get_category_list($class_key);
   //then get average for each category

   foreach ($category_list as $category)
   {
      $average = get_category_avg($class_key, $category['categoryKey']);
      $category_assignment_count = get_category_assignments_list($class_key, $category['categoryKey']);
      echo"
         <!-- colspan should equal the number of assignments -->
         <td colspan=\"{$category_assignment_count}\">{$average}</td>
         <!-- if colspan=1 then rowspan=2 -->
         ";
   }
   ?>
            <td>&nbsp;</td>
         </tr>
   <?
}

function category_assignment_count($class_key, $category_key)
{
   $assignment_array = get_assignment_array($class_key);
   //echo "<pre>";
   //print_r($assignment_array);
   //echo "</pre>";
   return sizeof($assignment_array[$category_key]['assignments']);
}
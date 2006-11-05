<?php
require('database.php');
require('class.php');
require('course.php');
require('term.php');
database_connect();
if ($_SERVER['REQUEST_METHOD'] == 'POST')
{
   // Validate submission
   //print_r($_REQUEST);
   $class_key = $_REQUEST['class_key'];
   $course_key = $_REQUEST['course_key'];
   $section = $_REQUEST['section'];
   $term_key = $_REQUEST['term_key'];
   if (classy_edit($class_key, $course_key, $section, $term_key))
      // If all goes well take them back to the studentMain page
      header('location:classMain.php');
}
else
{
   // Validate submission
   //print_r($_REQUEST);
   $result = classy_get($_REQUEST['class_key']);
   $result = $result[0];
   //print_r($result);
   $class_key = $result['class_key'];
   $course_key = $result['course_key'];
   $section = $result['section'];
   $term_key = $result['term_key'];
}
database_disconnect();
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
   <title>The Gradebook - Edit a class</title>
   <meta http-equiv="Content-Type" content="text/html; charset=utf8" />
   <link rel="stylesheet" type="text/css" href="c/main.css" />
   <link rel="icon" href="i/favicon.ico" type="image/vnd.microsoft.icon" />
</head>

<body>
   <div id="container">
   <div id="header">
      <h2>Edit a class</h2>
   </div>
   <div id="page">
      <form action="?" method="post">
      <input type="hidden" name="class_key" value="<? echo $class_key ?>" />
      <h3>Class basics</h3>
      <div class="indent">
      <table>
      <tr>
         <td>
         Course:
         </td>
         <td>
         <select name="course_key">
<?php
   database_connect();
   $results = course_get_all();
   foreach ($results as $option)
   {
      $checked = $option['course_key'] == $course_key ? ' SELECTED' : '';
      echo"
            <option value=\"{$option['course_key']}\"{$checked}>{$option['dept_key']} {$option['course_no']}</option>
         ";
   }
?>
         </select>
         </td>
      </tr>
      <tr>
         <td>
         Section:
         </td>
         <td>
         <input type="text" id="section" name="section" size="4" value="<? printf("%03d", $section); ?>" />
         </td>
      </tr>
      </label><br />
      <tr>
         <td>
         Term:
         </td>
         <td>
         <select name="term_key">
<?php
   $results = term_get_all();
   foreach ($results as $option)
   {
      $checked = $option['term_key'] == $term_key ? ' SELECTED' : '';
      echo"
            <option value=\"{$option['term_key']}\"{$checked}>{$option['semester']} {$option['year']}</option>
         ";
   }
   database_disconnect();
?>
         </select>
         </td>
      </tr>
      </table>
      </div>
      <h3>Grade cutoffs</h3>
      <div class="indent" id="cutoff">
      <label for="gradeCutoffA">A<input id="gradeCutoffA" name="gradeCutoffA" type="text" size="3" value="90" /></label>
      <label for="gradeCutoffB">B<input id="gradeCutoffB" name="gradeCutoffB" type="text" size="3" value="80" /></label>
      <label for="gradeCutoffC">C<input id="gradeCutoffC" name="gradeCutoffC" type="text" size="3" value="70" /></label>
      <label for="gradeCutoffD">D<input id="gradeCutoffD" name="gradeCutoffD" type="text" size="3" value="60" /></label>
      <label for="gradeCutoffF">F<input id="gradeCutoffF" name="gradeCutoffF" type="text" size="3" value="50" /></label><br />
      <input type="submit" id="submit" value="Submit" />
      </div>
      </form>
   </div>
   </div>
</body>
</html>
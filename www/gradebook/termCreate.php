<?
require('database.php');
require('term.php');
if ($_SERVER['REQUEST_METHOD'] == "POST")
{
   database_connect();
   // Validate submission
   //print_r($_REQUEST);
   $term_key = $_REQUEST['term_key'];
   $semester = $_REQUEST['semester'] != null ? $_REQUEST['semester'] : '';
   $year = $_REQUEST['year'] != null ? $_REQUEST['year'] : '';
   if (term_create($term_key, $semester, $year))
      // If all goes well take them back to the studentMain page
      header('location:classMain.php');
   database_disconnect();
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
   <title>The Gradebook - Create a term</title>
   <meta http-equiv="Content-Type" content="text/html; charset=utf8" />
   <link rel="stylesheet" type="text/css" href="c/main.css" />
   <link rel="icon" href="i/favicon.ico" type="image/vnd.microsoft.icon" />
</head>

<body>
   <div id="container">
   <div id="header">
      <h2>Create a term</h2>
   </div>
   <div id="page">
      <form action="<?php echo $_SERVER['PHP_SELF']?>" method="post">
      <h3>Term basics</h3>
      <div class="indent">
      <table>
      <tr>
         <td>
         Term:
         </td>
         <td>
         <input type="text" id="term_key" name="term_key" size="4" value="" />
         </td>
      </tr>
      <tr>
         <td>
         Semester:
         </td>
         <td>
         <input type="text" id="semester" name="semester" size="10" value="" />
         </td>
      </tr>
      <tr>
         <td>
         Year:
         </td>
         <td>
         <input type="text" id="year" name="year" size="4" value="" />
         </td>
      </tr>
      </table>
      <input type="submit" id="submit" value="Submit" />
      </div>
      </form>
   </div>
   </div>
</body>
</html>
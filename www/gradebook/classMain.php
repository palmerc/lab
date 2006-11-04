<?php
require('database.php');
require('class.php');
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
   <title>The Gradebook - Course management</title>
   <meta http-equiv="Content-Type" content="text/html; charset=utf8" />
   <link rel="stylesheet" type="text/css" href="c/main.css" />
   <link rel="icon" href="i/favicon.ico" type="image/vnd.microsoft.icon" />
   <script type="text/javascript" src="js/ajax-tooltip.js" />
   <script type="text/javascript" src="js/ajax-dynamic-content.js" />
   <script type="text/javascript" src="js/ajax.js" />
</head>
<body>
   <div id="container">
   <div id="header">
   <h2>Class management</h2>
   </div>
   <div id="page">
   <h3>Class options</h3>
   <ul>
      <li><a href="classCreate.php">Add a new class</a></li>
   </ul>
   <h3>Edit or delete an existing course</h3>
   <table>
      <tr>
         <th>Name</th><th>Course</th><th>Term</th>
      </tr>
      <tr>
         <td><a href="?">TITLE</a></td>
         <td>DEPT NUM.SEC</td>
         <td>SEM YEAR</td>
      </tr>
   </table>
   </div>
   </div>
</body>
</html>
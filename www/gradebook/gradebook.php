<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
   <title>The Grade Book - Class Information Goes Here</title>
   <meta http-equiv="Content-Type" content="text/html; charset=utf8" />
   <link rel="stylesheet" type="text/css" href="c/main.css" />
   <link rel="icon" href="i/favicon.ico" type="image/vnd.microsoft.icon" />
</head>

<body>
   <div id="container">
      <div id="page">
      <div id="header">
         <h2>Grade Book - CSCE 4410</h2>
      </div>
      <p id="fields">Currently hidden categories: none</p>
      <div id="gradebook">
         <form action="?" method="post">
         <table width="200" border="1">
           <tr>
             <th rowspan="3" scope="col">Student</th>
             <th colspan="13" scope="col">Categories</th>

             <th rowspan="3" scope="col">Student Average</th>
           </tr>
           <tr>
             <th colspan="6">Homework<input type="checkbox" alt="hide" checked="checked" /></th>
             <th colspan="3">Quizzes<input type="checkbox" alt="hide" checked="checked" /></th>
             <th colspan="2">Exams<input type="checkbox" alt="hide" checked="checked" /></th>
             <th rowspan="2">Final Exam<input type="checkbox" alt="hide" checked="checked" /></th>

             <th rowspan="2">Term Project<input type="checkbox" alt="hide" checked="checked" /></th>
           </tr>
           <tr>
             <th>1</th>
             <th>2</th>
             <th>3</th>
             <th>4</th>

             <th>5</th>
             <th>6</th>
             <th>1</th>
             <th>2</th>
             <th>3</th>
             <th>1</th>

             <th>2</th>
           </tr>
           <tr class="student">
             <th scope="col">Palmer, Cameron </th>
             <td><input type="text" size="3" value="100" /></td>
             <td><input type="text" size="3" value="100" /></td>
             <td><input type="text" size="3" value="100" /></td>

             <td><input type="text" size="3" value="100" /></td>
             <td><input type="text" size="3" value="100" /></td>
             <td><input type="text" size="3" value="100" /></td>
             <td><input type="text" size="3" value="100" /></td>
             <td><input type="text" size="3" value="100" /></td>
             <td><input type="text" size="3" value="100" /></td>

             <td><input type="text" size="3" value="100" /></td>
             <td><input type="text" size="3" value="100" /></td>
             <td><input type="text" size="3" value="100" /></td>
             <td><input type="text" size="3" value="100" /></td>
             <td>100</td>
           </tr>

           <tr>
             <th scope="col"><p>Subcategory Averages </p>

             </th>
             <td>85</td>
             <td>85</td>
             <td>85</td>
             <td>85</td>
             <td>85</td>
             <td>85</td>
             <td>85</td>
             <td>85</td>

             <td>85</td>
             <td>85</td>
             <td>85</td>
             <td>85</td>
             <td>85</td>
             <td>85</td>
           </tr>
           <tr>
             <th scope="col">Category Averages </th>

             <td colspan="6">85</td>
             <td colspan="3">85</td>
             <td colspan="2">85</td>
             <td>85</td>
             <td>85</td>
             <td>85</td>
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
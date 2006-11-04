<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
   <title>The Gradebook - Create a class</title>
   <meta http-equiv="Content-Type" content="text/html; charset=utf8" />
   <link rel="stylesheet" type="text/css" href="c/main.css" />
   <link rel="icon" href="i/favicon.ico" type="image/vnd.microsoft.icon" />
</head>

<body>
   <div id="container">
   <div id="header">
      <h2>Create a class</h2>
   </div>
   <div id="page">
      <form action="?" method="post">
      <h3>Class basics</h3>
      <div class="indent">
      <table>
      <tr>
         <td>
         Title:
         </td>
         <td>
         <input type="text" id="courseTitle" name="courseTitle" value="" />
         </td>
      </tr>
      <tr>
         <td>
         Department:
         </td>
         <td>
         <input type="text" id="courseDept" name="courseDept" size="4" value="" />
         </td>
      </tr>
      </label><br />
      <tr>
         <td>
         Course:
         </td>
         <td>
         <input type="text" id="courseNumber" name="courseNumber" size="4" value="" />
         </td>
      </tr>
      <tr>
         <td>
         Section:
         </td>
         <td>
         <input type="text" id="courseSection" name="courseSection" size="3" value="" />
         </td>
      </tr>     
      <tr>
         <td>
         Term:
         </td>
         <td>
         <input type="text" id="courseTerm" name="courseTerm" value="" />
         </td>
      </tr>
      <tr>
         <td>
         Year:
         </td>
         <td>
         <input type="text" id="courseYear" name="courseYear" size="4" value="" />
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
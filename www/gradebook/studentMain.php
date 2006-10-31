<?php
require('database.php');
require('student.php');
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
   <title>The Gradebook - Student management</title>
   <meta http-equiv="Content-Type" content="text/html; charset=utf8" />
   <link rel="stylesheet" type="text/css" href="c/main.css" />
   <link rel="icon" href="i/favicon.ico" type="image/vnd.microsoft.icon" />
   
   <script type="text/javascript">
   var xmlHttp;
   var dataDiv;
   var dataTable;
   var dataTableBody;
   var offsetEl;

   function createXMLHttpRequest() {
      if (window.ActiveXObject) {
          xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");
      }
      else if (window.XMLHttpRequest) {
          xmlHttp = new XMLHttpRequest();
      }
   }
   
   function initVars() {
      dataTableBody = document.getElementById("studentDataBody");
      dataTable = document.getElementById("studentData");
      dataDiv = document.getElementById("popup");
   }

   function getStudentData(element) {
      initVars();
      createXMLHttpRequest();
      offsetEl = element;
      var url = "getStudent.php?student_key=" + escape(element.id);
      xmlHttp.open("GET", url, true);
      xmlHttp.onreadystatechange = callback;
      xmlHttp.send(null);
   }

   function callback() {
      if (xmlHttp.readyState == 4) {
          if (xmlHttp.status == 200) {
              setData(xmlHttp.responseXML);
          }
      }
   }
     
   function setData(studentData) {
      clearData();
      setOffsets();
      var first_name = studentData.getElementsByTagName("firstName")[0].firstChild.data;
      var last_name = studentData.getElementsByTagName("lastName")[0].firstChild.data;
      var empl_id = studentData.getElementsByTagName("emplId")[0].firstChild.data;
      var row, row3;
      var nameData = "Name: " + first_name + " " + last_name;
      var emplIdData = "EmplID: " + empl_id;
      row = createRow(nameData);
      row2 = createRow(emplIdData);
      
      dataTableBody.appendChild(row);
      dataTableBody.appendChild(row2);
   }

   function createRow(data) {
      var row, cell, txtNode;
      row = document.createElement("tr");
      cell = document.createElement("td");

      cell.setAttribute("bgcolor", "#FFFAFA");
      cell.setAttribute("border", "0");

      txtNode = document.createTextNode(data);
      cell.appendChild(txtNode);
      row.appendChild(cell);
     
      return row;
   }

   function setOffsets() {
      var end = offsetEl.offsetWidth;
      var top = calculateOffsetTop(offsetEl);
      dataDiv.style.border = "black 1px solid";
      dataDiv.style.left = end + 15 + "px";
      dataDiv.style.top = top + "px";
   }
   
   function calculateOffsetTop(field) {
      return calculateOffset(field, "offsetTop");
   }

   function calculateOffset(field, attr) {
      var offset = 0;
      while(field) {
         offset += field[attr];
         field = field.offsetParent;
      }
      return offset;
   }

   function clearData() {
      var ind = dataTableBody.childNodes.length;
      for (var i = ind - 1; i >= 0 ; i--) {
         dataTableBody.removeChild(dataTableBody.childNodes[i]);
      }
      dataDiv.style.border = "none";
   }        
   </script>
</head>

<body>
   <div id="container">
   <div id="header">
   <h2>Student management</h2>
   </div>
   <div id="page">
   <h3>Student managment options</h3>
   <ul>
      <li><a href="studentCreate.php">Add a student</a></li>
      <li><a href="?">Import students from CSV</a></li>
      <li><a href="?">Export students to CSV</a></li>
      <li><a href="?">Undelete a student</a></li>
   </ul>
   <h3>Edit students</h3>
   <form action="<?php echo $_SERVER['PHP_SELF'] ?>" method="post">
   <table id="students">
      <tbody>
      <tr>
      <th>Name</th><th>Course</th><th>EmployeeID</th><th>Comments</th><th>Delete</th>
      </tr>
<?php
   database_connect();
   $users = student_get_all();
   foreach ($users as $row)
   {
      if ($row['is_active'] == 1)
      {
         $student_key = $row['student_key'];
         $email = $row['email'];
         $first_name = $row['first_name'];
         $last_name = $row['last_name'];
         $empl_id = $row['empl_id'];
         // We are going to populate this section with the students in the class
?>
      <tr>
      <td>
         <a id="<? echo $student_key ?>" onmouseover="javascript:getStudentData(this);" 
            onmouseout="clearData();"
            href="studentEdit.php?student_key=<? echo $student_key ?>">
            <? echo $last_name ?>, <? echo $first_name ?></a>
      </td>
      <td>CSCE xxxx.xxx</td>
      <td><?echo $empl_id ?></td>
      <td></td>
      <td><input type="checkbox" name="studentDelete"/></td>
      </tr>
<?php
      }
   }
   database_disconnect();
?>
      </tbody>
   </table>
   <input type="submit" value="Submit" />
   </form>
   <div style="position:absolute;" id="popup">
      <table id="studentData" bgcolor="#FFFAFA" border="0" cellspacing="2" cellpadding="2"/>
         <tbody id="studentDataBody"></tbody>
      </table>
   </div>
   </div>
   </div>
</body>
</html>
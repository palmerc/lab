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
      dataTableBody = document.getElementById("sDataBody");
      dataTable = document.getElementById("sData");
      dataDiv = document.getElementById("popup");
   }

   function getStudentData(element) {
      initVars();
      createXMLHttpRequest();
      offsetEl = element;
      
      var url = "getStudent.php?student_key=" + escape(element.id.substr(1));
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
     
   function processRow(label, data) {
      var row, cell;
      row = createRow();
      cell = createColumn(label);
      row.appendChild(cell);
      cell = createColumn(data);
      row.appendChild(cell);
      dataTableBody.appendChild(row);
   }
   
   function setData(sData) {
      clearData();
      setOffsets();
      var elements = [["First:", "firstName"],
                     ["Last:", "lastName"],
                     ["EmplID:", "emplId"],
                     ["EUID:", "euid"],
                     ["Email:", "email"],
                     ["Website:", "web"],
                     ["Phone:", "phone"],
                     ["Comments:", "comments"]];
      for (var i = 0; i < elements.length; i++) {
         var label = elements[i][0];
         var xmlTag = elements[i][1];
         if (sData.getElementsByTagName(xmlTag)[0].firstChild) {
            var elValue = sData.getElementsByTagName(xmlTag)[0].firstChild.data;
            processRow(label, elValue);
         }
      }
      if (sData.getElementsByTagName("photo")[0].firstChild) {
         var photo = sData.getElementsByTagName("photo")[0].firstChild.data;
         var row = createImageRow(photo);
         dataTableBody.appendChild(row);
      }
   }

   function createImageRow(data) {
      var row, cell, txtNode;
      row = document.createElement("tr");
      cell = document.createElement("td");
      cell.setAttribute("colspan", "2");

      imgNode = document.createElement("img");
      imgNode.setAttribute("src",data)
      cell.appendChild(imgNode);
      row.appendChild(cell);
     
      return row;
   }

   function createColumn(data) {
      var txtNode, cell;
      cell = document.createElement("td");

      txtNode = document.createTextNode(data);
      cell.appendChild(txtNode);
      return cell;
   }

   function createRow() {
      var row;
      row = document.createElement("tr");
      return row;
   }

   function setOffsets() {
      var end = offsetEl.offsetWidth;
      var top = calculateOffsetTop(offsetEl);
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

<?php
// Where the file is going to be placed 
$target_path = "uploads/";

/* Add the original filename to our target path.  
Result is "uploads/filename.extension" */
$target_path = $target_path . basename( $_FILES['uploadedfile']['name']); 
$_FILES['uploadedfile']['tmp_name'];  


?>

<html xmlns="http://www.w3.org/1999/xhtml">
   <body>
      <form enctype="multipart/form-data" action="uploader.php" method="POST">
      <input type="hidden" name="MAX_FILE_SIZE" value="100000" />
      Choose a file to upload: <input name="uploadedfile" type="file" /><br />
      <input type="submit" value="Upload File" />
      </form>
   </body>
</html>
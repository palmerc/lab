<?php
	$title = "CSCE 2410 - PHP Program 7";
	$section = "Assignment: Image Uploading";
	require("../../php-template.php");

	$upload_dir = "uploaddir/";
?>
    <div class="leftside">
	<form enctype="multipart/form-data" method="post" action="<?php echo $_SERVER['PHP_SELF'] ?>">
		<p>
			<input type="file" name="filename" value="" />
		</p>
		<p>
			<input type="submit" name="submit" value="Upload file" />
		</p>
	</form>
    <hr>
    </div>

<?php
    if ($_SERVER['REQUEST_METHOD'] == "POST") {
		if (!uploadFile($upload_dir)) {
            echo "<p class=\"leftside warning\">Only JPEG, GIF or PNG files are allowed</p>";
        }
	}

	displayImages($upload_dir);

function uploadFile($upload_dir) {
    $file_info = getimagesize($_FILES['filename']['tmp_name']);
    $mime_type =$file_info['mime'];
    if ($mime_type == 'image/jpeg' or $mime_type == 'image/gif' or $mime_type == 'image/png') {
        $upload_file = $upload_dir . basename($_FILES['filename']['name']);
        move_uploaded_file($_FILES['filename']['tmp_name'], $upload_file);
        return true;
    } else {
        return false;
    }
}

function displayImages($upload_dir) {
	$dh  = opendir($upload_dir);
	while (false !== ($filename = readdir($dh))) {  	$file_array[] = $filename;	}
	if (count($file_array) > 0) {
		echo '<table class="phototable">';
		foreach ($file_array as $file) {
			if (preg_match('/(\.jpg)|(\.png)|(\.gif)$/', $file)) {
				$FQfile = $upload_dir . rawurlencode($file);
				echo "<tr><td><a href=\"{$FQfile}\"><img alt=\"{$file}\" width=\"100\" src=\"" 
					. $FQfile . "\" /></a></td></tr>";
			}
		}
		echo '</table>';
	}
}

?>

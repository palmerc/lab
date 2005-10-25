<?php
	$title = "CSCE 2410 - PHP Program 7";
	$section = "Assignment: Image Uploading";
	require("../../php-template.php");

	$upload_dir = "uploaddir/";
?>
	<form enctype="multipart/form-data" method="post" action="<?php echo $_SERVER['PHP_SELF'] ?>" class="leftside">
		<p>
			<input type="file" name="filename" value="" />
		</p>
		<p>
			<input type="submit" name="submit" value="Upload file" />
		</p>
	</form>

<?php

//	echo '<div style="class: leftside;"><pre>';
//	print_r($_FILES);
//	echo '</pre></div>';

	if ($_SERVER['REQUEST_METHOD'] == "POST") {
		uploadFiles($upload_dir);
	}

	displayImages($upload_dir);

function uploadFiles($upload_dir) {
	//$gd_array = gd_info();
	print_r($gd_array);
	$upload_file = $upload_dir . basename($_FILES['filename']['name']);
//	echo $upload_file;
	move_uploaded_file($_FILES['filename']['tmp_name'], $upload_file);
	//$thumb_file = $upload_dir . "thumb" . basename($_FILES['filename']['name']);
	
	//copy($upload_file, $thumb_file);
	//resizeImageByWidth($thumb_file, 100);
}

function displayImages($upload_dir) {
	$dh  = opendir($upload_dir);
	//echo $upload_dir;	while (false !== ($filename = readdir($dh))) {  	$file_array[] = $filename;	}
	//print_r($file_array);
	if (count($file_array) > 0) {
		echo '<table class="phototable">';
		foreach ($file_array as $file) {
			if (preg_match('/\.jpg$/', $file)) {
				$FQfile = $upload_dir . $file;
				echo "<tr><td><a href=\"{$FQfile}\"><img alt=\"{$file}\" width=\"100\" src=\"" 
					. $FQfile . "\" /></a></td></tr>";
			}
		}
		echo '</table>';
	}
}

function resizeImageByWidth($filename, $newwidth) {	
	list($width, $height) = getimagesize($filename);
	$percentage = $newwidth / $width;

	$thumbwidth = $width * $percentage;
	$thumbheight = $height * $percentage;

	$thumbnail = imagecreatetruecolor($newwidth, $newheight);
	$sourceimg = imagecreatefromjpeg($filename);

	imagecopyresampled($thumbnail, $sourceimg, 0, 0, 0, 0, $newwidth, $newheight, $width, $height);

	imagejpeg($thumbnail);
}

function checkFileType() {

}



?>

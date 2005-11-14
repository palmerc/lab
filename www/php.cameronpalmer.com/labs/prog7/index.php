<?php
	$title = "CSCE 2410 - PHP Program 7";
  $section = "Assignment: Image Uploading";
  require("../../php-template.php");
  $netpbmdir="/usr/local/netpbm/bin/";
  $root_dir = dirname(__FILE__);
  if (file_exists("{$root_dir}/netpbm_root.php")) require("{$root_dir}/netpbm_root.php");
  require("netpbm.php");

  $upload_dir = "uploaddir/";
  $width = 100; // Pixel width of thumbnails
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
    <hr />
    </div>
    <br style="clear: both;" />

<?php
  if ($_SERVER['REQUEST_METHOD'] == "POST") {
                if (!uploadFile($upload_dir)) {
        echo "<p class=\"leftside warning\">Only JPEG, GIF or PNG files are allowed</p>";
        echo '<br style="clear: both;" />';
    } else {
        $image_name = $_FILES['filename']['name'];
        $image_thumb = "thumb-".$_FILES['filename']['name'];
        scaleImageByWidth($upload_dir.$image_name, $upload_dir.'thumbnails/'.$image_thumb, $width);
        }
  }
        displayImages($upload_dir);

function uploadFile($upload_dir) {
    $file_info = getimagesize($_FILES['filename']['tmp_name']);
    $mime_type = $file_info['mime'];
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
        while (false !== ($filename = readdir($dh))) {
        $file_array[] = $filename;
        }
        if (count($file_array) > 0) {
                //echo '<table class="phototable">';
                foreach ($file_array as $file) {
                        if (preg_match('/(\.jpg)|(\.png)|(\.gif)/i', $file)) {
                                $image_thumb = "thumb-".$file;
                                $FQfile = $upload_dir . rawurlencode($file);
                                $FQimage_thumb = $upload_dir.'thumbnails/'. rawurlencode($image_thumb);
                                echo "<div class=\"pic\"><a href=\"{$FQfile}\"><img alt=\"{$file}\" width=\"100\" src=\"" 
                                        . $FQimage_thumb . "\" /></a></div>";
                        }
                }
                //echo '</table>';
        }
}

function scaleImageByWidth($image_file, $newfile, $width) {
        global $jpegtopnm, $pngtopnm, $giftopnm, $pamscale, $pnmtojpeg, $pnmtopng, $ppmtogif;

        $imageinfo = getimagesize($image_file);
        $newwidth = $width;        
        $image_file = escapeshellarg($image_file);
        $newfile = escapeshellarg($newfile);        
        if ($imageinfo['mime'] == 'image/jpeg') {
                $execstr = "{$jpegtopnm} {$image_file} | {$pamscale} -width=100 | {$pnmtojpeg} > {$newfile}";
                //var_dump($execstr);
                exec($execstr);
        } else if ($imageinfo['mime'] == 'image/png') {
                exec("{$pngtopnm} {$image_file} | {$pamscale} -width={$newwidth} | {$pnmtopng} > {$newfile}");
        } else if ($imageinfo['mime'] == 'image/gif') {
                exec("{$giftopnm} {$image_file} | {$pamscale} -width={$newwidth} | {$ppmtogif} > {$newfile}");
        } else {
                echo "File type cannot be converted";
        }
}

?>
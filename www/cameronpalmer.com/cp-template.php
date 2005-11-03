<?php
header("Content-Type: text/html; charset=utf-8");
do_header();
ob_start("do_footer");
ob_start("do_content");

function do_header() {
  global $title, $section, $alternate_css;
    echo'
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
    <head>
        ';

    echo"
        <title>{$title}</title>
        ";

    echo"
        <link rel=\"stylesheet\" type=\"text/css\" href=\"c/main.css\" />
        ";

    if ($alternate_css and file_exists($alternate_css)) {
        echo"
        <link rel=\"stylesheet\" type=\"text/css\" href=\"{$alternate_css}\" />
            ";
    }

    echo'
        <link rel="shortcut icon" href="favicon.ico" type="image/vnd.microsoft.icon" />
        <link rel="icon" href="favicon.ico" type="image/vnd.microsoft.icon" />
    </head>
    <body>
    	<div id="pbody">
    		';
    echo"
            <div id=\"header\">
                <img src=\"i/bison.jpg\" alt=\"American Bison from the back of the five cent piece\"/>
                <h1>{$title}</h1>
                <div class=\"cleary\">&nbsp;</div>
            </div>
        ";
    echo'
        		<div id="sidebar">
    					<ul>
    						<li><a href="gallery/">Photo Gallery</a></li>
    						<li><a href="http://trac.cameronpalmer.com/">Trac Laboratory</a></li>
    						<li><a href="wiki/">Course Wiki</a></li>
     						<li><a href="resume.php">R&eacute;sum&eacute;</a></li> 			    			    			    			
     					</ul>
                        <div class=\"cleary\">&nbsp;</div>
    				</div>
    		';
    echo"
            <div id=\"content\">
                <h2>{$section}</h2>
        ";
}

function do_content($buf) {
  $x=$buf;
  
  return $buf;
}

function do_footer($buf) {
		echo"
					  
    		";
    return $buf.'
    				</div>
            <div id="footer">
                Copyright &copy; 2005 Cameron Palmer<br />
                <a href="mailto:cameron.palmer@gmail.com">cameron.palmer@gmail.com</a>
            </div>
        </div>
    </body>
</html>
';

}

?>
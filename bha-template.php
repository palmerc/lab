<?php

header("Content-Type: text/html; charset=utf-8");
do_header();
ob_start("do_footer");
ob_start("do_content");

function do_header(){
    global $title, $sec_title;
    echo'
    <!DOCTYPE xhtml PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
    <html>
        <head>
    ';
    echo"
            <title>$title</title>
    ";
    echo'
            <link rel="stylesheet" type="text/css" href="c/main.css" />
            <link rel="shortcut icon" href="/favicon.ico" type="image/vnd.microsoft.icon" />
            <link rel="icon" href="/favicon.ico" type="image/vnd.microsoft.icon" />
            <link rel="alternate" type="application/rss+xml" title="Bruce Hall Events" href="http://web2.unt.edu/bha/rss/" />
        </head>

        <body>
            <div id="wrap">
            <div id="navbar">
                <ul>
                    <li><a href="index.php">Home</a></li>
                    <li><a href="info.php">Info</a></li>
                    <li><a href="staff.php">Staff</a></li>
                    <li><a href="calendar.php">Calendar</a></li>
                    <li><a href="history.php">History</a></li>
                    <li><a href="pictures.php">Pictures</a></li>
                    <li><a href="links.php">Links</a></li>
                    <li><a href="bha.php">BHA</a></li>
                </ul>
            </div>
            <div id="content">
                <div id="content_header">
    ';
    echo"
                    <h2>$sec_title</h2>
                </div>
    ";
}

function do_content($buf){ // This is where a side bar would go
        return $buf.'
                <div class="fixit">&nbsp;</div><!-- get past the two columns -->
        ';
}

function do_footer($buf){
    return $buf.'
            </div>
            <div id="footer">
                <address>Bruce Hall<br />
                University of North Texas<br />
                1624 Chestnut<br />
                Denton, TX 76203</address>
                <p>Phone: 940-565-4343</p>
                <ul>
                    <li><a href="http://validator.w3.org/check?uri=referer">XHTML 1.0</a>/</li>
                    <li><a href="http://jigsaw.w3.org/css-validator/check/referer">CSS 2.0</a>/</li>
                    <li><a title="RSS 2.0" href="/rss/" id="rssbutton">RSS 2.0</a></li>
                </ul>
            </div>
            </div> <!-- END div Wrap -->
        </body>
    </html>
    ';
}

?>
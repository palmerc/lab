#!/usr/local/bin/php

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
	"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">
	<head>
		<meta http-equiv="content-type" content="text/html; charset=utf-8" />
		<title>CSCE 2410 - PHP Program 2</title>
    <link rel="stylesheet" href="c/main.css" type="text/css" />
	</head>
	<body>
		<h1>CSCE 2410 - PHP Program 2</h1>

<?php

    <table>
        <tr>
            <th>
                &nbsp;
            </th>

            for $i < 12 {
                echo "<th>$i</th>";
            }

        </tr>
    </table>
    echo 'This file was last updated: ' . date ('F d Y H:i:s.', getlastmod());

?>


	</body>
</html>
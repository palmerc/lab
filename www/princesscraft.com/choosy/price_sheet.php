<?php
require("XMLParser.php");
header("Content-Type: text/html; charset=utf-8");

$op =& new XMLParser("alineroptions.xml", "file");
$doc = $op->getTree();

/* Uncomment this for the raw view */
/*
header("Content-Type: text/plain");
print_r($doc);
die();
/* */
print_r($doc);
$els = $doc['options'][0]['option'];
echo "<style type=\"text/css\">
.subopt {
    padding-left: 20px;
}
</style>\n";
echo "<table>\n";
echo "<tr><th>Name</th><th>Cost</th></tr>\n";

foreach($els as $e) {
    $section = $e['attributes']['name'];
    $name = $e['name'][0]['value'];
    $cost = @$e['cost'][0]['value'];
    printf("<h1>%s</strong></h1>\n", $section);
    if($e['attributes']['type'] == "single_exclusion") {
        printf("<tr><td><strong>%s</strong></td></tr>\n", $name);
        foreach($e['suboption'] as $o) {
            $subname = $o['name'][0]['value'];
            $subcost = $o['cost'][0]['value'];
            printf("<tr><td class=\"subopt\">%s</td><td>$%0.02f</td></tr>\n", $subname, $subcost);
        }
    } else {
        printf("<tr><td>%s</td><td>$%0.02f</td></tr>\n", $name, $cost);
    }
}
echo "</table>\n";



?>
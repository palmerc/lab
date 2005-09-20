<?php
/*

Todo:

*   Split up templates for rss items.  use htmlspecialchars for
    content:encoded and use ascii for description.
*   Rig up something to show subsequent pages
*   Class-based access.  I wish there was something like Python's generator
    syntax for Python.

*/
$url = "http://www.rvtrader.com/rv_ur_list.php?uid=29388";
$ttl = 60; //minutes


require "httpsession.class.php";
require "cache.class.php";

$c =& new cache();
$data = $c->fetch($url, $ttl);
$headings = array("num", "img", "year", "brand", "model", "price", "type", "state");

//preg_match_all("#<TR.*?onclick=\"location.href='(.*?)';\"[\w\W]+?[>]([\w\W]+?)</TR>#", $data, $m);
if(preg_match_all("#Pages:.*href='rv_ur_list.php\?uid=29388(.*)'#", $data, $pages)){
	for($i=0;$i<count($pages[1]);$i++){
		$data .= $c->fetch($url.$pages[1][$i], $ttl);
		//echo "TEST: $url".$pages[1][$i]."\n";
		preg_match_all("#<TR.*?onclick=\"location.href='(.*?)';\"[\w\W]+?[>]([\w\W]+?)</TR>#", $data, $m);		
	}
}
$data = preg_replace("#<!--[\w\W]*?-->#", "", $data);
preg_match_all("#<TR.*?onclick=\"location.href='(.*?)';\"[\w\W]+?[>]([\w\W]+?)</TR>#", $data, $m);

header("Content-Type: application/rss+xml");

$trailer = array();
for($i=0;$i<count($m[0]);$i++) {
    //echo "Url: {$m[1][$i]}\n";
    preg_match_all("#<TD[\w\W]+?[>]([\w\W]+?)</TD#", $m[2][$i], $tds);
    $trailer[$i] = array("url" => $m[1][$i]);
    foreach($tds[1] as $n=>$x) {
        if($n==1) {
            if(preg_match("#<IMG src=\"(.+?)\"#", $x, $img)) {
                $ret = $img[1];
                #echo "  $n: image: {$img[1]}\n";
            } else $ret=false;
        } else {
            $ret = trim($x);
            #echo "  $n: ".trim($x)."\n";
        }
        $trailer[$i][$headings[$n]] = $ret;
    }
}

echo '<'.'?xml version="1.0" encoding="utf-8"?'.'>
<rss version="2.0"
	xmlns:content="http://purl.org/rss/1.0/modules/content/"
	xmlns:dc="http://purl.org/dc/elements/1.1/"
	><channel>
    <title>Princess Craft Trailers</title>
    <link>http://princesscraft.com/</link>
    <description>Princess Craft trailers for sale via rvtrader.com</description>
';

foreach($trailer as $t) {
    echo "<item><title>{$t['year']} {$t['brand']} {$t['model']}</title>
    <link>http://www.rvtrader.com/{$t['url']}</link><description>";
    if($t['img']) echo "&lt;img src=\"http://www.rvtrader.com/{$t['img']}\" style=\"float: left\" /&gt;";
    echo "&lt;strong&gt;Model:&lt;/strong&gt; {$t['year']} {$t['brand']} {$t['model']}&lt;br /&gt;\n";
    echo "&lt;strong&gt;Price:&lt;/strong&gt; {$t['price']}\n";
    echo "</description></item>\n";
}
echo "</channel></rss>";

//print_r($trailer);
?>

<?php

$title="Princess Craft - Inventory";
$sec_title = "Current Inventory";

require("pc-template-clean.php");

$url = "http://www.rvtrader.com/rv_ur_list.php?uid=29388";
$ttl = 60; //minutes

require "rss/httpsession.class.php";
require "rss/cache.class.php";

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

echo '<table id="inventory">
        <tr><th>Picture</th><th>Year</th><th>Make</th><th>Model</th><th>Price</th>';
foreach($trailer as $t) {
    echo "<tr>
        <td>";
        if($t['img']) 
            echo "<img src=\"http://www.rvtrader.com/{$t['img']}\" />";
        echo "</td><td>{$t['year']}</td><td>{$t['brand']}</td><td>{$t['model']}</td>
            <td>{$t['price']}</td></tr>";
}
echo "</table>";

//print_r($trailer);
?>

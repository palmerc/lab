<?php

$numperpage = 20;

$q = trim($_REQUEST['q2']);
if(!isset($subreq)) $subreq=false;

require_once("lookup_controller.php");
//header("Content-Type: text/plain");

$args = array();
if(!isset($_REQUEST['loc'])) {
    if($subreq) return;
    toss();
}
if(!isset($_REQUEST['career'])) {
    if($subreq) return;
    toss();
}

$args["location__in"] = $_REQUEST['loc'];
$args["career__in"] = $_REQUEST['career'];
$args["term__eq"] = $_REQUEST['sem'];
$args["active__eq"] = "1";

$anylike = array(); //temp array, used below
$searchable = array("short_title", "dept", "subject", "catalog", "building", "room", "instrnm", "mtgdates", "mtgtime");
$fiddleable = array("day", "days");
$terms = array_unique(preg_split("/\s+/", $q));
foreach($terms as $t) {
    if(strpos($t, ":") !== false) {
        list($k,$v) = explode(":", $t);
        if(strlen($v) && in_array($k, $fiddleable)) {
            $k="mtgdates";
            $args["{$k}__like"] = $v;
            continue;
        } else if(strlen($v) && in_array($k, $searchable)) {
            $args["{$k}__like"] = $v;
            continue;
        }
    }
    $anylike[] = $t;
}
if(count($anylike)) {
    $args["these__like"] = array(
        $searchable,
        $anylike);
}

$num = $cclasses->get_count($args);
$page = 1;
if(isset($_REQUEST['page'])) $page = $_REQUEST['page']-0;
$maxpage = max(ceil($num / $numperpage), 1);
if(($page > $maxpage && $num != 0) || $page < 1) {
    die("Invalid page");
}

$x = $cclasses->get_list($args, array("count"=>$numperpage, "start" => ($page - 1) * $numperpage));
$p = array();
foreach($x as $i) {
    if($subreq) {
        echo $i->to_html()."\n";
    } else {
        $p[] = $i->to_js();
    }
}
if(!$subreq) toss($p);

function toss($p=false) {
    if($p==false) $p = array();
    header("Content-Type: text/plain");
    global $q,$page, $maxpage;
    printf("CobaCourses.showresult('%s',%d,%d,[%s]);\n", addslashes($q), $page, $maxpage, join(",\n", $p));
    exit;
}

function buildPager() {
    global $page, $maxpage;
    $totalPages = $maxpage; $thisPage = $page;
    $baseurl = $_SERVER['PHP_SELF']."?".buildRoot();
    global $numperpage;
    $r="";
    
    $half = $numperpage / 2;
    $newCenter = $half;
    
    if($totalPages > $numPerPage - 1) {
        // First guess: the page we're on
        $newCenter = $thisPage;
        // Oops, too close to the end?
        $newCenter = min($newCenter, $totalPages - $half); //if(newCenter > totalPages - 5) newCenter = totalPages - 5;
        // Or too close to the beginning?
        $newCenter = max($newCenter, $half); //if(newCenter < 5) newCenter = 5;
        if($newcenter >= $totalPages) {
            die("Inverted pages");
        }// This shouldn't happen, due to the 9 test above
    }
    
    // It's called 'selectedThingie' at Kate's suggestion, she says it's
    // a technical term for "little pager controls that you click to select your page"
    // Yeah I know it's overkill but it saves a condition.
    if($totalPages != 1) {
        $minVisible = max($newCenter - ($half - 1), 1);
        $maxVisible = min($newCenter + ($half - 1), $totalPages);
        if($minVisible != 1) {
            //create page 1
            $r.=addPageLink(1, $baseurl . "&page=1", $thisPage==1);
            
            if($minVisible != 2)
                $r.="...";
        }
        for($i = $minVisible; $i <= $maxVisible; $i++) {
            if ($i==0) $s=""; else $s="&page=" . $i;
            $r.=addPageLink($i, $baseurl . $s, $thisPage==$i);
        }
        if($maxVisible < $totalPages) {
            //create page totalPages
            if($maxVisible < $totalPages - 1)
                $r.="...";
            $r.=addPageLink($totalPages, $baseurl . "&page=" . $totalPages, $thisPage==$totalPages);
        }
    }
    return $r;
}
function addPageLink($i, $u, $sel=false) {
    $x="";
    $u=fixamp($u);
    if($sel) $x=" class=\"selectedPage\"";
    return "<a href=\"{$u}\"{$x} onclick=\"CobaCourses.setPage({$i}); return false;\">{$i}</a>";
}
function buildRoot() {
    $x = array();
    foreach($_GET as $k=>$v) {
        if($k=="page") continue;
        if(is_array($v)) {
            foreach($v as $a) {
                $x[] = urlencode($k)."[]=".urlencode($a);
            }
        } else {
            $x[] = urlencode($k)."=".urlencode($v);
        }
    }
    return join("&", $x);
}

function fixamp($s) {
    return preg_replace("@&(?!#?[xX]?(?:[0-9a-fA-F]+|\w{1,8});)@", "&amp;", $s);
}

?>
<?php
$settings['pagetype'] = "i";
$settings['extrasheets'] = array("main.css"); //or false
$settings['extrajs'] = array("/programs/courses/ajax.js.php");
$settings['title'] = "UNT/College of Business/Programs/Courses";
require strtolower(getcwd())."/../../common/common.php";
include("_contact.inc");
include("_sidebar.inc");

if(isset($_REQUEST['q2']) && !isset($_REQUEST['submit'])) {
    //set the defaults
    $_REQUEST['loc'] = array("MAIN", "Z-DALL", "Z-INET-TX", "Z-INET-OS");
    $_REQUEST['career'] = array("UGRD", "GRAD");
    $_REQUEST['sem'] = $curterm['cur'];
    $_REQUEST['submit'] = 1;
}
?>
    <div id="main">
<form id="form0" name="form0" method="get" action="<?php echo $_SERVER['PHP_SELF']; ?>"><div class="stupid">
<div id="search-fields">
    <h3>Search Fields</h3>
    <fieldset>
        <legend>Search Term</legend>
        <input type="text" id="q2" name="q2" value="<?php echo htmlspecialchars($_REQUEST['q2']); ?>" autocomplete="off" /><img src="img/spinner.gif" width="16" height="16" alt="" id="search-spinner" /> (<a href="doc/">Help</a>)
    </fieldset>
    <fieldset>
        <legend>Career</legend>
        <label for="career_ugrd"><input type="checkbox" name="career[]" id="career_ugrd" value="UGRD" <?php if(in_array("UGRD", $_REQUEST['career']) || !isset($_REQUEST['submit'])) echo 'checked="checked" '; ?>/>Undergraduate</label>
        <label for="career_grad"><input type="checkbox" name="career[]" id="career_grad" value="GRAD" <?php if(in_array("GRAD", $_REQUEST['career']) || !isset($_REQUEST['submit'])) echo 'checked="checked" '; ?>/>Graduate</label>
    </fieldset>
    <fieldset>
        <legend>Location</legend>
        <label for="loc_main"><input type="checkbox" name="loc[]" id="loc_main" value="MAIN" <?php if(in_array("MAIN", $_REQUEST['loc']) || !isset($_REQUEST['submit'])) echo 'checked="checked" '; ?>/>Main Campus</label>
        <label for="loc_dal"><input type="checkbox" name="loc[]" id="loc_dal" value="Z-DALL" <?php if(in_array("Z-DALL", $_REQUEST['loc']) || !isset($_REQUEST['submit'])) echo 'checked="checked" '; ?>/>Dallas Campus</label>
        <label for="loc_int_tx"><input type="checkbox" name="loc[]" id="loc_int_tx" value="Z-INET-TX" <?php if(in_array("Z-INET-TX", $_REQUEST['loc']) || !isset($_REQUEST['submit'])) echo 'checked="checked" '; ?>/>Internet (Texas)</label>
        <label for="loc_int_other"><input type="checkbox" name="loc[]" id="loc_int_other" value="Z-INET-OS" <?php if(in_array("Z-INET-OS", $_REQUEST['loc']) || !isset($_REQUEST['submit'])) echo 'checked="checked" '; ?>/>Internet (Other)</label>
    </fieldset>
    <div class="jshide">
        <input type="submit" name="submit0" id="submit0" value="Submit" />
    </div>
    <br style="clear: left;" />
    <fieldset id="select-semester">
        <legend>Semester</legend>
        <?php
        error_reporting(E_ALL);
        foreach($curterm as $k=>$t) {
            if( $_REQUEST['sem'] == $t || (!isset($_REQUEST['sem']) && $k=="cur")) {
                $s=' checked="checked" ';
            } else {
                $s="";
            }
            echo "<label for=\"sem_{$t}\"><input type=\"radio\" name=\"sem\" id=\"sem_{$t}\" value=\"{$t}\" {$s} />".describeterm($t)."</label>";
        }
        ?>
    </fieldset>
    <input type="hidden" name="submit" value="1" />
</div>
<hr style="clear: both" />
<p class="notice">Notice: This data lags slightly behind the "live" figures,
    because it is updated from EIS downloads.  For up-to-the-minute information,
    visit <a href="http://my.unt.edu/">my.unt.edu</a>.  Last updated: <?php
require_once("lookup_controller.php");
$query = "SELECT MAX(updtd) FROM tbl_classes";
$q = mysql_query($query) or die("Can't get updated date");
$r = mysql_fetch_array($q, MYSQL_ASSOC);
echo $r['MAX(updtd)'];    
    ?>
</p>

<div id="search-results">
    <h2>Search Results</h2>
    <p class="jshide">Use the submit button above to view updated list of courses (Javascript disabled or your browser does not support <code>XMLHTTPRequest</code>)</p>
    <?php
ob_start("insert_pager");

function insert_pager($s) {
    global $subreq;
    if(!$subreq) return $s;
    return str_replace('<div id="pager"></div>', "<div id=\"pager\">".buildPager()."</div>", $s);
}


?>
    <div id="pager"></div>
    
    <div id="search-results-placeholder">
        <p>Enter a search term above to populate a list of courses that match.</p>
    </div>
    <table id="search-results-table">
        <thead>
            <tr><th>Catalog ID</th><th>Desc</th><th>Time</th><th>Room</th><th>Instructor</th><th>Course ID</th></tr>
        </thead>
        <tbody id="search-results-tbody">
<?php
if(isset($_REQUEST['q2'])) {
    $subreq=true;
    require("lookup.php");
}
?>
        </tbody>
        <tfoot><!-- nothing? --></tfoot>
    </table>
</div>

</div><!-- /main -->

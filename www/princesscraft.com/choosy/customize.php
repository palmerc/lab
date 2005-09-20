<?php

require("xml_parser.php");

if(isset($_REQUEST['nextstep'])) {
    $step = $_REQUEST['nextstep'];
} else {
    $step = 1;
}

$step_func = "step_$step";

render_header();
$step_func(); //calling a function pointer, basically
render_footer();

function step_1() {
    ?>
    <h2>Step 1: How many will it sleep?</h2>
    <input type="hidden" name="nextstep" value="2" />
    <table width="100%" class="matrix">
        <tr class="top">
            <th class="lefty">&nbsp;</th>
            <th>Standard</th>
            <th>LX</th>
            <th>LXE</th>
            <th>Scout</th>
            <th>Sportliner</th>
        </tr>
        <tr>
            <th class="lefty">Sleeps</th>
            <td>2-6</td>
            <td>2</td>
            <td>2</td>
            <td>2</td>
            <td>2</td>
        </tr>
        <tr>
            <th class="lefty"># of Beds</th>
            <td>Configurable</td>
            <td>1 Full</td>
            <td>2 Twin</td>
            <td>2 Twin</td>
            <td>2 Twin</td>
        </tr>
        <tr>
            <th class="lefty ind">Water Heater, Sit Down Shower, and Cassette Toilet</th>
            <td>&nbsp;</td>
            <td>&nbsp;</td>
            <td>&bull;</td>
            <td>&nbsp;</td>
            <td>&nbsp;</td>
        </tr>
        <tr>
            <th class="lefty ind">Cassette Toilet Optional</th>
            <td>&nbsp;</td>
            <td>&bull;</td>
            <td>&nbsp;</td>
            <td>&nbsp;</td>
            <td>&nbsp;</td>
        </tr>
        <tr>
            <th class="lefty">Starts At</th>
            <td>$8,743</td>
            <td>$9,730</td>
            <td>$11,900</td>
            <td>$1995</td>
            <td>$100</td>
        </tr>
        <tr>
            <th class="lefty">&nbsp;</th>
            <td><input type="submit" name="std" value="Pick Me" /></td>
            <td><input type="submit" name="lx" value="Pick Me" /></td>
            <td><input type="submit" name="lxe" value="Pick Me" /></td>
            <td><input type="submit" name="scout" value="Pick Me" /></td>
            <td><input type="submit" name="sportliner" value="Pick Me" /></td>
        </tr>
    </table>
    <?php
}


function step_2() {
    ?>
    <h2>Step 2: Base package</h2>
    <p>Aliner offers several trim lines that you can choose from to make your
    Aliner just the way you want it.  The packages are totally customizable,
    and all options can be added individually if you so choose.  If you don&#8217;t
    see the option you want listed, just give us a ring and we&#8217;ll get right
    on it.</p>
    
    <input type="hidden" name="nextstep" value="3" />
    <label for="pack_base"><input type="radio" name="pack" id="sleeps_two" />Sleeps Two</label>
    <label for="pack_s"><input type="radio" name="pack" id="sleeps_four" />Sleeps Four</label>
    <label for="pack_ls"><input type="radio" name="pack" id="sleeps_six" />Sleeps Six</label>
    <label for="pack_expedition"><input type="radio" name="pack" id="sleeps_six" />Sleeps Six</label>
    <input type="submit" value="Step 2 &raquo;" />
    <?php
}

function step_3() {
    ?>
    <h2>Step 3: Select Options</h2>
    
    <?php
    $data = file_get_contents("alineroptions.xml");
    $x =& new xml2Array();
    $dom = $x->parse($data);
    unset($data);
    foreach($dom['OPTIONS'][0]['SECTION'] as $s) {
        echo "<div class=\"{$s['attributes']['NAME']}\">
        <h2>";
        printf("%s", $s['attributes']['NAME']);
        echo "</h2>
              <ul>";
        foreach($s['OPTION'] as $o) { //Don't forget to handle the suboptions
            $id = $o['attributes']['NAME'];
            echo "<li><label for=\"{$id}\"><input type=\"checkbox\" name=\"{$id}\" id=\"{$id}\" /></label>";
            @printf("%s, \$%0.02f", $o['NAME'][0]['DATA'], $o['COST'][0]['DATA']);
            echo "</li>\n";
        }
        echo "</ul>
              </div>";
    }
}

function render_header() {
?>
<html>
<head>
    <title>Princesscraft: Customize Your Aliner</title>
    <style type="text/css">
body {
    font: 1em Arial;
}
#wrap {
    width: 760px;
    margin: 0 auto;
}

#wrap h2 {
    font: 1.4em Arial;
    color: #666;
    border-bottom: 1px solid navy;
}
label {
    display: block;
    cursor: pointer; cursor: hand;
}
label:hover, tr:hover td {
    background: #eed;
}
table.matrix {
    border-collapse: collapse;
}
th, td {
    border: 1px solid #ccc;
    padding: 2px 4px;
}
th {
    font-weight: normal;
    color: #666;
}
th {
    text-align: center;
}
th.lefty {
    text-align: left;
    border-left: 0;
}
td {
    text-align: center;
}
tr.top th {
    border: 0;
}
th.ind {
    padding-left: 20px;
}


    </style>
</head>
<body>
<div id="wrap">
    <div id="header"><img src="../i/pc_header.png" width="750" height="99" alt="PrincessCraft Trailers" /></div>
    <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
<?php
}

function render_footer() {
?>
</form>
</div>
</body></html>
<?php
}

?>

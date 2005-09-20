<?php

$title="Princess Craft: Customize an Aliner";
$sec_title = "Customize Your Aliner";
$full_width = true;

require("../../pc-template-clean.php");
require("options.class.php");

error_reporting(E_ALL);
ini_set("display_errors", true);

$options =& new optionlist("../../choosy/alineroptions.xml");
$o = $options->get_model_list() or die("Unable to locate models");
$c = $options->get_characteristics_list() or die("Unable to get characteristics");
?>
    <form action="options.php" method="post">
    <table width="100%" class="matrix">
        <tr class="top">
            <th class="lefty">&nbsp;</th>
<?php
        foreach($o as $model){
            echo "<th>{$model->name}</th>";
        }
        echo "</tr>\n";
        foreach($c as $v){
            echo "<tr><th class=\"lefty\">{$v->name}</th>";
            foreach($o as $w){
                echo $v->had_by_ashtml($w);
            }
            echo "</tr>\n";
        }

        echo '<tr><th class="lefty">Starts at</th>';
        foreach($o as $v){
            echo "<td>{$v->cost}</td>";
        }
        echo "</tr><tr><th class=\"lefty\">&nbsp;</th>";
        foreach($o as $model){
            echo "<td><input type=\"submit\" name=\"{$model->id}\" value=\"Pick Me\" /><span class=\"hide\"><a href=\"customize/{$model->id}\">Customize an {$model->name}</a></span></td>";
        }
        echo "</tr>";
?>
    </table>
    </form>
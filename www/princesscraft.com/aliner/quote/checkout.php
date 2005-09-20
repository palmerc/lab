<?php
$full_width = true;

require("form_functions.php");
require("options.class.php");

$options =& new optionlist("../../choosy/alineroptions.xml");
$total = 0;

foreach($options->get_model_list() as $o) {
    if(isset($_REQUEST[$o->id])) $_REQUEST['model'] = $o->id;
}

$model_name = @$_REQUEST['model'];
$model = $options->get_model($model_name) or die("Unknown model");
$title="Princess Craft: Finalize Your {$model->name}";
$sec_title = "Finalize Your {$model->name}";

require("../../pc-template-clean.php");
echo '<form action="'.$ptr.'aliner/quote/details.php" method="post">';

echo "<table class=\"quotesummary\">\n";
printf("<tr><td class=\"quoteitem\">%s</td><td class=\"moolah\">\$%0.02f</td></tr>", $model->name, $model->cost);
$total = $model->cost;
foreach($options->get_list() as $o) {
    if($o->is_selected() !== false) {
        printf("<tr><td>%s</td><td class=\"moolah\">\$%0.02f</td></tr>\n", $o->get_title(), $o->get_cost());
        $o->add_cost($total);
    }
}
printf("<tr class=\"total\"><td class=\"total\">%s</td><td class=\"moolah\">\$%0.02f</td></tr>\n", "Total MSRP, as configured", $total);
echo "</table>\n";
passthru_post();
echo '<div style="padding: 50px 0 0 0; text-align: center;"><input style="padding: 0 2em;" type="submit" value="Submit" /></div></form>';
?>
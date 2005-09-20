<?php
$full_width = true;

require("options.class.php");
$options =& new optionlist("../../choosy/alineroptions.xml");
if(isset($_REQUEST['model'])) {
    $model_name = @$_REQUEST['model'];
    $model = $options->get_model($model_name) or die("Unknown model");
} else {
    foreach($options->get_model_list() as $o) {
        if(isset($_REQUEST[$o->id])) {
            // We need to be _really_ carefuly about getting into a loop here
            // If mod_rewrite isn't set up correctly, I think we'll get a
            //   "unknown model" or 404 on the second run
            // Another wee problem, we don't get the $ptr var until we
            //   include pc-template, below.  grr.
            $redir = $o->id;
	}
    }
    // If we got here without a model, it's an error.
    if(!isset($redir)) die("mod_rewrite problem?");
}

$title="Princess Craft: Customize Your {$model->name}";
$sec_title = "Customize Your {$model->name}";

require("../../pc-template-clean.php");
if(isset($redir)) {
    ob_end_clean();
    header("Location: {$ptr}aliner/quote/customize/{$redir}");
    exit();
}
?>

<script type="text/javascript" src="<?php echo $ptr; ?>c/nicetitle_labels.js"></script>

<p style="margin-bottom: 0;">Note: Prices reflect MSRP.  Feel free to submit your custom aliner
for a personalized &#8220;Best Price&#8221; quote.</p>

<form action="<?php echo $ptr; ?>aliner/quote/checkout.php" method="post"><div class="stupid">
<input type="hidden" name="<?php echo "{$model->id}"; ?>" value="Pick Me" />
<p id="totalmain">MSRP, as Configured: <span id="totalcost">x</span></p>

<?php

$sections=& $options->get_section_list() or die("Can't get sections");

echo "<div class=\"column left\">\n";

foreach($sections as $i=>$sec) {
    $secname = ucwords($sec->name);
    echo "<div class=\"{$sec->name}\">\n<h2>{$secname}</h2>";
    foreach($sec->get_list() as $o) {
        if($o->is_applicable($model)) {
            echo $o->to_htmlform();
        } else {
            echo "Not applicable: {$o->name}<br />\n";
        }
    }
    echo "</div>";
    if($i==3)
        echo "</div><div class=\"column right\">\n";
}
echo "</div>";

echo '<div style="padding: 50px 0 0 0; text-align: center;"><input style="padding: 0 2em;" type="submit" value="Submit" /></div>';

?>
</div></form>
<?php

?>

<script type="text/javascript">
/* <![CDATA[ */

bag = {};
<?php
foreach($options->get_list() as $v) {
    if($v->is_applicable($model)) {
        echo $v->to_js();
    }
}
?>

base = { <?php echo "\"price\": {$model->cost}, \"name\": \"{$model->name}\""; ?> }
function setupEvents(e) {
    /* This function will attach all the proper things to update the price
       totals, etc
     */
    if(!document.getElementById) return;
    if(!document.getElementById("totalcost")) return;

    var inputs = document.getElementsByTagName("INPUT");
    for (var i=0; i < inputs.length; i++) {
        el = inputs[i];
        addEvent(el, "click", clickUpdateTotals);
    }
}

function checksAndDisables(e) {
    var inputs = document.getElementsByTagName("INPUT");

    for (var i=0; i < inputs.length; i++) {
        el = inputs[i];
        id = el.getAttribute("id");
        if(bag[id]) {
            if(bag[id].includes) { //Added by cameron to handle includes
                for(var j=0; j < bag[id].includes.length; j++) {
                    if(!document.getElementById(id).checked) {
                        for(var j=0; j < bag[id].includes.length; j++) {
                            x = bag[id].includes[j];
                            if(document.getElementById(x).disabled){
                                document.getElementById(x).checked=false;
                                document.getElementById(x).disabled=false;
                            }
                        }
                    }
                    if(document.getElementById(id).checked){
                        for(var j=0; j < bag[id].includes.length; j++) {
                            x = bag[id].includes[j];
                            if(!document.getElementById(x).checked){
                                document.getElementById(x).checked=true;
                            }
                            if(!document.getElementById(x).disabled){
                                document.getElementById(x).disabled=true;
                            }
                        }
                    }
                }
            } //end includes

            if(bag[id].requires) {
                var enabled = true;
                for(var j=0; j < bag[id].requires.length; j++) {
                    x = bag[id].requires[j];
                    if(!document.getElementById(x).checked) enabled=false;
                }
                if(el.disabled != !enabled) el.disabled = !enabled;
                if(!enabled && el.checked) {
                    el.checked = false;
                    dirty = true;
                }
            } //end requires
        } //end bag[id]
    }
}

function clickUpdateTotals(e) {
    if(window.event) {
        el = window.event.srcElement;
    } else {
        el = this;
    }
    checksAndDisables(e);
    updateTotals(e);
}

function updateTotals(e) {
    var total = base.price;

    var inputs = document.getElementsByTagName("INPUT");
    for (var i=0; i < inputs.length; i++) {
        el = inputs[i];
        id = el.getAttribute("id");
        if(bag[id]) {
            //alert(el.checked);
            if(el.checked && !el.disabled) total += bag[id].price;
        }
    }

    document.getElementById("totalcost").innerHTML = moolah(total);
}

// Add an eventListener to browsers that can do it somehow.
// Originally by the amazing Scott Andrew.
function addEvent(obj, evType, fn){
  if (obj.addEventListener){
    obj.addEventListener(evType, fn, true);
    return true;
  } else if (obj.attachEvent){
	var r = obj.attachEvent("on"+evType, fn);
    return r;
  } else {
	return false;
  }
}

function moolah(n) {
	return "$"+Xtend(Math.round(n * 100) / 100, 2);
}

//helper to add zeros (from http://www.merlyn.demon.co.uk/js-round.htm#TtR)
function Xtend(Q, N) {
    var P;
    Q = String(Q);
    if (/e/i.test(Q)) {
        return Q;
    }
    while ((P = Q.indexOf(".")) < 0) {
        Q += ".";
    }
    while (Q.length <= P + N) {
        Q += "0";
    }
    return Q;
}

addEvent(window, "load", setupEvents);
addEvent(window, "load", checksAndDisables);
addEvent(window, "load", updateTotals);

/* ]]> */
</script>

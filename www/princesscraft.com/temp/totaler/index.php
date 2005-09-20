<html>
<head>
<style type="text/css">
label { display: block; }
</style>

<script type="text/javascript">
/* <![CDATA[ */

bag = {};
bag.option_one = { "price": 1.20, "allows": ["option_three"] };
bag.option_two = { "price": 4.00, "allows": [] };
bag.option_three = { "price": 9.99, "allows": [] };

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

function clickUpdateTotals(e) {
    if(window.event) {
        el = window.event.srcElement;
    } else {
        el = this;
    }
    id = el.getAttribute("id");
    if (!bag[id]) return; //in this case, we shouldn't _need_ to update the total
    
    o = bag[id];
    //alert("You checked an option worth "+o.price);
    //alert("Updating a total on " + el.getAttribute("id") + el.checked);
    updateTotals(e);
}

function updateTotals(e) {
    var inputs = document.getElementsByTagName("INPUT");
    total = 0;
    for (var i=0; i < inputs.length; i++) {
        el = inputs[i];
        id = el.getAttribute("id");
        if(bag[id]) {
            //alert(el.checked);
            if(el.checked) total += bag[id].price;
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
addEvent(window, "load", updateTotals);

/* ]]> */
</script>
</head>
<body>
<div id="totalcost">Cost</div>

<form name="form0" id="form0" action="?" method="post">
    <label for="option_one"><input type="checkbox" id="option_one">$1.20 Option One</label>
    <label for="option_two"><input type="checkbox" id="option_two">$4.00 Option Two</label>
    <label for="option_three"><input type="checkbox" id="option_three">$9.99 Option Three</label>
</form>

</body></html>
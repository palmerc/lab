<?php
header("Content-Type: text/javascript");

$silent = true;
require strtolower(getcwd())."/../../common/common.php";
?>/*

College of Business Administration at UNT - Course Search

    Try.these is from the Prototype library,
    see http://prototype.conio.net/

    addEvent is by Scott Andrew
    
    String.prototype.trim is from http://www.ditchnet.org/wp/2005/04/04/

*/

var None = null; // I'm stuck in Python mode!

var Try = {
    these: function() {
        var returnValue;
        
        for (var i = 0; i < arguments.length; i++) {
            var lambda = arguments[i];
            try {
                returnValue = lambda();
                break;
            } catch (e) {}
        }
        return returnValue;
    }
}

var Debug = {
    enabled: false,
    win:None,
    init: function() {
        var self = Debug;
        if(!self.enabled) return;
        self.win = window.open("", "dbgwin", "width=500,height=400");
        if(!self.win) {
            //popups are blocked, grr
            alert("popups blocked");
            return;
        }
        if(self.win.loaded) {
            self.win.document.write("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n</pre>");
        }
        self.win.loaded = true;
        //window.dbgwin.focus();
        self.win.document.write("<pre><font color=red>Debug window for "+window.location.href+"</font>\n");
    },
    trace: function(s) {
        var self = Debug;
        if(!self.win) return;
        if(!self.win.document) return;
        if(!self.enabled) return;
        self.win.document.write(s+"\n");
        self.win.scrollBy(0,1000);
    }
}

String.prototype.trim = function() {
    return this.replace(/^\s*/,'').replace(/\s*$/,'');
};


function addEvent(obj, evType, fn, useCapture){
    if (obj.addEventListener){
        obj.addEventListener(evType, fn, useCapture);
        return true;
    } else if (obj.attachEvent){
        var r = obj.attachEvent("on"+evType, fn);
        return r;
    } else {
        alert("Handler could not be attached");
    }
}


var Ajax = {
    getTransport: function() {
        return Try.these(
            function() {return new ActiveXObject('Msxml2.XMLHTTP')},
            function() {return new ActiveXObject('Microsoft.XMLHTTP')},
            function() {return new XMLHttpRequest()}
        ) || false;
    }
}

var CobaCourses = {
    runner: None,
    live: false,
    baseurl: "/programs/courses/",
    checks: ["career_ugrd", "career_grad", "loc_main", "loc_dal", "loc_int_tx", "loc_int_other"],
    text: ["q2"],
    radio: [<?php foreach($curterm as $t) { $x[] = "\"sem_{$t}\""; } echo join(", ", $x); ?>],
    init: function() {
        var self = CobaCourses;
        Debug.trace("My runner is " + self.runner);
        Debug.trace("Populating fields");
        //TODO: Check the hash part, set vars
        
        Debug.trace("Attaching onchange");
        //TODO: Attach onChange
        a = self.checks.concat(self.text).concat(self.radio);
        Debug.trace(a);
        for(v in a) {
            watchEl(a[v], self.refresh);
        }
        self.live = true;
        var q = getvar("q2").trim();
        if(q == "") self.refresh();
        else {
            hide("search-spinner");
            hide("search-results-placeholder");
        }
    },
    refresh: function() {
        var self = CobaCourses;
        self.page = 1;
        Debug.trace("-&gt; refresh()");
        self.goUrl();
        Debug.trace("&lt;- refresh()");
    },
    buildUrl: function() {
        var self = CobaCourses;
        var q = getvar("q2").trim()
        stuff = [];
        for(c in self.checks) {
            try { var el = getEl(self.checks[c]); }
            catch(e) { continue; }
            if(el.checked) 
                stuff.push(el.name + "=" + el.value);
        }
        for(r in self.radio) {
            try { var el = getEl(self.radio[r]); }
            catch(e) { continue; }
            if(el.checked)
                stuff.push(el.name + "=" + el.value);
        }
        
        stuff.unshift("q2=" + escape(q));
        
        qs = stuff.join("&");
        //var url = self.baseurl + "lookup.php?" + qs;
        return qs;
    },
    goUrl: function() {
        var self = CobaCourses;
        var q = getvar("q2").trim()
        if(q=="") {
            self.showblank();
            return;
        }
        
        var url = self.baseurl + "lookup.php?" + self.buildUrl() + "&page=" + self.page;
        show("search-spinner");
        Debug.trace("url: " + url);
        
        self.resetAjax();
        self.runner.open("GET", url, true);
        self.busy = true;
        self.runner.onreadystatechange=function() {
            if(self.runner.readyState==4 && self.runner.responseText) {
                eval(self.runner.responseText);
                live = false;
            }
        };
        self.runner.send(null);
    },
    showresult: function(thisSearchString,thisPage,totalPages,courseListings) {
        var self = CobaCourses;
        baseurl = "?" + self.buildUrl();
        hide("search-results-placeholder");
        show("search-results-table");
        hide("search-spinner");
        
        var pager = document.getElementById("pager");
        assert(pager, "pager check");
        var numPerPage = 20; //note: evens only
        var half = numPerPage / 2;
        var newCenter = half;
        if(totalPages > numPerPage - 1) {
            // First guess: the page we're on
            newCenter = thisPage;
            // Oops, too close to the end?
            newCenter = Math.min(newCenter, totalPages - half); //if(newCenter > totalPages - 5) newCenter = totalPages - 5;
            // Or too close to the beginning?
            newCenter = Math.max(newCenter, half); //if(newCenter < 5) newCenter = 5;
            assert(newCenter < totalPages, "inverted pages"); // This shouldn't happen, due to the 9 test above
        }
        
        // It's called 'selectedThingie' at Kate's suggestion, she says it's
        // a technical term for "little pager controls that you click to select your page"
        if(pager.searchString == baseurl && pager.centeredThingie == newCenter && pager.selectedThingie && totalPages > 1) {
            // We can reuse the existing objects, just juggle a class
            pager.selectedThingie.className = "";
            id = "pager_" + thisPage;
            pager.selectedThingie = document.getElementById(id);
            assert(pager.selectedThingie, "selected thingie existence");
            pager.selectedThingie.className = "selectedPage";
        } else {
            // Yeah I know it's overkill but it saves a condition.
            pager.searchString = baseurl;
            pager.centeredThingie = newCenter;
            while(pager.firstChild) pager.removeChild(pager.firstChild);
            if(totalPages != 1) {
                var minVisible = Math.max(newCenter - (half - 1), 1);
                var maxVisible = Math.min(newCenter + (half - 1), totalPages);
                var a;
                if(minVisible != 1) {
                    //create page 1
                    addPageLink(pager, 1, baseurl + "&page=1");
                    if(minVisible != 2)
                        pager.appendChild(document.createTextNode("..."));
                }
                for(var i = minVisible; i <= maxVisible; i++) {
                    if (i==0) s=""; else s="&page=" + i;
                    addPageLink(pager, i, baseurl + s);
                }
                if(maxVisible < totalPages) {
                    //create page totalPages
                    if(maxVisible < totalPages - 1)
                        pager.appendChild(document.createTextNode("..."));
                    addPageLink(pager, totalPages, baseurl + "&page=" + totalPages);
                }
                
                pager.selectedThingie = document.getElementById("pager_" + thisPage);
                if(pager.selectedThingie) {
                    pager.selectedThingie.className = "selectedPage";
                }
            }
            
        }
        pager.searchString = baseurl;


        // ******************************
        // Actually update the table and whatnot
        // ******************************
        tab = document.getElementById("search-results-tbody");
        while(tab.firstChild) tab.removeChild(tab.firstChild);
        var tr, td;
        document.v = courseListings;
        for(var i=0; i < courseListings.length; i++) {
            c = courseListings[i];
            tr = document.createElement("TR");
            //alert(classes.length); alert(c.length);
            //assert(classes.length == c.length, "class length thingie");
            for(var j in c) {
                td = document.createElement("TD");
                td.setAttribute("class", j);
                td.appendChild(document.createTextNode(c[j]));
                tr.appendChild(td);
            }
            tab.appendChild(tr);
        }
    },
    showblank: function() {
        var self = CobaCourses;
        //if (self.runner.readyState != 0)
        //    self.resetAjax();
        Debug.trace("-&gt; showblank()");
        hide("search-spinner");
        hide("search-results-table");
        show("search-results-placeholder");
        var pager = getEl("pager");
        while(pager.firstChild) pager.removeChild(pager.firstChild);
        //
    },
    resetAjax: function() {
        var self = CobaCourses;
        if(self.runner) {
            self.runner.abort();
            self.runner = None;
        }
        if(self.live)
            hide("search-spinner");
        self.runner = Ajax.getTransport()
    },
    setPage: function(i) {
        var self = CobaCourses;
        self.page = i;
        self.goUrl();
    }
}
//CobaCourses.resetAjax();

function hide(id) {
    el = getEl(id);
    if(el.style.display != "none")
        el.style.display="none";
}
function show(id) {
    el = getEl(id);
    if(el.style.display != "")
        el.style.display="";
}

function getEl(n) {
    el = document.getElementById(n);
    if(!el) {
        Debug.trace("Couldn't grab element " + n);
        alert("Bad " + n);
        return;
    }
    return el;
    
}
function watchEl(n, fn) {
    el = getEl(n);
    if(el.getAttribute("type") == "text")
        addEvent(el, "keyup", fn);
    addEvent(el, "change", fn);
    addEvent(el, "click", fn);
}

function getvar(x) {
    var el = getEl(x);
    return el.value;
}
function setvar(x,nv) {
    var el = getEl(x);
    el.value = nv;
}

function addPageLink(pager, i, link) {
    a = document.createElement("A");
    a.setAttribute("title", "Page " + i);
    a.setAttribute("id", "pager_" + i); /* is this the right way? */
    a.setAttribute("href", link);
    eval("a.onclick = function(e) { CobaCourses.setPage("+i+"); return false; };");
    a.appendChild(document.createTextNode(i));
    pager.appendChild(a);
}


function assert(fact, why) {
	if(!fact) {
		var msg="Assert failure: " + why + "; ";
		if(arguments.callee.caller != null) {
			msg = msg + " in function " + arguments.callee.caller.toString().match(/function\s+(\w+)/)[1];
		}
		alert(msg);
	}
}

Debug.init();
/* Initialization Stuff */
//addEvent(window, "load", Debug.init);
addEvent(window, "load", CobaCourses.init);
if(Ajax.getTransport()) {
    document.write("<style type=\"text/css\">.jshide {display: none;}</style>\n");
}
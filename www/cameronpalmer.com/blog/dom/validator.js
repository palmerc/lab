/*
  Javascript-powered HTML Validator
  Written by Tim Hatch
  (c) 2005
  
  "Don't be evil.  No, really."
  
  This file is very loosely licensed.  I have not seen anyone else even attempt
  to write such a parser in Javascript (though to be honest, I wrote it in PHP
  and translated it), and I would like to see it widely accepted.  Although I
  haven't seen this work in software thus far, I wish to license this under
  the CC-nc-sa license.  http://creativecommons.org/licenses/nc-sa/1.0/
  
  If you want to rip this file out and include it in a plugin or something,
  please contact me - there are some non-inconsequential bugs and if enough
  people are actually using this, I'll set up a CVS repository.

*/

function grammar() {
	//TODO: Somehow block the fictional tags "inline", "block", "all", and "attrs"
	var r = new Array();
	r["inline"] = ["strong", "em", "del", "ins", "a", "br"]; //you might also add "span"
	r["block"] = ["p", "blockquote", "ul", "ol", "dl", "h3", "h4", "h5"]; // You might also add "div", "h1", "h2"
	r["all"] = array_merge(r["inline"], r["block"]);

	r["p"]          = array_subtract(r["all"], "p");
	r["blockquote"] = array_subtract(r["all"], "blockquote");
	r["ul"]         = ["li"];
	r["ol"]         = ["li"];
	r["li"]         = array_subtract(r["all"], "li");
	r["dl"]         = ["dt", "dd"];
	r["dt"]         = array_subtract(r["all"], "dt");
	r["dd"]         = array_subtract(r["all"], "dd");
	
	r["a"]      = array_subtract(r["inline"], "a");
	r["em"]     = array_subtract(r["inline"], "em");
	r["ins"]    = array_subtract(r["inline"], "ins");
	r["del"]    = array_subtract(r["inline"], "del");
	r["strong"] = array_subtract(r["inline"], "strong");
	r["attrs"] = new Array();
	r["attrs"]["a"] = ["href"];
	
	return r;
}

var g = grammar();

function array_merge(one, two) {
	var values = new Array();
	for (var i in one) {
		if (one[i] != null) 
			values.push(one[i]);
	}
	for (var i in two) {
		if (two[i] != null) 
			values.push(two[i]);
	}
	return values;
}

function array_subtract(one, thingie) {
	var values = new Array();
	for (var i in one) {
		if (one[i] != null && one[i] != thingie) 
			values.push(one[i]);
	}
	return values;
}

function array_search(one, thingie) {
	for (i in one) {
		if(one[i] != null && one[i] == thingie) {
			return true;
		}
	}
	return false;
}

function peek(array) {
	return array[array.length-1];
}

function validate(id) {
	var f=document.getElementById(id);
	assert(f);
	var r=document.getElementById('result');
	assert(r);
	var tree=document.getElementById('tree');
	assert(tree);
	p=document.getElementById("preview");
	assert(p);
	var t = f.value;
	r.className="bad";
	r.innerHTML="Please wait...";
	tree.innerHTML="Parse tree:<br />";
	/* TODO:
	 make it able to highlight the bad line
	 make it obey a limited subset of tags
	 make it fully "understand" attributes
	 
	 add in delay
	 
	 clean up "testing" code
	 
	 fix "shift toggles 'invalid' while bad attribute exists" bug
	 required attributes/elements (blockquote cite, a href, img alt)
	 nested tags like a/em/a which are not good
	 
	 */
	var re=0;
	var re=/<(\/?)(\w+)([^>]*?)(\/?)>([^<]*)/g;
	var res=/<(\/?)(\w+)([^>]*?)(\/?)>([^<]*)/;
	
	var rea=/\b(\w+)="(.*?)"( |$)/g;
	var reas=/\b(\w+)="(.*?)"( |$)/;
	
	var badesc=/&[^;]{6}/;
	if(v=t.match(badesc)) {
		//only do this part if we're allowed to fiddle with the pointer
/*		if(f.setSelectionRange) {
			f.setSelectionRange(v.index, v.index+1);
			f.blur();
			f.focus();
		}*/
		//alert("Invalid escape sequence at character "+v.index+". It has been selected for you.");
		r.innerHTML="Invalid escape sequence at near string "+v[0];
		p.innerHTML="No Preview (see error below)";
		return 1;
	}

	var index=0;
	var i=0;
	var stack = new Array();

	/* there is an odd even-odd bug in firefox, relating to it returning null twice.
	   there are two ways to solve this.  either perform another regexp to confuse the parser
	   and make it start over, or just loop through it twice
	  */
	//if(f.setSelectionRange) t.match(re);
	while(tags=re.exec(t)) {
		//do absolutely nothing
	}
	
	while(tags = re.exec(t)) {
		//print_r(tags);
		/*
			tags.1: closing
			tags.2: tagname
			tags.3: attributes
			tags.4: selfclosing
			tags.5: cdata
		*/
		if(tags[3].match(/</)) {
			r.innerHTML="Malformed attributes in " + escape2(tags[0]);
			p.innerHTML="No Preview (see error below)";
			return 1;
		}
		if(tags[1] == "") {
			//opening
			//verify that it's allowed
			if(stack.length==0) {
				ptag="block";
			} else {
				ptag=peek(stack);
			}
			if(!array_search(g[ptag], tags[2])) {
				//not allowed
				r.innerHTML="the tag " + tags[2] + " is not allowed within " + ptag + (ptag=="block"?" (try wrapping it with a &lt;p&gt; tag)":"");
				p.innerHTML="No Preview (see error below)";
				return 1;
			}
			
			stack.push(tags[2]);
			tree.innerHTML += str_repeat("&nbsp;",stack.length * 3) + tags[2] + "<br />";
			if(tags[4]=="/") {
				//close the sucker
				stack.pop();
			}
		} else {
			//closing
			if(stack.length==1 && trim(tags[5]).length!=0) {
				r.innerHTML="Character data is not allowed to be bare near " + tags[2] + " at character " + tags.index + " (invalid text is \"" + trim(tags[5]) + "\")";
				p.innerHTML="No Preview (see error below)";
				return 1;
			}
			if(tags[4].length!=0) {
				r.innerHTML="Tags cannot be both opening and closing near " + escape2(tags[0]) + " at character " + tags.index;
				p.innerHTML="No Preview (see error below)";
				return 1;
			}				
			
			if(!stack.length) {
				r.innerHTML="No tags open where trying to handle " + escape2(tags[0]);
				p.innerHTML="No Preview (see error below)";
				return 1;
			} else {
				var old=stack.pop();
				if(old != tags[2]) {
					//alert("Bad!");
					r.innerHTML="Invalid nesting at " + escape2(tags[0]) + " (did not match previous tag " + old + ")";
					p.innerHTML="No Preview (see error below)";
					return 1;
				}
			}
		}
		//now lets' try to get the attributes out
		while(attrs = rea.exec(tags[3])) {
			/* attrs[1] is name, attrs[2] is value */
			if(!array_search(g["attrs"][tags[2]], attrs[1])) {
				r.innerHTML="The attribute " + attrs[1] + " is not allowed for the tag " + tags[2];
				p.innerHTML="No Preview (see error below)";
				return 1;
			}
		}
		var temp = trim(tags[3].replace(reas, ""));
		if(temp.length) {
			r.innerHTML="Malformed attributes in " + escape2(tags[0]);
			p.innerHTML="No Preview (see error below)";
			return 1;
		}		
		
	}
	if(stack.length==0) {
		//good, pending checking of the remaining portions
		var temp = trim(t.replace(re, ""));
		if(temp.length == 0) {
			r.innerHTML="Good";
			r.className="good";
			p.innerHTML=t;
		} else {
			r.innerHTML="Weird (probably a barestring or malformed tag at the beginning or end, try wrapping it with a &lt;p&gt; tag)";
			p.innerHTML="No Preview (see error below)";
		}
	} else {
		r.innerHTML="Bad, forgot to close the tag &lt;"+stack.pop()+"&gt;";
		p.innerHTML="No Preview (see error below)";
	}

/*	var parts = tags[0].match(res);
	print_r(parts);*/
}

function print_r(a) {
	var msg="";
	for (var i in a) {
		msg = msg + i.toString() + "=>\"" + a[i] + "\"\n";
	}
	if(r) return msg;
	alert(msg);
}

function trim(s) {
	while (s.length > 0 && (s.indexOf(' ')==0 || s.indexOf("\n")==0 || s.indexOf("\r")==0 ) ) s = s.substr(1);
	while (s.length > 0 && (s.lastIndexOf(' ') == s.length - 1 || s.lastIndexOf("\n") == s.length - 1 || s.lastIndexOf("\r") == s.length - 1 ) ) s = s.substr(0, s.length - 1);
	return s;
}

/* Better trim from htmlarea */
String.prototype.trim = function() {
	return this.replace(/^\s+/, '').replace(/\s+$/, '');
};


function escape2(s) {
	s=s.replace(/</, "&lt;");
	return s;
}
function str_repeat(nugget, times) {
	var r="";
	while(--times >= 0) r+=nugget;
	return r;
}

function assert(fact) {
	if(!fact) {
		var msg="Assert failure";
		if(arguments.callee.caller != null) {
			msg = msg + " in function " + arguments.callee.caller.toString().match(/function\s+(\w+)/)[1];
		}
		alert(msg);
	}
}


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

addEvent(window, "load", setupValidator);
function setupValidator(e) {
	addEvent(document.getElementById("comment-data"), "keyup", validateWrapper);
	validateWrapper();
}

function validateWrapper(e) {
	validate("comment-data");
}

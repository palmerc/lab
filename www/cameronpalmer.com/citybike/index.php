<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
	"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<meta http-equiv="content-type" content="text/html; charset=utf-8"/>
		<title>CityBike Vienna</title>
		<script type="text/javascript">
	//<![CDATA[

var req;
var stationHash = new Hash();
var lang = "de";
var map;

function Hash() {
	// Constructor
	this.length = 0;
	this.items = new Array();

	for (var i = 0; i < arguments.length; i += 2) {
		if (typeof(arguments[i + 1]) != 'undefined') {
			this.items[arguments[i]] = arguments[i + 1];
			this.length++;
		}
	}
	
	this.getItem = function(key) {
		return this.items[key];
	}

	this.setItem = function(key, value) {
		if (typeof(this.items[key]) == 'undefined') {
			this.length++;
		}
		this.items[key] = value;
	}

	this.removeItem = function(key) {
		if (typeof(this.items[key]) != 'undefined') {
			this.length--;
			var tmp = this.items[key];
			delete this.items[key];
		}
		return tmp;
	}
}

String.prototype.titleCase = function() {
	var str = "";
	var words = this.split(" ");

	for (var i in words) {
		str += words[i].charAt(0).toUpperCase() + words[i].substr(1).toLowerCase();
		if (i < words.length) {
			str += " ";
		}
	}
	return str;
}
			
function loadXMLDoc(url) {
	if (window.XMLHttpRequest && !(window.ActiveXObject)) {
		try {
			req = new XMLHttpRequest();
		} catch(e) {
			req = false;
		}
	} else if (window.ActiveXObject) {
		try {
			req = new ActiveXObject("Msxml2.XMLHTTP");
		} catch(e) {
			try {
				req = new ActiveXObject('Microsoft.XMLHTTP');
			} catch(e) {
				req = false;
			}
		}
	}
	if (req) {
		req.onreadystatechange=processReqChange;
		req.open("GET", url, true);
		req.send(null);
	}
}

// function to handle asynchronous call
function processReqChange() {
	if (req.readyState==4) {
		if (req.status==200) {
			processStations();
			addStations();
		} else {
			alert("There was a problem retrieving the XML data:\n" + req.statusText);
		}
	}
}

function processStations() {
	var content = req.responseText;
	var tmpArr = content.split("\n");
	for (var i = 0; i < tmpArr.length; i++) {
		var record = tmpArr[i].split(";");
		if (record.length == 9) {
			var num = record[0];
			var s = new Object();
			s.name = record[1];
			s.titleName = s.name.titleCase();

			s.latitude = record[2];
			s.longitude = record[3];
			s.description = record[4];
			s.picture = record[5];
			s.bikes = record[6];
			s.empty = record[7];
			s.capacity = record[8]; 
			stationHash.setItem(num, s);
		}
	}
	document.getElementById("locations").innerHTML = "Processing done, " + stationHash.length;

}

function addStations() {
	var cityBikeMarker = new GIcon(G_DEFAULT_ICON);
	//cityBikeMarker.image = "http://www.google.com/intl/en_us/mapfiles/ms/micons/blue-dot.png";
               
	// Set up our GMarkerOptions object
	markerOptions = { icon:cityBikeMarker };

	// Add station markers to the map
	for (var i in stationHash.items) {
		var s = stationHash.items[i];
		s.point = new GLatLng(s.latitude, s.longitude);
		var name = '<p class="name">' + i + ' - ' + s.titleName + '</p>';
		var desc = '<p class="description">' + s.description + '</p>';
		var pic = '<img class="picture" src="http://dynamisch.citybikewien.at/include/r4_get_data.php?url=terminal/cont/img/' + s.picture + '" height="100px" width="100px" />';
		var bikes = '<p class="bikes">' + "Bikes available: " + s.bikes + '</p>';
		var empty = '<p class="empty">' + "Empty spaces: " + s.empty + '</p>';
		var capacity = '<p class="capacity">' + "Capacity: " + s.capacity + '</p>';
		var gps = '<p class="gps">' + "GPS: " + s.latitude + "," + s.longitude + '</p>';
		var html = '<div class="station">' + name + desc + pic + bikes + empty + capacity + gps + '</div>';
  		map.addOverlay(createMarker(s.point, html, markerOptions));
	}
}

function createMarker(point, html, markerOptions) {
	var marker = new GMarker(point, markerOptions);
	GEvent.addListener(marker, "click", function() {
		map.openInfoWindowHtml(point, html);
	});
	return marker;
}

function mapsLoaded() {
	if (GBrowserIsCompatible()) {
		map = new GMap2(document.getElementById("map"));
		var trafficInfo = new GTrafficOverlay();
		var center = new GLatLng(48.189365, 16.351068);
		map.setCenter(center, 14);
		map.enableScrollWheelZoom();
		map.addControl(new GLargeMapControl());
		map.addControl(new GMapTypeControl());
		map.addControl(new GScaleControl());
		map.addOverlay(trafficInfo);
	}
	loadXMLDoc("http://cameronpalmer.com/citybike/status.csv");
}

function loadMaps() {
	google.load("maps", "2", {"callback" : mapsLoaded, "language" : lang});
}

function initLoader() {
	lang = navigator.language.substr(0,2);
	var script = document.createElement("script");
	script.src = "http://maps.google.com/jsapi?key=ABQIAAAAOIETG0E0dKjOTufoxp5V2hSkKvSN7SEoe8SIEWfgQbA_uxQPiBQE8HWSKDxNcLxYG-BNErFsgTmY8g&amp;callback=loadMaps";
	script.type = "text/javascript";
	document.getElementsByTagName("head")[0].appendChild(script);
}

	//]]>
		</script>
	</head>
	<body onload="initLoader()"; onunload="GUnload()">
		<div id="map" style="height:800px; width:800px"></div>
		<p id="locations"></p>
	</body>
</html>

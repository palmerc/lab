<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
	"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<meta http-equiv="content-type" content="text/html; charset=utf-8"/>
		<title>CityBike Vienna</title>
		<script type="text/javascript">
	//<![CDATA[

var locArray = new Object;
var lang = "de";
function include(pURL) {
	if (window.XMLHttpRequest) { // code for Mozilla, Safari, ** And Now IE 7 **, etc
		xmlhttp=new XMLHttpRequest();
	} else if (window.ActiveXObject) { //IE
		xmlhttp=new ActiveXObject('Microsoft.XMLHTTP');
	}
	if (typeof(xmlhttp)=='object') {
		xmlhttp.onreadystatechange=postFileReady;
		xmlhttp.open('GET', pURL, true);
		xmlhttp.send(null);
	}
}

// function to handle asynchronous call
function postFileReady() {
	if (xmlhttp.readyState==4) {
		if (xmlhttp.status==200) {
			var content = xmlhttp.responseText;
			var tmpArr = content.split("\n");
			for (var i = 0; i < tmpArr.length; i++) {
				var halves = tmpArr[i].split(";");
				var num = halves[0];
				var latLng = halves[1];
				locArray[num] = latLng;
			}
		} else {
			alert("Help!");
		}
	}
}

function createMarker(map, point, message, markerOptions) {
	var marker = new GMarker(point, markerOptions);
	GEvent.addListener(marker, "click", function() {
		var stationHtml = "<b>" + message + "</b>";
		map.openInfoWindowHtml(point, stationHtml);
	});
	return marker;
}

function mapsLoaded() {
	if (GBrowserIsCompatible()) {
		var map = new GMap2(document.getElementById("map"));
		var center = new GLatLng(48.189365, 16.351068);
		map.setCenter(center, 14);
		map.addControl(new GLargeMapControl());
		map.addControl(new GMapTypeControl());
		map.addControl(new GScaleControl());
	
		var cityBikeMarker = new GIcon(G_DEFAULT_ICON);
		//cityBikeMarker.image = "http://www.google.com/intl/en_us/mapfiles/ms/micons/blue-dot.png";
                
		// Set up our GMarkerOptions object
		markerOptions = { icon:cityBikeMarker };

		var marker = new GMarker(center, {draggable: true, bouncy: false});
		GEvent.addListener(marker, "dragstart", function() {
			map.closeInfoWindow();
		});
		GEvent.addListener(marker, "dragend", function() { 
			marker.openInfoWindowHtml("GPS: " + marker.getPoint().toUrlValue());
		});
		map.addOverlay(marker);

		// Add station markers to the map
		//var message = "Hello"
		//for (var loc in locArray) {
		//	var latLng = locArray[loc].split(',');
		//	var lat = latLng[0];
		//	var long = latLng[1];
		//	var point = new GLatLng(lat, long);
  		//	map.addOverlay(createMarker(map, point, message, markerOptions));
		//}
	}
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

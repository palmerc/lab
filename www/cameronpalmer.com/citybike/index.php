<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
	"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<meta http-equiv="content-type" content="text/html; charset=utf-8"/>
		<title>CityBike Vienna</title>
		<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAOIETG0E0dKjOTufoxp5V2hSkKvSN7SEoe8SIEWfgQbA_uxQPiBQE8HWSKDxNcLxYG-BNErFsgTmY8g"
		type="text/javascript"></script>
		<script type="text/javascript">
	//<![CDATA[

var locArray = new Object;
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
			mapLoad()
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

function mapLoad() {
	if (GBrowserIsCompatible()) {
		var map = new GMap2(document.getElementById("map"));
		map.setCenter(new GLatLng(48.189365, 16.351068), 14);
		map.addControl(new GLargeMapControl());
		map.addControl(new GMapTypeControl());
	
		var cityBikeMarker = new GIcon(G_DEFAULT_ICON);
		//cityBikeMarker.image = "http://www.google.com/intl/en_us/mapfiles/ms/micons/blue-dot.png";
                
		// Set up our GMarkerOptions object
		markerOptions = { icon:cityBikeMarker };

		// Add station markers to the map
		var message = "Hello"
		for (var loc in locArray) {
			var latLng = locArray[loc].split(',');
			var lat = latLng[0];
			var long = latLng[1];
			var point = new GLatLng(lat, long);
  			map.addOverlay(createMarker(map, point, message, markerOptions));
		}
	}
}

function load() {
	include('http://cameronpalmer.com/citybike/location.csv');
}

	//]]>
		</script>
	</head>
	<body onload="load()" onunload="GUnload()">
		<div id="map" style="height:800px; width:800px"></div>
		<p id="locations"></p>
	</body>
</html>

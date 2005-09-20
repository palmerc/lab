<?php

header("Content-Type: text/html; charset=utf-8");
do_header();
ob_start("do_footer");
ob_start("do_content");


function do_header() {
    global $title, $sec_title;
    echo '
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>';

echo "
<title>{$title}</title>
";

echo '
    <link rel="stylesheet" type="text/css" href="c/main.css" />
    <link rel="shortcut icon" href="favicon.ico" type="image/vnd.microsoft.icon" />
    <link rel="icon" href="favicon.ico" type="image/vnd.microsoft.icon" />
    <link rel="alternate" type="application/rss+xml" title="Princess Craft Inventory" href="http://www.princesscraft.com/rss/" />
        
    <script language="Javascript" type="text/javascript">
    <!-- Hide script from old browsers
    
    sfHover = function() {
      var sfEls = document.getElementById("nav").getElementsByTagName("LI");
      for (var i=0; i<sfEls.length; i++) {
	sfEls[i].onmouseover=function() {
          this.className+=" sfhover";
        }
        sfEls[i].onmouseout=function() {
	  this.className=this.className.replace(new RegExp(" sfhover\\\\b"), "");
        }
      }
    }
    if (window.attachEvent) window.attachEvent("onload", sfHover);
    // End hiding script from old browsers -->
    </script>
</head>
<body>
<div id="wrap">
<div id="header"><img src="i/pc_header.png" width="750" height="99" alt="PrincessCraft Trailers" /></div>
<div id="navbar">
    <ul id="nav">
    
      <li>Princess Craft
        <ul>
	  <li><a href="/">Home</a></li>
	  <li><a href="contact.php">Contact Us</a></li>
	  <li><a href="links.php">Links</a></li>
	</ul>
      </li>
    
      <li>Camping Trailers
        <ul>
          <li><a href="#">Aliner Campers</a>
            <ul>
	          <li><a href="choosy/customize.php">Customize</a></li>
	          <li><a href="docs/aliner_buyer_guide.pdf">Aliner Buyer&#8217;s Guide</a></li>
              <li><a href="http://www.aliner.com">Manufacturer&#8217;s Website</a></li>
            </ul>  
          </li>
        </ul>
      </li>
      
      <li>Truck Campers
        <ul>
          <li><a href="#">Shadow Cruiser</a>
            <ul>
              <li><a href="http://www.shadowcruiser.com">Manufacturer&#8217;s Website</a></li>
            </ul>  
          </li>
          <li><a href="#">Arctic Fox</a>
            <ul>
              <li><a href="http://www.northwoodmfg.com/fox-campers.htm">Manufacturer&#8217;s Website</a></li>
            </ul>
          </li>  
          <li><a href="#">North Star</a>
            <ul>
              <li><a href="http://www.northstarcampers.com">Manufacturer&#8217;s Website</a></li>
            </ul>
          </li>
          <li><a href="#">Alpenlite</a>
            <ul>
              <li><a href="http://www.wrv.com/html/alpenlite_lim_about.html">Manufacturer&#8217;s Website</a></li>
            </ul>
          </li>
        </ul>
      </li>
      
      <li>Truck Caps
        <ul>
          <li><a href="#">Leer</a>
            <ul>
              <li><a href="http://www.leer.com">Manufacturer&#8217;s Website</a></li>
            </ul>
          </li>
          <li><a href="#">Century</a>
            <ul>
              <li><a href="http://www.centurycaps.com">Manufacturer&#8217;s Website</a></li>
            </ul>
          </li>
        </ul>
      </li>
            
      <li>Accessories
        <ul>
          <li><a href="#">SmittyBilt</a>
            <ul>
              <li><a href="http://www.smittybiltinc.com">Manufacturer&#8217;s Website</a></li>
            </ul>
          </li>
          <li><a href="#">Lund</a>
            <ul>
              <li><a href="http://www.lundlook.com">Manufacturer&#8217;s Website</a></li>
            </ul>
          </li>
          <li><a href="#">Deflecta</a>
            <ul>
              <li><a href="http://www.deflectashield.com">Manufacturer&#8217;s Website</a></li>
            </ul>
          </li>
          <li><a href="#">DeeZee</a>
            <ul>
              <li><a href="http://www.deezee.com">Manufacturer&#8217;s Website</a></li>
            </ul>
          </li>
          <li><a href="#">Auto Ventshade</a>
            <ul>
              <li><a href="http://www.autoventshade.com">Manufacturer&#8217;s Website</a></li>
            </ul>
          </li>
        </ul>  
      </li>
      
    </ul> <!-- End of #nav -->
  <div id="navbar_widener"></div> <!-- End of #navbar_widener -->
</div> <!-- Endo of #navbar -->
';

echo "
<div id=\"content\"><div id=\"content_inner\"><div id=\"content_inner_header\"><h2>{$sec_title}</h2></div>
";

}

function do_footer($buf) {
    return $buf.'
<div id="footer">
    <address>
        Copyright &copy; 2005 Princess Craft Manufacturing, Inc.<br />
        102 N 1st St, Pflugerville, TX 78660-2754<br />
        Phone: (512) 251-4536, Toll Free: (800) 338-7123, Fax: (512) 251-3134<br />
        sales@princesscraft.com
    </address>
    <ul>
    	<li><a href="http://validator.w3.org/check?uri=referer">XHTML 1.0</a>/</li>
    	<li><a href="http://jigsaw.w3.org/css-validator/check/referer">CSS 2.0</a>/</li>
    	<li><a title="RSS 2.0" href="/rss/" id="rssbutton">RSS 2.0</a></li>
    </ul>
</div> <!-- end div -->
</div> <!-- end wrap -->
</body>
</html>
';

}

function do_content($buf) {
return $buf.'</div></div>
<div id="sidebar">
  <div id="rvtrader"><a href="http://www.rvtrader.com/rv_ur_list.php?uid=29388">Click here to See Our Current Inventory!</a></div>

<!--<div id="img">-->
<img src="i/aliner_icon.png" height="87" width="110" alt="Aliner Pop-Up Camping Trailer" />

<img src="i/campers.png" height="87" width="110" alt="Truck Campers" />

<img src="i/caps.png" height="87" width="110" alt="Truck Caps" />

<img src="i/access.png" height="87" width="110" alt="Truck Accessories" />
<!--</div>-->

</div>
<div class="fixit">&nbsp;</div><!-- get past the two columns -->
';

}

?>

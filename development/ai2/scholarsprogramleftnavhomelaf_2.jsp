<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 

<%@ page contentType="text/html; charset=UTF-8" %>
<%@ page import = "com.ibm.drit.acme.cda.common.*" %>
<%@ page import = "com.ibm.drit.acme.portal.cma.contentgeneration.html.HtmlComposer" %>
<%@ page import = "com.ibm.drit.acme.portal.cma.contentgeneration.composer.CompositeDoc" %>
<%@ page import = "com.ibm.drit.acme.portal.cma.contentgeneration.composer.CompositionConstants" %>
<%@ page import = "com.ibm.drit.acme.portal.common.ContentMBean" %>
<%@ page import = "com.ibm.drit.acme.portal.common.MetaContentMBean" %>
<%@ page import = "com.ibm.drit.acme.portal.cma.contentcreation.util.*" %>

<jsp:useBean id="components" class="com.ibm.drit.acme.portal.common.cda.LafComponentBean" scope="request"><jsp:setProperty name="components" property="id"/></jsp:useBean>


<%
	/*
      String content_id = request.getParameter("content_id");
	ContentStorageAndRetrieval csar = new ContentStorageAndRetrieval();
	ContentMBean contentBean = csar.retrieveContent(content_id);	
      MetaContentMBean  metaBean = contentBean.getMetaContent();
	byte[] bodyByteArray = contentBean.getContent();
	String body = new String(bodyByteArray);
	int nav_id = metaBean.getNavId();
      StringBuffer htmlMetaTags = metaBean.getHtmlMetaTags();
      */

	NavBar navBar = new NavBar();
	//navBar.setNavId(nav_id);
      navBar.setNavId(Integer.parseInt(components.getNavId()));

      //------------------------------define all the navigation styles------------------ 
	navBar.setDividerRow(
		"\n<!-- <tr bgcolor=\"#99ccff\">\n"
		+	"  <td colspan=\"4\"><img src=\"/g/1cp.gif\" alt=\"\" width=\"1\" height=\"8\"></td>\n"
		+	"</tr> --> \n\n"
	);
	navBar.setCategoryRow(
		"<tr>\n"
		+ "	<td class=\"left-nav\" colspan=\"2\"><a class=\"left-nav\" href=\"<!-- acme:nav.url -->\"><!-- acme:nav.name --></a></td>\n"
		+ "</tr>\n"
	);	
	navBar.setSelectedCategoryRow(
		"<tr>\n"
		+ "	<td class=\"left-nav-highlight\" colspan=\"2\"><a class=\"left-nav\" href=\"<!-- acme:nav.url -->\"><!-- acme:nav.name --></a></td>\n"
		+ "</tr>\n"
	);	
	navBar.setOpenCategoryRow(
		"<tr>\n"
		+ "	<td class=\"left-nav\" colspan=\"2\"><a class=\"left-nav\" href=\"<!-- acme:nav.url -->\"><!-- acme:nav.name --></a></td>\n"
		+ "</tr>\n"
	);
	navBar.setSubCategoryRow(
		"<tr class=\"left-nav-child\">\n"
		+ "	<td><img src=\"//www.ibm.com/i/v14/t/cl-bullet.gif\" width=\"2\" height=\"8\" alt=\"\" border=\"0\" /></td>\n"
		+ "	<td><a class=\"left-nav-child\" href=\"<!-- acme:nav.url -->\"><!-- acme:nav.name --></a></td>\n"
		+ "</tr>\n"
	);
	navBar.setSelectedSubCategory(
	
		"<tr class=\"left-nav-child-highlight\">\n"
		+ "	<td><img src=\"//www.ibm.com/i/v14/t/cl-bullet.gif\" width=\"2\" height=\"8\" alt=\"\" border=\"0\" /></td>\n"
		+ "	<td><a class=\"left-nav-child\" href=\"<!-- acme:nav.url -->\"><!-- acme:nav.name --></a></td>\n"
		+ "</tr>\n"
	);
	navBar.setSubCatDividerRow(
		"<!-- <tr bgcolor=\"#cce5ff\">\n"
		+	"  <td colspan=\"4\"><img src=\"/g/1cp.gif\" alt=\"\" width=\"1\" height=\"3\"></td>\n"
		+	"</tr> -->\n"
	);


       navBar.setThirdCategoryRow("<span class=\"small\"><a href=\"<!-- acme:nav.url -->\">" 
                                            +"<!-- acme:nav.name --></a></span>" 
                                      	    +"&nbsp;<span style=\"color: #999999\">|</span>");

	navBar.setFourthCategoryRow("<span class=\"small\"><a href=\"<!-- acme:nav.url -->\">" 
                                            +"<!-- acme:nav.name --></a></span>" 
                                      	    +"&nbsp;<span style=\"color: #999999\">|</span>");


    	
   //-------------------call to build the navigation-------------
   navBar.build();

   
   //--------------get everything out of the nav bar ------------------------
	String header          = navBar.getHeaderName();
      String leftNav = navBar.getNavBar();
      String selectedNav     = navBar.getNavigationSelectedName();
      String crumbs          = navBar.getBreadCrumbs();
      String thirdRowNav     = navBar.getThirdCategory();
	String fourthRowNav    = navBar.getFourthCategory();
   
	out.println( "<!-- NavId: " + navBar.getNavId() + "-->" );
		
%>


<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en-US" lang="en-US">
<head>
<%//=htmlMetaTags%>
<jsp:getProperty name="components" property="meta" />
<jsp:getProperty name="components" property="postMeta" />

<!-- ibm.com V14 OneX css -->
<link rel="stylesheet" type="text/css" href="//www.ibm.com/common/v14/main.css" />
<link rel="stylesheet" type="text/css" media="all" href="//www.ibm.com/common/v14/screen.css" />
<link rel="stylesheet" type="text/css" media="screen,print" href="//www.ibm.com/common/v14/table.css" />
<link rel="stylesheet" type="text/css" media="print" href="//www.ibm.com/common/v14/print.css" />
<script type="text/javascript" language="JavaScript" src="//www.ibm.com/common/v14/detection.js"></script>

<!-- legacy pwd css -->
<link rel="stylesheet" type="text/css" href="/applet/css/pwd.css" />
<link rel="SHORTCUT ICON" href="http://www.ibm.com/favicon.ico" />

<!--
<style type="text/css">
	P,ul,li,ol {
		font-size : 9pt;
		font-family : Arial, sans-serif;
	}
</style>
-->

<!-- Bring in Flash detection script  -->
<script language="JavaScript">
<!--
function MM_openBrWindow(theURL,winName,features) { //v2.0
  window.open('jct09002c'+theURL,winName,features);
}
//-->
</script>
<!-- End Flash detection script  -->


</head>
<body alink="#0000ff" bgcolor="#ffffff" leftmargin="2" topmargin="2" marginwidth="2" marginheight="2">
<!-- BEGIN MASTHEAD FILE -->
<table width="760" cellspacing="0" cellpadding="0" border="0">
<tr valign="top">
<td width="110" class="bbg"><a href="http://www.ibm.com/us"><img width="110" src="//www.ibm.com/i/v14/t/ibm-logo.gif" height="52" border="0" alt="IBM"/></a></td><td width="650" class="mbbg" align="right">
<table align="right" cellspacing="0" cellpadding="0" border="0">
<tr class="cty-tou">
<td class="upper-masthead-corner" width="17" rowspan="2"><a href="#main"><img alt="Skip to main content" height="1" width="1" border="0" src="//www.ibm.com/i/c.gif"/></a></td><td align="left">
<table align="left" cellspacing="0" cellpadding="0" border="0">
<tr>
<td><span class="spacer">&nbsp;&nbsp;&nbsp;&nbsp;</span><b class="country">Country/region</b><span class="spacer">&nbsp;[</span><a href="http://www.ibm.com/planetwide/select/selector.html" class="ur-link">select</a><span class="spacer">]</span></td><td class="upper-masthead-divider" width="29">&nbsp;&nbsp;&nbsp;&nbsp;</td><td align="left"><a href="http://www.ibm.com/legal/" class="ur-link">Terms of use</a></td>
</tr>
</table>
</td><td width="40">&nbsp;</td>
</tr>
<tr>
<td colspan="2" height="1" class="cty-tou-border"><img src="//www.ibm.com/i/c.gif" width="1" height="1" alt="" border="0"/></td>
</tr>
<tr>
<td colspan="3"><img src="//www.ibm.com/i/c.gif" width="1" height="8" alt="" border="0"/></td>
</tr>
<tr>
<td>&nbsp;</td><td colspan="2" align="center">
<form title="Search form" name="search-form" method="get" action="http://www.ibm.com/Search">
<table cellspacing="0" cellpadding="0" border="0">
<tr>
<td width="1"><label for="q"><img alt="Search for:" height="1" width="1" src="//www.ibm.com/i/c.gif" border="0"/></label></td><td align="right"><input value="" size="15" name="q" maxlength="100" id="q" class="input" type="text"/></td><td width="7">&nbsp;<input type="hidden" name="v" value="11"/><input type="hidden" name="lang" value="en"/><input type="hidden" name="cc" value="zz"/></td><td><input type="image" src="//www.ibm.com/i/v14/t/zz/en/search.gif" name="Search" value="Search" alt="Search" width="71" height="18"/></td><td width="20">&nbsp;</td>
</tr>
</table>
</form>
</td>
</tr>
</table>
</td>
</tr>
<tr>
<td colspan="2" class="blbg">
<table width="760" cellspacing="0" cellpadding="0" border="0">
<tr>
<td>
<table cellspacing="0" cellpadding="0" border="0">
<tr>
<td><span class="spacer">&nbsp;&nbsp;&nbsp;&nbsp;</span></td><td><a href="http://www.ibm.com/" class="masthead-mainlink">Home</a></td><td width="27" class="masthead-divider">&nbsp;&nbsp;&nbsp;&nbsp;</td><td><a href="http://www.ibm.com/products/" class="masthead-mainlink">Products</a></td><td width="27" class="masthead-divider">&nbsp;&nbsp;&nbsp;&nbsp;</td><td><a href="http://www.ibm.com/servicessolutions/" class="masthead-mainlink">Services &amp; solutions</a></td><td width="27" class="masthead-divider">&nbsp;&nbsp;&nbsp;&nbsp;</td><td><a href="http://www.ibm.com/support/" class="masthead-mainlink">Support &amp; downloads</a></td><td width="27" class="masthead-divider">&nbsp;&nbsp;&nbsp;&nbsp;</td><td><a href="http://www.ibm.com/account/" class="masthead-mainlink">My account</a></td><td><span class="spacer">&nbsp;&nbsp;&nbsp;&nbsp;</span></td>

</tr>
</table>
<!--</td><td align="right"><span class="masthead-phone">Call&nbsp;1-800-SHOP-IBM&nbsp;&nbsp;&nbsp;&nbsp;</span></td>-->
</tr>
</table>
</td>
</tr>
</table>
<script src="//www.ibm.com/common/v14/pmh.js" language="JavaScript" type="text/javascript"></script>
<!-- END MASTHEAD FILE -->

<!-- PWD V11 SEARCH CODE  -->
<!--<jsp:include page="/application/octet-stream/a/us/en/university/scholars/spsearchboxlaf.jsp" flush="true"/>-->

<!-- BEGIN Main page table -->
<table width="760" border="0" cellspacing="0" cellpadding="0" id="v14-body-table">
<tbody>
<tr valign="top">
<td width="150" id="navigation">
<table border="0" cellpadding="0" cellspacing="0" width="150">
<tr>
	<td class="left-nav-spacer">&nbsp;</td>
</tr>
</table>
<table width="150" border="0" cellspacing="0" cellpadding="0">
<tr>
	<td class="left-nav-overview" colspan="2"><a class="left-nav-overview" href="/us/en/university/scholars/">IBM Academic Initiative</a></td>
</tr>
<tr>
	<td class="left-nav" colspan="2"><a class="left-nav" href="/us/en/university/scholars/products/">Products &amp; solutions</a></td>
</tr>
<tr>
	<td class="left-nav" colspan="2"><a class="left-nav" href="/us/en/university/scholars/downloads/">Downloads &amp; CDs</a></td>
</tr>
<tr>
	<td class="left-nav" colspan="2"><a class="left-nav" href="/us/en/university/scholars/training/classroom.html">Training</a></td>
</tr>
<tr>
	<td class="left-nav" colspan="2"><a class="left-nav" href="/us/en/university/scholars/courseware/">Curriculum &amp; courseware</a></td>
</tr>
<tr>
	<td class="left-nav" colspan="2"><a class="left-nav" href="/us/en/university/scholars/resources/">Forums and community</a></td>
</tr>
<tr>
	<td class="left-nav" colspan="2"><a class="left-nav" href="/us/en/university/scholars/certification/">Certification</a></td>
</tr>
<tr>
	<td class="left-nav" colspan="2"><a class="left-nav" href="/us/en/university/scholars/library/">Library</a></td>
</tr>
<tr>
	<td class="left-nav" colspan="2"><a class="left-nav" href="/us/en/university/scholars/support/">Support</a></td>
</tr>
<tr>
	<td class="left-nav" colspan="2"><a class="left-nav" href="/us/en/university/scholars/news/">News &amp; events</a></td>
</tr>
<tr>
	<td class="left-nav" colspan="2"><a class="left-nav" href="/us/en/university/scholars/members/">Membership</a></td>
</tr>
<tr>
	<td class="left-nav" colspan="2"><a class="left-nav" href="/us/en/university/scholars/feedback/">Feedback</a></td>
</tr>
<tr class="left-nav-last">
	<td width="14"><img src="//www.ibm.com/i/c.gif" width="14" height="1" alt="" class="display-img" /></td>
	<td width="136"><img src="//www.ibm.com/i/v14/t/left-nav-corner.gif" width="136" height="19" alt="" class="display-img" /></td>
</tr>
</table>
<br />
<table border="0" cellpadding="0" cellspacing="0" width="150">
<tr>
	<td colspan="2" class="related"><b class="related">Related links:</b></td>
</tr>

<tr class="rlinks">
	<td><img src="//www.ibm.com/i/v14/t/rl-bullet.gif" width="2" height="8" alt="" border="0"/></td>
	<td><a href="/us/en/university/students/" class="rlinks">Student Portal</a></td>
</tr>
<tr class="rlinks">
	<td><img src="//www.ibm.com/i/v14/t/rl-bullet.gif" width="2" height="8" alt="" border="0"/></td>
	<td><a href="http://www.ibm.com/industries/education/doc/jsp/indseg/highereducation/" class="rlinks">Higher education resources</a></td>
</tr>

<tr class="rlinks">
	<td><img src="//www.ibm.com/i/v14/t/rl-bullet.gif" width="2" height="8" alt="" border="0"/></td>
	<td><a href="http://www.ibm.com/developerworks/" class="rlinks">developerWorks resource for developers</a></td>
</tr>
<tr class="rlinks">
	<td><img src="//www.ibm.com/i/v14/t/rl-bullet.gif" width="2" height="8" alt="" border="0"/></td>
	<td><a href="http://www.ibm.com/alphaworks" class="rlinks">alphaWorks emerging technologies</a></td>
</tr>
<tr class="rlinks">
	<td><img src="//www.ibm.com/i/v14/t/rl-bullet.gif" width="2" height="8" alt="" border="0"/></td>
	<td><a href="/us/en/university/scholars/it/index.html" class="rlinks">IBM per l'Universit&agrave;<br />Italia</a></td>
</tr>
<tr class="rlinks">
	<td><img src="//www.ibm.com/i/v14/t/rl-bullet.gif" width="2" height="8" alt="" border="0"/></td>
	<td><a href="/us/en/university/scholars/pt/index.html" class="rlinks">IBM para as Universidades<br />Portugu&ecirc;s</a></td>
</tr>
<tr class="rlinks">
	<td><img src="//www.ibm.com/i/v14/t/rl-bullet.gif" width="2" height="8" alt="" border="0"/></td>
	<td><a href="//www.ibm.com/de/ibm/unternehmen/university_relations/index.html" class="rlinks">
	IBM Wissenschafts<br />beziehungen in Deutschland</a></td>
</tr>
<tr class="rlinks">
	<td><img src="//www.ibm.com/i/v14/t/rl-bullet.gif" width="2" height="8" alt="" border="0"/></td>
	<td><a href="//www.ibm.com/cn/ai" class="rlinks">IBM Academic Initiative<br />China</a></td>
</tr>
<tr class="rlinks">
    <td><img src="//www.ibm.com/i/v14/t/rl-bullet.gif" width="2" height="8" alt="" border="0"/></td>
    <td><a href="//www.ibm.com/ru/software/info/students/" class="rlinks">IBM Academic Initiative<br />Программа</a></td>
</tr>
<tr class="rlinks">
    <td><img src="//www.ibm.com/i/v14/t/rl-bullet.gif" width="2" height="8" alt="" border="0"/></td>
    <td><a href="//www.ibm.com/in/university/" class="rlinks">IBM Academic Initiative<br />India</a></td>
</tr>
<tr>
	<td width="14"><img src="//www.ibm.com/i/c.gif" width="14" height="1" alt="" class="display-img"/></td>
	<td width="136"><img src="//www.ibm.com/i/c.gif" width="136" height="19" alt="" class="display-img"/></td>
</tr>
</table>
<table border="0" cellpadding="0" cellspacing="0" width="150">
<tr>
	<td colspan="2" class="related"></td>
</tr>
<tr>
	<td width="14"><img src="//www.ibm.com/i/c.gif" width="14" height="1" alt="" class="display-img"/></td>
	<td width="136"><img src="//www.ibm.com/i/c.gif" width="136" height="19" alt="" class="display-img"/></td>
</tr>

</table>
<br />&nbsp;

<!-- blank for outage -->		
<!-- generate print version of page -->				
<!--
<table border="0" cellspacing="0" cellpadding="0">
<tr valign="middle">
<td width="8">&nbsp;</td>
<td width="10">&nbsp;</td>
</tr>
</table>
-->
</td>
<!-- begin rt content area -->
           
<td width="610" valign="top">

<!-- ------------------ -->
<!-- BEGIN CONTENT BODY -->			
<!-- ------------------ -->

	<%//=body%>
	<jsp:getProperty name="components" property="body" />

<!-- ---------------- -->
<!-- END CONTENT BODY -->			
<!-- ---------------- -->

</td>
</tr>
</table>


<table width="760" cellspacing="0" cellpadding="0" border="0">
<tr>
	<td height="19" class="bbg">
	<table cellspacing="0" cellpadding="0" border="0">
	<tr>
		<td><span class="spacer">&nbsp;&nbsp;&nbsp;&nbsp;</span><a href="http://www.ibm.com/ibm/" class="mainlink">About IBM</a></td>
		<td width="27" class="footer-divider">&nbsp;&nbsp;&nbsp;&nbsp;</td>
		<td><a href="http://www.ibm.com/privacy/" class="mainlink">Privacy</a></td>
		<td width="27" class="footer-divider">&nbsp;&nbsp;&nbsp;&nbsp;</td>
		<td><a href="http://www.ibm.com/contact/" class="mainlink">Contact</a></td>
	</tr>
	</table>
	</td>
</tr>
</table>

<!-- surfaid code include -->
<script type="text/javascript" language="JavaScript1.2" src="//www.ibm.com/common/stats/stats.js"></script>
<noscript><img src="//i.ihost.com/i/c.gif" width="1" height="1" alt="" border="0" /></noscript>
<script src="http://www.google-analytics.com/urchin.js" type="text/javascript">
</script>
<script type="text/javascript">
_uacct = "UA-68677-3";
urchinTracker();
</script>
</body>
</html>



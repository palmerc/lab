<%@ taglib uri="http://java.sun.com/jsp/jstl/sql" prefix="sql" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>

<sql:query var="rs" dataSource="jdbc/IBMDB">
SELECT * FROM news
    WHERE (publish IS TRUE) AND (start_date <= CURDATE()) AND (end_date >= CURDATE())
</sql:query>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
   "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
    <head>
        <title>IBM Academic Initiative News</title>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <!-- ibm.com V14 OneX css -->
        <link rel="stylesheet" type="text/css" href="//www.ibm.com/common/v14/main.css" />
        <link rel="stylesheet" type="text/css" media="all" href="//www.ibm.com/common/v14/screen.css" />
        <link rel="stylesheet" type="text/css" media="screen,print" href="//www.ibm.com/common/v14/table.css" />
        <link rel="stylesheet" type="text/css" media="print" href="//www.ibm.com/common/v14/print.css" />
        <link rel="alternate" title="IBM Academic Initiative RSS" href="ibm_ai_rss.jsp" type="application/rss+xml" />

        <!-- legacy pwd css -->
        <link rel="stylesheet" type="text/css" href="/jct09002c/applet/css/pwd.css" />
        <link rel="SHORTCUT ICON" href="http://www.ibm.com/favicon.ico" />

        <!--
        <style type="text/css">
            P,ul,li,ol {
                font-size : 9pt;
                font-family : Arial, sans-serif;
            }
        </style>
        -->
    </head>
    <body>
        <table border="0" cellpadding="0" cellspacing="0" width="443">
            <tr>
                <td class="v14-header-1">Top stories</td>
            </tr>
            <tr>
                <td>
                    <table border="0" cellpadding="0" cellspacing="0" class="v14-gray-table-border">
                        <tr>
                            <td width="443">
                                <table border="0" cellpadding="0" cellspacing="0" width="430">
                                <c:forEach var="row" items="${rs.rows}">
                                    <tr valign="top">
                                        <td align="right" width="18" class="ipt"><img alt="" height="16" src="//www.ibm.com/i/v14/icons/fw_bold.gif" width="16"/></td>
                                        <td class="nlbp">
                                            <p><b><a class="fbox" href="${row.link}">${row.headline}</a></b>
                                            ${row.summary}
                                        </td>			
                                    </tr>
                                </c:forEach>
                                    <tr>
                                        <td colspan="2"><img src="//www.ibm.com/i/c.gif" border="0" height="5" width="1" alt=""/></td>
                                    </tr>

                                    <tr>
                                        <td colspan="2"><img src="//www.ibm.com/i/c.gif" border="0" height="5" width="1" alt=""/></td>
                                    </tr>
                                </table>
                            </td>
                        </tr>
                    </table>
                </td>
            </tr>
        </table>
    </body>
</html>

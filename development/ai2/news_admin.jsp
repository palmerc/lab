<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/sql" prefix="sql" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/fmt" prefix="fmt" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
   "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
    <head>
        <title>
            IBM Academic Initiative News Admin
        </title>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <!-- ibm.com V14 OneX css -->
        <link rel="stylesheet" type="text/css" href="//www.ibm.com/common/v14/main.css" />
        <link rel="stylesheet" type="text/css" media="all" href="//www.ibm.com/common/v14/screen.css" />
        <link rel="stylesheet" type="text/css" media="screen,print" href="//www.ibm.com/common/v14/table.css" />
        <link rel="stylesheet" type="text/css" media="print" href="//www.ibm.com/common/v14/print.css" />
    </head>
    <body>
        <h1>IBM Academic Initiative News Admin</h1>
        <form action="store.jsp" method="post">
            <input type="hidden" name="type" value="create" />
            <input type="submit" value="New Story" />
        </form>
        <form action="" method="post">
            <input type="submit" value="Change Status" />
            <hr />
            <h2>Active News Stories</h2>
            <sql:query var="rs" dataSource="jdbc/IBMDB">
            SELECT * FROM news WHERE publish IS TRUE
            </sql:query>
            <c:if test="${!empty rs.rows}">
            <table>
                <tr>
                    <th>Date</th>
                    <th>Headline</th>
                    <th>Status</th>
                    <th>Start Date</th>
                    <th>End Date</th>
                </tr>
            <c:forEach var="row" items="${rs.rows}">
                <tr>
                    <td>${row.publish_date}</td>
                    <td><a href="store.jsp?type=edit&amp;newsid=${row.newsid}">${row.headline}</a></td>
                    <td>
                        <select name="${row.newsid}">
                            <option selected>active</option>
                            <option>archive</option>
                        </select>
                    </td>
                    <td>
                        <input type="text" size="10" value="${row.start_date}" />
                    </td>
                    <td>
                        <input type="text" size="10" value="${row.end_date}" />
                    </td>
                </tr>
            </c:forEach>
            </table>
            </c:if>
            <hr />
            <h2>Archived News Stories</h2>
            <sql:query var="rs" dataSource="jdbc/IBMDB">
            SELECT * FROM news WHERE publish IS FALSE
            </sql:query>
            <c:if test="${!empty rs.rows}">
            <table>
                <tr>
                    <th>Date</th>
                    <th>Headline</th>
                    <th>Status</th>
                    <th>Start Date</th>
                    <th>End Date</th>
                </tr>
            <c:forEach var="row" items="${rs.rows}">
                <tr>
                    <td>${row.publish_date}</td>
                    <td><a href="store.jsp?type=edit&amp;newsid=${row.newsid}">${row.headline}</a></td>
                    <td>
                        <select name="${row.newsid}">
                            <option>active</option>
                            <option selected>archive</option>
                        </select>
                    </td>
                    <td>
                        <input type="text" size="10" value="${row.start_date}" />
                    </td>
                    <td>
                        <input type="text" size="10" value="${row.end_date}" />
                    </td>
                </tr>
            </c:forEach>
            </table>
            </c:if>
            <input type="submit" value="Change Status" />
        </form>
        <form action="store.jsp" method="post">
            <input type="hidden" name="type" value="create" />
            <input type="submit" value="New Story" />
        </form>

    </body>
</html>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/sql" prefix="sql" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/fmt" prefix="fmt" %>
<%@ page contentType="text/html; charset=UTF-8" %>
<?xml version="1.0" encoding="utf-8"?>
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
        <h1>IBM Academic Initiative Marketing Admin</h1>
        <form action="store.jsp" method="post">
            <input type="hidden" name="type" value="create" />
            <input type="submit" value="New Marketing" />
        </form>
            <hr />
            <h2>Active Marketing</h2>
            <sql:query var="rs" dataSource="jdbc/IBMDB">
            SELECT marketing.idMarketing,
                marketing.dateMarketing,
                marketing.introductionMarketing,
                marketing.productNameMarketing,
                marketing.productDescriptionMarketing,
                marketing.cwLinkMarketing,
                marketing.swLinkMarketing,
                brands.nameBrands,
                brands.linkBrands,
                brands.logoBrands
            FROM marketing, brands 
            WHERE (marketing.idBrands=brands.idBrands)
                AND (marketing.publishMarketing IS TRUE)
            </sql:query>
            <c:if test="${!empty rs.rows}">
            <table>
                <tr>
                    <th>Date</th>
                    <th>Product Name</th>
                    <th>Status</th>
                    <th>Brand</th>
                </tr>
            <c:forEach var="row" items="${rs.rows}">
                <tr>
                    <td>${row.dateMarketing}</td>
                    <td><a href="store.jsp?type=edit&amp;idMarketing=${row.idMarketing}">${row.productNameMarketing}</a></td>
                    <td>
                        active
                    </td>
                    <td>
                        ${row.nameBrands}
                    </td>
                </tr>
            </c:forEach>
            </table>
            </c:if>
            <hr />
            <h2>Archived Marketing</h2>
            <sql:query var="rs" dataSource="jdbc/IBMDB">
            SELECT marketing.idMarketing,
                marketing.dateMarketing,
                marketing.introductionMarketing,
                marketing.productNameMarketing,
                marketing.productDescriptionMarketing,
                marketing.cwLinkMarketing,
                marketing.swLinkMarketing,
                brands.nameBrands,
                brands.linkBrands,
                brands.logoBrands
            FROM marketing, brands 
            WHERE (marketing.idBrands=brands.idBrands)
                AND (marketing.publishMarketing IS FALSE)
            </sql:query>
            <c:if test="${!empty rs.rows}">
            <table>
                <tr>
                    <th>Date</th>
                    <th>Product Name</th>
                    <th>Status</th>
                    <th>Brand</th>
                </tr>
            <c:forEach var="row" items="${rs.rows}">
                <tr>
                    <td>${row.dateMarketing}</td>
                    <td><a href="store.jsp?type=edit&amp;idMarketing=${row.idMarketing}">${row.productNameMarketing}</a></td>
                    <td>
                        archived
                    </td>
                    <td>
                        ${row.nameBrands}
                    </td>
                </tr>
            </c:forEach>
            </table>
            </c:if>
        <form action="store.jsp" method="post">
            <input type="hidden" name="type" value="create" />
            <input type="submit" value="New Marketing" />
        </form>

    </body>
</html>
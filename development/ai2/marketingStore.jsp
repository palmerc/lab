<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
    <head>
        <title>Creating Marketing Item</title>
        <!-- ibm.com V14 OneX css -->
        <link rel="stylesheet" type="text/css" href="//www.ibm.com/common/v14/main.css" />
        <link rel="stylesheet" type="text/css" media="all" href="//www.ibm.com/common/v14/screen.css" />
        <link rel="stylesheet" type="text/css" media="screen,print" href="//www.ibm.com/common/v14/table.css" />
        <link rel="stylesheet" type="text/css" media="print" href="//www.ibm.com/common/v14/print.css" />
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    </head>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/sql" prefix="sql" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/fmt" prefix="fmt" %>
<%@ page contentType="text/html; charset=UTF-8" %>

<fmt:parseDate value="${param.dateMarketing}" var="parseDateMarketing" pattern="yyyy-MM-dd" />
<jsp:useBean id="now" class="java.util.Date" />

<c:choose>
    <c:when test="${param.type == 'create'}">
    <body>
        <form action="marketingStore.jsp" method="post">
            <input type="hidden" name="type" value="insert" />
            <table>
                <tr>
                    <td>
                        idMarketing
                    </td>
                    <td>
                        NULL
                    </td>
                </tr>
                
                <tr>
                    <td>
                        Status
                    </td>
                    <td>
                        <select name="publishMarketing">
                            <option value="1">Active</option>
                            <option value="0" selected>Archive</option>
                        </select>
                    </td>
                </tr>
                <tr>
                    <td>
                        Publish Date
                    </td>
                    <td>
                        <input type="text" size="10" name="dateMarketing" />
                    </td>
                </tr>
                <tr>
                    <td>
                        Product Name
                    </td>
                    <td>
                        <input type="text" name="productNameMarketing" />
                    </td>
                </tr>
                <tr>
                    <td>
                        Product Brand
                    </td>
                    <td>
                        <input type="text" name="idBrands" />
                    </td>
                </tr>
                <tr>
                    <td>
                        Courseware Link
                    </td>
                    <td>
                        <input type="text" name="cwLinkMarketing" />
                    </td>
                </tr>
                <tr>
                    <td>
                        Software Link
                    </td>
                    <td>
                        <input type="text" name="swLinkMarketing" />
                    </td>
                </tr>
                <tr>
                    <td>
                        Introduction
                    </td>
                    <td>
                        <input type="text" name="introductionMarketing" value="What is..." />
                    </td>
                </tr>
                <tr>
                    <td>
                        Product Description
                    </td>
                    <td>
                        <textarea name="productDescriptionMarketing" cols="40" rows="20"></textarea>
                    </td>
                </tr>
            </table>
            <input type="submit" value="Submit" />
            <input type="button" value="Cancel" />
        </form>
    </body>
</html>
    </c:when>
    <c:when test="${param.type == 'edit'}">
        <sql:query var="rs" dataSource="jdbc/IBMDB">
        SELECT * FROM marketing WHERE idMarketing = ?
            <sql:param value="${param.idMarketing}" />
        </sql:query>
        <c:forEach var="row" items="${rs.rows}" >
    <body>
        <form action="marketingStore.jsp" method="post">
            <input type="hidden" name="type" value="update" />
            <input type="hidden" name="idMarketing" value="${row.idMarketing}" />
            <table>
                <tr>
                    <td>
                        idMarketing
                    </td>
                    <td>
                        ${row.idMarketing}
                    </td>
                </tr>
                <tr>
                    <td>
                        Status
                    </td>
                    <td>
                        <select name="publishMarketing">
                            <option value="1" <c:if test="${row.publish == true}">selected</c:if>>Active</option>
                            <option value="0" <c:if test="${row.publish == false}">selected</c:if>>Archive</option>
                        </select>
                    </td>
                </tr>
                <tr>
                    <td>
                        Publish Date
                    </td>
                    <td>
                        <input type="text" size="10" name="dateMarketing" value="${row.dateMarketing}" />
                    </td>
                </tr>
                <tr>
                    <td>
                        Product Name
                    </td>
                    <td>
                        <input type="text" name="productNameMarketing" value="${row.productNameMarketing}" />
                    </td>
                </tr>
                <tr>
                    <td>
                        Product Brand
                    </td>
                    <td>
                        <input type="text" name="idBrands" value="${row.idBrands}" />
                    </td>
                </tr>
                <tr>
                    <td>
                        Courseware Link
                    </td>
                    <td>
                        <input type="text" name="cwLinkMarketing" value="${row.cwLinkMarketing}" />
                    </td>
                </tr>
                <tr>
                    <td>
                        Software Link
                    </td>
                    <td>
                        <input type="text" name="swLinkMarketing" value="${row.swLinkMarketing}" />
                    </td>
                </tr>
                <tr>
                    <td>
                        Introduction
                    </td>
                    <td>
                        <input type="text" name="introductionMarketing" value="${row.introductionMarketing}" />
                    </td>
                </tr>
                <tr>
                    <td>
                        Product Description
                    </td>
                    <td>
                        <textarea name="productDescriptionMarketing" cols="40" rows="20">${row.productDescriptionMarketing}</textarea>
                    </td>
                </tr>
            </table>
            <input type="submit" value="Submit" />
            <input type="button" value="Cancel" />
        </form>
    </body>
</html>
        </c:forEach>
    </c:when>

    <c:when test="${param.type == 'insert'}">        
        <sql:update dataSource="jdbc/IBMDB">
           INSERT INTO marketing (idMarketing,
                dateMarketing,
                publishMarketing,
                productNameMarketing,
                swLinkMarketing,
                cwLinkMarketing,
                introductionMarketing,
                productDescriptionMarketing)
           VALUES (?, ?, ?, ?, ?, ?, ?, ?)
               
           <sql:param value="${null}" />
           <sql:dateParam value="${parseDateMarketing}" type="date" />
           <sql:param value="${param.publishMarketing}" />
           <sql:param value="${param.productNameMarketing}" />
           <sql:param value="${param.swLinkMarketing}" />
           <sql:param value="${param.cwLinkMarketing}" />
           <sql:param value="${param.introductionMarketing}" />
           <sql:param value="${param.productDescriptionMarketing}" />
        </sql:update>
        <c:redirect url="marketingAdmin.jsp" />
    </c:when>

    <c:when test="${param.type == 'update'}">
        <sql:update var="updateCount" dataSource="jdbc/IBMDB">
            UPDATE marketing SET 
                dateMarketing = ?, 
                publishMarketing = ?, 
                productNameMarketing = ?,
                swLinkMarketing = ?,
                cwLinkMarketing = ?,
                introductionMarketing = ?,
                productDescriptionMarketing = ?
            WHERE idMarketing = ?
               
           <sql:dateParam value="${parseDateMarketing}" type="date" />
           <sql:param value="${param.publishMarketing}" />
           <sql:param value="${param.productNameMarketing}" />
           <sql:param value="${param.swLinkMarketing}" />
           <sql:param value="${param.cwLinkMarketing}" />
           <sql:param value="${param.introductionMarketing}" />
           <sql:param value="${param.productDescriptionMarketing}" />
           <sql:param value="${param.idMarketing}" />
        </sql:update>
        <c:out value="${updateCount} rows updated." />
        <c:redirect url="marketingAdmin.jsp" />
    </c:when>
</c:choose>

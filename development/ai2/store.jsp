<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/sql" prefix="sql" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/fmt" prefix="fmt" %>

<fmt:parseDate value="${param.start_date}" var="parsedStartDate" pattern="yyyy-MM-dd" />
<fmt:parseDate value="${param.end_date}" var="parsedEndDate" pattern="yyyy-MM-dd" />
<fmt:parseDate value="${param.publish_date}" var="parsedPublishDate" pattern="yyyy-MM-dd" />
<jsp:useBean id="now" class="java.util.Date" />

<c:choose>
    <c:when test="${param.type == 'create'}">
        <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
            "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

        <html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
            <head>
                <title>Creating News Item</title>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <!-- ibm.com V14 OneX css -->
                <link rel="stylesheet" type="text/css" href="//www.ibm.com/common/v14/main.css" />
                <link rel="stylesheet" type="text/css" media="all" href="//www.ibm.com/common/v14/screen.css" />
                <link rel="stylesheet" type="text/css" media="screen,print" href="//www.ibm.com/common/v14/table.css" />
                <link rel="stylesheet" type="text/css" media="print" href="//www.ibm.com/common/v14/print.css" />
            </head>
            <body>
                <form action="store.jsp" method="post">
                    <input type="hidden" name="type" value="insert" />
                    <table>
                        <tr>
                            <td>
                                NewsID
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
                                <select name="publish">
                                    <option value="1">Active</option>
                                    <option value="0" selected>Archive</option>
                                </select>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                Start Date
                            </td>
                            <td>
                                <input type="text" size="10" name="start_date" />
                            </td>
                        </tr>
                        <tr>
                            <td>
                                End Date
                            </td>
                            <td>
                                <input type="text" size="10" name="end_date" />
                            </td>
                        </tr>
                        <tr>
                            <td>
                                Publish Date
                            </td>
                            <td>
                                <input type="text" size="10" name="publish_date" />
                            </td>
                        </tr>
                        <tr>
                            <td>
                                Headline
                            </td>
                            <td>
                                <input type="text" name="news_headline" />
                            </td>
                        </tr>
                        <tr>
                            <td>
                                Link
                            </td>
                            <td>
                                <input type="text" name="link" />
                            </td>
                        </tr>
                        <tr>
                            <td>
                                Summary
                            </td>
                            <td>
                                <textarea name="news_summary" cols="40" rows="5"></textarea>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                Story
                            </td>
                            <td>
                                <textarea name="news_story" cols="40" rows="20"></textarea>
                            </td>
                        </tr>
                    <table>
                    <input type="submit" value="Submit" />
                    <input type="submit" value="Cancel" />
                </form>
            </body>
        </html>
    </c:when>
    <c:when test="${param.type == 'edit'}">
        <sql:query var="rs" dataSource="jdbc/IBMDB">
        SELECT * FROM news WHERE newsid = ?
            <sql:param value="${param.newsid}" />
        </sql:query>
        <c:forEach var="row" items="${rs.rows}" >
        <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
            "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

        <html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
            <head>
                <title>Editing News Item ${row.newsid} - ${row.headline}</title>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <!-- ibm.com V14 OneX css -->
                <link rel="stylesheet" type="text/css" href="//www.ibm.com/common/v14/main.css" />
                <link rel="stylesheet" type="text/css" media="all" href="//www.ibm.com/common/v14/screen.css" />
                <link rel="stylesheet" type="text/css" media="screen,print" href="//www.ibm.com/common/v14/table.css" />
                <link rel="stylesheet" type="text/css" media="print" href="//www.ibm.com/common/v14/print.css" />
            </head>
            <body>
                <form action="store.jsp" method="post">
                    <input type="hidden" name="type" value="update" />
                    <table>
                        <tr>
                            <td>
                                NewsID
                            </td>
                            <td>
                                ${row.newsid}
                            </td>
                        </tr>
                        
                        <tr>
                            <td>
                                Status
                            </td>
                            <td>
                                <select name="publish">
                                    <option value="true" <c:if test="${row.publish == true}">selected</c:if>>Active</option>
                                    <option value="false" <c:if test="${row.publish == false}">selected</c:if>>Archive</option>
                                </select>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                Start Date
                            </td>
                            <td>
                                <input type="text" size="10" name="start_date" value="${row.start_date}" />
                            </td>
                        </tr>
                        <tr>
                            <td>
                                End Date
                            </td>
                            <td>
                                <input type="text" size="10" name="end_date" value="${row.end_date}" />
                            </td>
                        </tr>
                        <tr>
                            <td>
                                Publish Date
                            </td>
                            <td>
                                <input type="text" size="10" name="publish_date" value="${row.publish_date}" />
                            </td>
                        </tr>
                        <tr>
                            <td>
                                Headline
                            </td>
                            <td>
                                <input type="text" name="news_headline" value="${row.headline}" />
                            </td>
                        </tr>
                        <tr>
                            <td>
                                Link
                            </td>
                            <td>
                                <input type="text" name="link" value="${row.link}" />
                            </td>
                        </tr>
                        <tr>
                            <td>
                                Summary
                            </td>
                            <td>
                                <textarea name="news_summary" cols="40" rows="5">${row.summary}</textarea>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                Story
                            </td>
                            <td>
                                <textarea name="news_story" cols="40" rows="20">${row.story}</textarea>
                            </td>
                        </tr>
                    <table>
                    <input type="submit" value="Submit" />
                    <input type="submit" value="Cancel" />
                </form>
            </body>
        </html>
        </c:forEach>
    </c:when>

    <c:when test="${param.type == 'insert'}">
        <sql:update dataSource="jdbc/IBMDB">
           INSERT INTO news
           (newsid, publish_date, start_date, end_date, publish, link, headline, summary, story)
           VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?)
               
           <sql:param value="${null}" />
           <sql:dateParam value="${now}" />
           <sql:dateParam value="${parsedStartDate}" type="date" />
           <sql:dateParam value="${parsedEndDate}" type="date" />
           <sql:param value="${param.publish}" />
           <sql:param value="${param.link}" />
           <sql:param value="${param.news_headline}" />
           <sql:param value="${param.news_summary}" />
           <sql:param value="${param.news_story}" />
        </sql:update>
        <c:redirect url="news_admin.jsp" />
    </c:when>

    <c:when test="${param.type == 'update'}">
        <c:if test="${param.publish == true}">
            <c:set var="parsedPublishBool" value="true" />
        </c:if>
        <c:if test="${param.publish == false}">
            <c:set var="parsedPublishBool" value="false" />
        </c:if>
        <sql:update dataSource="jdbc/IBMDB">
           UPDATE news
           SET publish_date = ?,
           start_date = ?,
           end_date = ?,
           publish = ?,
           link = ?,
           headline = ?,
           summary = ?,
           story = ?
           WHERE newsid = ?
               
           <sql:param value="${param.newsid}" />
           <sql:dateParam value="${now}" />
           <sql:dateParam value="${parsedStartDate}" type="date" />
           <sql:dateParam value="${parsedEndDate}" type="date" />
           <sql:param value="${parsedPublishBool}" />
           <sql:param value="${param.link}" />
           <sql:param value="${param.news_headline}" />
           <sql:param value="${param.news_summary}" />
           <sql:param value="${param.news_story}" />
        </sql:update>
        <c:redirect url="news_admin.jsp" />
    </c:when>
</c:choose>

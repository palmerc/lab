<%@ page contentType="text/xml" %><?xml version="1.0" encoding="UTF-8" ?>
<%@ taglib uri="http://java.sun.com/jsp/jstl/sql" prefix="sql" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>

<rss version="2.0">
    <channel>
        <title>IBM Academic Initiative News</title>
        <link>http://www.ibm.com/university/</link>
        <description>Get your geek on.</description>
        <language>en-us</language>
        <%-- Should probably be set to change once a day? Sat, 07 Sep 2002 00:00:01 GMT --%>
        <pubDate></pubDate>
        <%-- Date that the content last changed --%>
        <lastBuildDate></lastBuildDate>
        <docs>http://blogs.law.harvard.edu/tech/rss</docs>
        <generator>IBM AI RSS Generator 1.0</generator>
        <managingEditor>editor@ibm.com</managingEditor>
        <webMaster>webmaster@ibm.com</webMaster>

<sql:query var="rs" dataSource="jdbc/IBMDB">
    SELECT newsid, publish_date, start_date, end_date, publish, link, headline, summary, story FROM news
        WHERE (publish IS TRUE) AND (start_date <= CURDATE()) AND (end_date >= CURDATE())
</sql:query>        
<c:forEach var="row" items="${rs.rows}">
        <item>
            <title>${row.headline}</title>
            <link>${row.link}</link>
            <description>${row.summary}</description>
            <pubDate>${row.publish_date}</pubDate>
            <guid>${row.link}</guid>
        </item>
</c:forEach>
    </channel>
</rss>
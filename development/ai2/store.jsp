<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/sql" prefix="sql" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/fmt" prefix="fmt" %>

<fmt:parseDate value="${param.start_date}" var="parsedStartDate" pattern="yyyy-MM-dd" />
<fmt:parseDate value="${param.end_date}" var="parsedEndDate" pattern="yyyy-MM-dd" />
<fmt:parseDate value="${param.publish_date}" var="parsedPublishDate" pattern="yyyy-MM-dd" />
<jsp:useBean id="now" class="java.util.Date" />
<sql:update dataSource="jdbc/TestDB">
   INSERT INTO news
   (newsid, publish_date, start_date, end_date, publish, link, headline, summary, story)
   VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?)
       
   <sql:param value="${null}" />
   <sql:dateParam value="${now}" />
   <sql:dateParam value="${parsedStartDate}" type="date" />
   <sql:dateParam value="${parsedEndDate}" type="date" />
   <sql:param value="${true}" />
   <sql:param value="${param.link}" />
   <sql:param value="${param.news_headline}" />
   <sql:param value="${param.news_summary}" />
   <sql:param value="${param.news_story}" />
</sql:update>
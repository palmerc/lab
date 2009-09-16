<%@ page language="java" import="com.cameronpalmer.farris.servlet.*"
	contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<jsp:include page="preamble.jsp" />
<body>
<jsp:include page="header.jsp" />

<form action="BlogServlet" method="post">
<div class="colmask blogstyle">
	<div class="colmid">
		<div class="colleft">
			<div class="col1">
			<input type="hidden" name="type" value="compose" />
			Title:<input type="text" name="postSubject" />
			<textarea rows="24" cols="80" name="postBody"></textarea>
			<input type="submit" name="publishPost" value="Publish Post" />
			<input type="button" name="deletePost" value="Save Now" />
			Post Options:
			Reader Comments
			<input type="radio" name="comments" value="Allow" checked="checked" />Allow
			<input type="radio" name="comments" value="Disallow" />Disallow
			
			Post Date and Time:
			<input type="text" />
			<input type="text" />
			<a href="posts.jsp">Return to list of posts</a>
			</div>
			
			<div class="col2">
			</div>
		</div>	
	</div>
</div>
</form>

<jsp:include page="footer.jsp" />
</body>
<jsp:include page="postscript.jsp" />

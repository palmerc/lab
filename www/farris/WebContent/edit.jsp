<%@ page language="java" import="com.cameronpalmer.farris.servlet.*"
	contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<jsp:include page="preamble.jsp" />
<body>
<jsp:include page="header.jsp" />

<form action="BlogServlet" method="post">
<input type="hidden" name="formType" value="postUpdate" />
<div class="colmask blogstyle">
	<div class="colmid">
		<div class="colleft">
			<div class="col1">
			<input type="hidden" name="postUuid" value="${post.uuid}" />
			Title:<input type="text" name="postSubject" value="${post.subject}" />
			<textarea rows="24" cols="80" name="postBody">${post.body}</textarea>
			<input type="submit" name="publishPost" value="Publish Post" />
			<input type="submit" name="savePost" value="Save Now" />
			Post Options:
			Reader Comments
			<input type="radio" name="comments" value="Allow" checked="checked" />Allow
			<input type="radio" name="comments" value="Disallow" />Disallow
			
			Post Date and Time:
			<input type="text" />
			<input type="text" />
			<a href="../posts">Return to list of posts</a>
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

<%@ page language="java" import="com.cameronpalmer.farris.servlet.*"
	contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<jsp:include page="preamble.jsp" />
<body>
<jsp:include page="header.jsp" />
<div class="colmask blogstyle">
	<div class="colmid">
		<div class="colleft">
			<div class="col1">
			<!-- Output the blog posts -->
			<c:forEach var="post" items="${posts}">
				<ul>
					<li class="publishedDate">${post.publishedDate}</li>
					<li class="blogLink"><a href="post/${post.uuid}">Link</a></li>
					<li class="author">${post.author}</li>
					<li class="place"><a href="${post.place}">${post.place}</a></li>
				</ul>
				<h2 class="blogEntrySubject">${post.subject}</h2>
				${post.body}
			</c:forEach>
			</div>
			<div class="col2">
				<h2>Previous Posts</h2>
				<ul>
					<li><a href=""></a></li>
				</ul>
			</div>
		</div>	
	</div>
</div>
<jsp:include page="footer.jsp" />
</body>
<jsp:include page="postscript.jsp" />
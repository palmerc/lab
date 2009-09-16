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
			<input type="hidden" name="type" value="posts" />
			<input type="button" name="newPost" value="New Post" />
			<input type="text" name="postSearchText" />
			<input type="button" name="postSearchButton" value="Search" />
			Posts Per Page
			<select>
				<option>5</option>
				<option>10</option>
				<option>25</option>
				<option>50</option>
				<option>100</option>
				<option>300</option>
			</select>
			<table>
			<c:forEach var="post" items="${posts}">
				<tr>
					<td><input type="checkbox" name="post1" /></td>
					<td><a href="edit/${post.uuid}">Edit</a></td>
					<td><a href="view/${post.uuid}">View</a></td>
					<td>${post.subject}</td>
					<td><a href="comments/${post.uuid}">Comments</a></td>
					<td>${post.date}</td>
					<td>${post.author}</td>
					<td><a href="delete/${post.uuid}">Delete</a></td>
				</tr>
			</c:forEach>
			</table>
			<input type="button" name="publishPost" value="Publish Selected" />
			<input type="button" name="deletePost" value="Delete Selected" />
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
<?xml version="1.0" encoding="UTF-8"?>
<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<title>CameronPalmer.com</title>
<link rel="stylesheet" type="text/css" href="css/main.css" />
<link rel="shortcut icon" href="images/favicon.ico"
	type="image/vnd.microsoft.icon" />
<link rel="icon" href="images/favicon.ico" type="image/vnd.microsoft.icon" />
</head>

<body>
<div id="header">
	<h1><a href="index.jsp">CameronPalmer.com</a></h1>
</div>

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
					<td><a href="${post.uuid}">Edit</a></td>
					<td><a href="${post.uuid}">View</a></td>
					<td>${post.subject}</td>
					<td><a href="${post.uuid}">Comments</a></td>
					<td>${post.date}</td>
					<td>${post.author}</td>
					<td><a href="${post.uuid}">Delete</a></td>
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

<div id="footer">
	<p>Copyright 2009</p>
</div>
</body>
</html>
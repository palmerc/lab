<?xml version="1.0" encoding="UTF-8"?>
<%@ page language="java" import="com.cameronpalmer.farris.servlet.*"
	contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
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
	<h1>CameronPalmer.com</h1>
</div>

<div class="colmask blogstyle">
	<div class="colmid">
		<div class="colleft">
			<div class="col1">
			<!-- Output the blog posts -->
			<c:forEach var="post" items="${posts}">
				<ul>
					<li class="publishedDate">${post.date}</li>
					<li class="blogLink"><a href="?">Link</a></li>
					<li class="author">${post.author}</li>
					<li class="place"><a href="${post.place}">${post.place}</a></li>
				</ul>
				<h2 class="blogEntrySubject">${post.subject}</h2>
				<p>${post.body}</p>		
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

<div id="footer">
	<p>Copyright 2009</p>
 </div>
</body>
</html>
<?xml version="1.0" encoding="UTF-8"?>
<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
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

<div id="footer">
	<p>Copyright 2009</p>
</div>
</body>
</html>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
   "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
    <head>
        <title>
            IBM Academic Initiative News Input
        </title>
    </head>
    <body>
        <h1>New News</h1>
        <form action="store.jsp" method="post">
            Status  
            <select name="publish">
                <option value="true">Published</option>
                <option value="false" selected>Unpublished</option>
            </select>
            Start Date
            <input type="text" name="start_date" /><br />
            End Date
            <input type="text" name="end_date" /><br />
            Publish Date
            <input type="text" name="publish_date" /><br />
            Headline
            <input type="text" name="news_headline" /><br />
            Link
            <input type="text" name="link" /><br />
            Summary
            <textarea name="news_summary" cols="40" rows="5"></textarea><br />
            Story
            <textarea name="news_story" cols="40" rows="20"></textarea>
            
            <input type="submit" value="Submit" />
        </form>
    </body>
</html>
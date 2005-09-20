<html>
<head>
<title><?cs var:title ?></title>
</head>
<body>
<p><?cs var:body ?></p>

<dl><?cs each:i = item?>
    <dt><?cs var:i.title ?></dt>
    <dd><?cs var:i.date ?></dd>
<?cs /each
?></dl>
</body>
</html>
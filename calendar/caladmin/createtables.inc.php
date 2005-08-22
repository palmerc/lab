<?
/* ©2004 Proverbs, LLC. All rights reserved. */ 

if(!defined("CREATEDB_DATABASE_ACCESS")) 
{
	define("CREATEDB_DATABASE_ACCESS", TRUE); 

	require('../calaccess.inc.php');

	class setupdb extends caldbaccess
	{
		// Constructor
		function setupdb()
		{
			$this->caldbaccess();
		}

		function CreateDatabase($userid, $password)
		{
			$a = $this->tbl_users;
			$b = $this->tbl_bydate;
			$c = $this->tbl_recurring;

			$query  = "CREATE TABLE $a (userid varchar(30) NOT NULL default '', ";
			$query .= "password varchar(30) default NULL, level tinyint(4) NOT NULL default '0', ";
			$query .= "UNIQUE KEY userid (userid)) TYPE=MyISAM;";
			$this->query($query);

			sleep(2);

			$query  = "CREATE TABLE $b (id int(11) NOT NULL auto_increment, ";
			$query .= "event_date int(11) NOT NULL default '0', duration float NOT NULL default '1', ";
			$query .= "short_description varchar(50) NOT NULL default '', long_description text NOT NULL, ";
			$query .= "userid varchar(30) NOT NULL default '', PRIMARY KEY id (id)) TYPE=MyISAM;";
			$this->query($query);

			sleep(2);

			$query  = "CREATE TABLE $c (id int(11) NOT NULL auto_increment, ";
			$query .= "weekday tinyint(4) NOT NULL default '0', event_time varchar(8) NOT NULL default '', ";
			$query .= "duration float NOT NULL default '1', period tinyint(4) NOT NULL default '0', ";
			$query .= "month tinyint(4) NOT NULL default '0', short_description varchar(50) NOT NULL default '', ";
			$query .= "long_description text NOT NULL, userid varchar(30) NOT NULL default '', ";
			$query .= "KEY id (id)) TYPE=MyISAM; ";
			$this->query($query);

			$this->AddNewUser($userid, 2, $password);

			$query = "SELECT * FROM $a WHERE userid='$userid'";

			$arr = $this->getrows($query);
			if (count($arr) > 0)
				return true;
			else
				return false;
		}
	}
}
?>
-- --------------------------------------------------------

-- 
-- Table structure for table `calendardate`
-- 

CREATE TABLE `calendardate` (
  `id` int(11) NOT NULL auto_increment,
  `event_date` int(11) NOT NULL default '0',
  `duration` float NOT NULL default '1',
  `short_description` varchar(50) NOT NULL default '',
  `long_description` text NOT NULL,
  `userid` varchar(30) NOT NULL default '',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

-- --------------------------------------------------------

-- 
-- Table structure for table `calendarday`
-- 

CREATE TABLE `calendarday` (
  `id` int(11) NOT NULL auto_increment,
  `weekday` tinyint(4) NOT NULL default '0',
  `event_time` varchar(8) NOT NULL default '',
  `duration` float NOT NULL default '1',
  `period` tinyint(4) NOT NULL default '0',
  `month` tinyint(4) NOT NULL default '0',
  `short_description` varchar(50) NOT NULL default '',
  `long_description` text NOT NULL,
  `userid` varchar(30) NOT NULL default '',
  KEY `id` (`id`)
) TYPE=MyISAM;

-- --------------------------------------------------------

-- 
-- Table structure for table `calendarusers`
-- 

CREATE TABLE `calendarusers` (
  `userid` varchar(30) NOT NULL default '',
  `password` varchar(30) default NULL,
  `level` tinyint(4) NOT NULL default '0',
  UNIQUE KEY `userid` (`userid`)
) TYPE=MyISAM;

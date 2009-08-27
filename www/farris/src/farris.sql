-- Database: farris

-- DROP DATABASE farris;

CREATE DATABASE farris
  WITH OWNER = postgres
       ENCODING = 'UTF8'
       LC_COLLATE = 'en_US'
       LC_CTYPE = 'en_US'
       CONNECTION LIMIT = -1;

CREATE TABLE blog
(
uuid INTEGER NOT NULL,
author VARCHAR,
place VARCHAR,
published_date DATE,
published_time TIME,
updated_date DATE,
updated_time TIME,
subject VARCHAR,
body TEXT
);

CREATE UNIQUE INDEX blog_uuid_idx ON blog(uuid);

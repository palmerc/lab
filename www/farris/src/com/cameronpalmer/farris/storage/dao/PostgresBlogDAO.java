package com.cameronpalmer.farris.storage.dao;

import java.sql.Connection;
import java.sql.Date;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Time;
import java.util.UUID;

import com.cameronpalmer.farris.blog.Blog;
import com.cameronpalmer.farris.storage.factory.PostgresDAOFactory;

/**
 * This is the concrete implementation of the CRUD for Blog objects
 * @author palmerc
 *
 */
public class PostgresBlogDAO implements BlogDAO {
	private Connection connection;
	String insertBlogSQL = "INSERT INTO " +
			"blog" +
			"(" +
			"uuid, " +
			"author, " +
			"place, " +
			"published_date, " +
			"published_time, " +
			"updated_date, " +
			"updated_time, " +
			"subject, " +
			"body" +
			")" +
			"VALUES (?,?,?,?,?,?,?,?,?)";
	String updateBlogSQL = "UPDATE blog " +
			"SET author=?, " +
			"place=?, " +
			"published_date=?, " +
			"published_time=?, " +
			"updated_date=?, " +
			"updated_time=?, " +
			"subject=?, " +
			"body=?" +
			"WHERE uuid=?";
	String selectBlogSQL = "SELECT * FROM blog WHERE uuid=?";
	String deleteBlogSQL = "DELETE FROM blog WHERE uuid=?";

	public PostgresBlogDAO() throws ClassNotFoundException, SQLException {
		connection = PostgresDAOFactory.createConnection();
	}
	
	@Override
	public void insert(Blog blog) throws SQLException {
		if ( blog == null ) {
			return;
		}
		
		Date publishedDate = new Date(blog.getPublishedDate().getTime());
		Date updatedDate = new Date(blog.getUpdatedDate().getTime());
		Time publishedTime = new Time(blog.getPublishedDate().getTime());
		Time updatedTime = new Time(blog.getUpdatedDate().getTime());
		
		PreparedStatement p = connection.prepareStatement(insertBlogSQL);
		p.setObject(1, blog.getUuid()); // UUID
		p.setString(2, blog.getAuthor()); // author
		p.setString(3, blog.getPlace()); // place
		p.setDate(4, publishedDate); // published_date
		p.setTime(5, publishedTime); // published_time
		p.setDate(6, updatedDate); // updated_date
		p.setTime(7, updatedTime); // updated_time
		p.setString(8, blog.getSubject()); // subject
		p.setString(9, blog.getBody()); // body
		
		p.execute();
		p.close();
	}

	@Override
	public void update() {
		// TODO Auto-generated method stub
		
	}
	
	@Override
	public Blog select(UUID uuid) throws SQLException {
		PreparedStatement p = connection.prepareStatement(selectBlogSQL);
		
		p.setObject(1, uuid);
		ResultSet rs = p.executeQuery();
		if ( rs.next() ) {
			String author = rs.getString("author");
			String place = rs.getString("place");
			Date publishedDate = rs.getDate("published_date");
			Time publishedTime = rs.getTime("published_time");
			Date updatedDate = rs.getDate("updated_date");
			Time updatedTime = rs.getTime("updated_time");
			String subject = rs.getString("subject");
			String body = rs.getString("body");
			p.close();
			
			Blog blog = new Blog();
			blog.setUuid(uuid);
			blog.setAuthor(author);
			blog.setPlace(place);
			blog.setPublishedDate(new Date(publishedDate.getTime() + publishedTime.getTime()));
			blog.setUpdatedDate(new Date(updatedDate.getTime() + updatedTime.getTime()));
			blog.setSubject(subject);
			blog.setBody(body);
			
			return blog;
		} else {
			return null;
		}	
	}
	
	@Override
	public void delete() {
		// TODO Auto-generated method stub
		
	}

}

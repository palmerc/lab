package com.cameronpalmer.farris.storage.dao;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.sql.Date;

import javax.sql.DataSource;

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
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
		SimpleDateFormat timeFormat = new SimpleDateFormat("HH:mm:ss");
		Date publishedDate = blog.getPublishedDate();
		Date updatedDate = blog.getUpdatedDate();
		String publishedDateString = dateFormat.format(publishedDate);
		String publishedTimeString = timeFormat.format(publishedDate);
		String updatedDateString = dateFormat.format(updatedDate);
		String updatedTimeString = timeFormat.format(updatedDate);
		
		PreparedStatement p = connection.prepareStatement(insertBlogSQL);
		p.setString(1, blog.getUuid().toString()); // UUID
		p.setString(2, blog.getAuthor()); // author
		p.setString(3, blog.getPlace()); // place
		p.setString(4, publishedDateString); // published_date
		p.setString(5, publishedTimeString); // published_time
		p.setString(6, updatedDateString); // updated_date
		p.setString(7, updatedTimeString); // updated_time
		p.setString(8, blog.getSubject()); // subject
		p.setString(9, blog.getBody()); // body
	}

	@Override
	public void update() {
		// TODO Auto-generated method stub
		
	}
	
	@Override
	public Blog select() {
		return null;
	}
	
	@Override
	public void delete() {
		// TODO Auto-generated method stub
		
	}

}

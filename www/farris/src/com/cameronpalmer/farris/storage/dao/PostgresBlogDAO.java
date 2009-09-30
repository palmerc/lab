package com.cameronpalmer.farris.storage.dao;

import java.sql.Connection;
import java.sql.Date;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Time;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

import com.cameronpalmer.farris.storage.factory.PostgresDAOFactory;
import com.cameronpalmer.farris.to.PostTO;

/**
 * This is the concrete implementation of the CRUD for Blog objects
 * 
 * @author palmerc
 * 
 */
public class PostgresBlogDAO implements BlogDAO {
	private Connection connection;
	String insertBlogSQL = "INSERT INTO " + "blog" + "(" + "uuid, "
			+ "author, " + "place, " + "published_date, " + "published_time, "
			+ "updated_date, " + "updated_time, " + "subject, " + "body" + ")"
			+ "VALUES (?,?,?,?,?,?,?,?,?)";
	String updateBlogSQL = "UPDATE blog " + "SET author=?, " + "place=?, "
			+ "published_date=?, " + "published_time=?, " + "updated_date=?, "
			+ "updated_time=?, " + "subject=?, " + "body=?" + "WHERE uuid=?";
	String selectBlogSQL = "SELECT * FROM blog WHERE uuid=?";
	String selectAllPostsSQL = "SELECT * FROM blog";
	String deleteBlogSQL = "DELETE FROM blog WHERE uuid=?";

	public PostgresBlogDAO() throws ClassNotFoundException, SQLException {
		connection = PostgresDAOFactory.createConnection();
	}

	@Override
	public boolean insert(PostTO blog) throws SQLException {
		boolean success = false;
		if (blog == null) {
			return false;
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

		success = p.execute();
		p.close();

		return success;
	}

	@Override
	public boolean update(PostTO blog) throws SQLException {
		boolean success = false;
		if (blog == null) {
			return false;
		}

		Date publishedDate = new Date(blog.getPublishedDate().getTime());
		Date updatedDate = new Date(blog.getUpdatedDate().getTime());
		Time publishedTime = new Time(blog.getPublishedDate().getTime());
		Time updatedTime = new Time(blog.getUpdatedDate().getTime());

		// author=1, place=2, published_date=3, published_time=4,
		// updated_date=5,
		// updated_time=6, subject=7, body=8, uuid=9
		PreparedStatement p = connection.prepareStatement(updateBlogSQL);

		p.setString(1, blog.getAuthor()); // author
		p.setString(2, blog.getPlace()); // place
		p.setDate(3, publishedDate); // published_date
		p.setTime(4, publishedTime); // published_time
		p.setDate(5, updatedDate); // updated_date
		p.setTime(6, updatedTime); // updated_time
		p.setString(7, blog.getSubject()); // subject
		p.setString(8, blog.getBody()); // body
		p.setObject(9, blog.getUuid()); // UUID

		success = p.execute();
		p.close();

		return success;
	}

	@Override
	public PostTO select(UUID uuid) throws SQLException {
		PreparedStatement p = connection.prepareStatement(selectBlogSQL);

		p.setObject(1, uuid);
		ResultSet rs = p.executeQuery();
		if (rs.next()) {
			String author = rs.getString("author");
			String place = rs.getString("place");
			Date publishedDate = rs.getDate("published_date");
			Time publishedTime = rs.getTime("published_time");
			Date updatedDate = rs.getDate("updated_date");
			Time updatedTime = rs.getTime("updated_time");
			String subject = rs.getString("subject");
			String body = rs.getString("body");
			p.close();

			PostTO blog = new PostTO();
			blog.setUuid(uuid);
			blog.setAuthor(author);
			blog.setPlace(place);
			blog.setPublishedDate(new Date(publishedDate.getTime()
					+ publishedTime.getTime()));
			blog.setUpdatedDate(new Date(updatedDate.getTime()
					+ updatedTime.getTime()));
			blog.setSubject(subject);
			blog.setBody(body);

			return blog;
		} else {
			return null;
		}
	}

	@Override
	public List<PostTO> getAllPosts() throws SQLException {
		PreparedStatement p = connection.prepareStatement(selectAllPostsSQL);
		ResultSet rs = p.executeQuery();
		
		List<PostTO> posts = new ArrayList<PostTO>();		
		while ( rs.next() ) {
			UUID uuid = (UUID) rs.getObject("uuid");
			String author = rs.getString("author");
			String place = rs.getString("place");
			Date publishedDate = rs.getDate("published_date");
			Time publishedTime = rs.getTime("published_time");
			Date updatedDate = rs.getDate("updated_date");
			Time updatedTime = rs.getTime("updated_time");
			String subject = rs.getString("subject");
			String body = rs.getString("body");

			PostTO post = new PostTO();
			post.setUuid(uuid);
			post.setAuthor(author);
			post.setPlace(place);
			post.setPublishedDate(new Date(publishedDate.getTime()
					+ publishedTime.getTime()));
			post.setUpdatedDate(new Date(updatedDate.getTime()
					+ updatedTime.getTime()));
			post.setSubject(subject);
			post.setBody(body);
			
			posts.add(post);
		}
		p.close();
		
		return posts;
	}

	@Override
	public boolean delete(UUID uuid) throws SQLException {
		boolean success = false;
		if (uuid == null) {
			return false;
		}

		PreparedStatement p = connection.prepareStatement(deleteBlogSQL);

		p.setObject(1, uuid);
		success = p.execute();
		p.close();

		return success;
	}
}

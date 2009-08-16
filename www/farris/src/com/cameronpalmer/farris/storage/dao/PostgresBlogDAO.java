package com.cameronpalmer.farris.storage.dao;

import com.cameronpalmer.farris.blog.Blog;

/**
 * This is the concrete implementation of the CRUD for Blog objects
 * @author palmerc
 *
 */
public class PostgresBlogDAO implements BlogDAO {
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
	String selectBlogSQL = "SELECT * FROM blog WHERE uuid=?"
	String deleteBlogSQL = "DELETE FROM blog WHERE uuid=?";

	@Override
	public void insert() {
		PreparedStatement p = con.
		// uuid, date, subject, body
		// TODO Auto-generated method stub
		
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

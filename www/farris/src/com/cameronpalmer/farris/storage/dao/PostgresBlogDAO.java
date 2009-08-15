package com.cameronpalmer.farris.storage.dao;

import com.cameronpalmer.farris.blog.Blog;

/**
 * This is the concrete implementation of the CRUD for Blog objects
 * @author palmerc
 *
 */
public class PostgresBlogDAO implements BlogDAO {
	String insertBlogSQL = "INSERT INTO blog(";
	String updateBlogSQL = "UPDATE ";
	String deleteBlogSQL = "DELETE FROM blog WHERE uuid=blah";

	@Override
	public void insert() {
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

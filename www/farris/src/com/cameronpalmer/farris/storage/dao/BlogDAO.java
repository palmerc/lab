package com.cameronpalmer.farris.storage.dao;

import java.sql.SQLException;

import com.cameronpalmer.farris.blog.Blog;

/**
 * Interface for Blog Data Access Objects.
 * @author palmerc
 *
 */
public interface BlogDAO {
	public void insert(Blog blog) throws SQLException;
	public void update();
	public Blog select();
	public void delete();
}

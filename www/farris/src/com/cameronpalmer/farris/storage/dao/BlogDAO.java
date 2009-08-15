package com.cameronpalmer.farris.storage.dao;

import com.cameronpalmer.farris.blog.Blog;

/**
 * Interface for Blog Data Access Objects.
 * @author palmerc
 *
 */
public interface BlogDAO {
	public void insert();
	public void update();
	public Blog select();
	public void delete();
}

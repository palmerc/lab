package com.cameronpalmer.farris.storage.dao;

import java.sql.SQLException;
import java.util.List;
import java.util.UUID;

import com.cameronpalmer.farris.to.PostTO;

/**
 * Interface for Blog Data Access Objects.
 * @author palmerc
 *
 */
public interface BlogDAO {
	/**
	 * Insert a blog object.
	 * @param blog object to be inserted.
	 * @return true for success, or false for failure. Passing a null blog will return false.
	 * @throws SQLException
	 */
	public boolean insert(PostTO blog) throws SQLException;

	/**
	 * Update a blog object.
	 * @param blog object to be updated.
	 * @return true for success, or false for failure. Passing a null blog will return false.
	 * @throws SQLException
	 */
	public boolean update(PostTO blog) throws SQLException;
	
	/**
	 * Retrieve a blog object.
	 * @param uuid the key of the record to be retrieved.
	 * @return the blog object or null if nonexistent.
	 * @throws SQLException
	 */
	public PostTO select(UUID uuid) throws SQLException;
	
	public List<PostTO> getAllPosts() throws SQLException;
	
	/**
	 * Delete a blog object.
	 * @param uuid the key of the record to be deleted.
	 * @return true for success, or false for failure. Passing a null uuid will return false.
	 * @throws SQLException
	 */
	public boolean delete(UUID uuid) throws SQLException;
}

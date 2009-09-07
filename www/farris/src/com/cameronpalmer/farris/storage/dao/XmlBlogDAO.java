package com.cameronpalmer.farris.storage.dao;

import java.sql.SQLException;
import java.util.List;
import java.util.UUID;

import com.cameronpalmer.farris.blog.Post;


public class XmlBlogDAO implements BlogDAO {

	@Override
	public boolean delete(UUID uuid) throws SQLException {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean insert(Post blog) throws SQLException {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public Post select(UUID uuid) throws SQLException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public boolean update(Post blog) throws SQLException {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public List<Post> getAllPosts() throws SQLException {
		// TODO Auto-generated method stub
		return null;
	}
}

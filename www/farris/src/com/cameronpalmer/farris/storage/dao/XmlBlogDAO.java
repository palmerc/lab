package com.cameronpalmer.farris.storage.dao;

import java.sql.SQLException;
import java.util.List;
import java.util.UUID;

import com.cameronpalmer.farris.to.PostTO;


public class XmlBlogDAO implements BlogDAO {

	@Override
	public boolean delete(UUID uuid) throws SQLException {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean insert(PostTO blog) throws SQLException {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public PostTO select(UUID uuid) throws SQLException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public boolean update(PostTO blog) throws SQLException {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public List<PostTO> getAllPosts() throws SQLException {
		// TODO Auto-generated method stub
		return null;
	}
}

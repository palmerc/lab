package com.cameronpalmer.farris.blog;

import java.sql.SQLException;
import java.util.Date;
import java.util.List;
import java.util.UUID;

import com.cameronpalmer.farris.storage.dao.BlogDAO;
import com.cameronpalmer.farris.storage.factory.DAOFactory;
import com.cameronpalmer.farris.storage.factory.DAOFactory.DAOFactoryType;

public class BlogServiceImpl implements BlogService {
	BlogDAO blogDAO = null;
	
	private void init() throws ClassNotFoundException, SQLException {
		if ( blogDAO == null ) {
			DAOFactory daoFactory = DAOFactory.getDAOFactory(DAOFactoryType.POSTGRES);
			blogDAO = daoFactory.getBlogDAO();
		}
	}
	
	@Override
	public Post getPost(UUID uuid) {
		// contact backend and get Blog Object
		return null;
	}
	
	@Override
	public void insertPost(Post blog) {
		if ( blog == null ) {
			return;
		}

		try {
			this.init();
			blogDAO.insert(blog);
		} catch (SQLException e) {
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
	}

	@Override
	public List<Post> getPosts() {
		List<Post> results = null;
		try {
			this.init();		
			results = blogDAO.getAllPosts();	
		} catch (SQLException e) {
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
		
		return results; 
	}

	@Override
	public List<Post> getPosts(Date from, Date to) {
		return null;
	}

	@Override
	public List<Post> getPosts(int number) {
		return null;
	}
}

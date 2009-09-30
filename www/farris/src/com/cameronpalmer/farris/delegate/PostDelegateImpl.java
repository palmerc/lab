package com.cameronpalmer.farris.delegate;

import java.sql.SQLException;
import java.util.Date;
import java.util.List;
import java.util.UUID;

import com.cameronpalmer.farris.storage.dao.BlogDAO;
import com.cameronpalmer.farris.storage.factory.DAOFactory;
import com.cameronpalmer.farris.storage.factory.DAOFactory.DAOFactoryType;
import com.cameronpalmer.farris.to.PostTO;

public class PostDelegateImpl implements PostDelegate {
	BlogDAO blogDAO = null;
	
	private void init() throws ClassNotFoundException, SQLException {
		if ( blogDAO == null ) {
			DAOFactory daoFactory = DAOFactory.getDAOFactory(DAOFactoryType.POSTGRES);
			blogDAO = daoFactory.getBlogDAO();
		}
	}
	
	@Override
	public PostTO getPost(UUID uuid) {
		PostTO post = null;
		try {
			this.init();
			post = blogDAO.select(uuid);
		} catch (SQLException e) {
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
		
		return post;
	}
	
	@Override
	public void insertPost(PostTO blog) {
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
	public void updatePost(PostTO blog) {
		if ( blog == null ) {
			return;
		}
		
		try {
			this.init();
			blogDAO.update(blog);
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}

	@Override
	public List<PostTO> getPosts() {
		List<PostTO> results = null;
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
	public List<PostTO> getPosts(Date from, Date to) {
		return null;
	}

	@Override
	public List<PostTO> getPosts(int number) {
		return null;
	}
}

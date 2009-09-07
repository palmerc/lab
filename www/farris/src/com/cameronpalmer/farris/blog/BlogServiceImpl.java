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
		DAOFactory daoFactory = DAOFactory.getDAOFactory(DAOFactoryType.POSTGRES);
		blogDAO = daoFactory.getBlogDAO();
	}
	
	@Override
	public Post getPost(UUID uuid) {
		// contact backend and get Blog Object
		return null;
	}

	@Override
	public void setPost(Post blogObject) {
		// TODO Auto-generated method stub

	}

	@Override
	public List<Post> getPosts() {
		List<Post> results = null;
		try {
			if ( blogDAO == null ) {
				this.init();
			}
			
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
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public List<Post> getPosts(int number) {
		// TODO Auto-generated method stub
		return null;
	}

	public static List<Post> getPostsStatically() {
		BlogService bs = new BlogServiceImpl();
		return bs.getPosts();
	}
	
}

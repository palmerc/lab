package com.cameronpalmer.farris.storage.factory;

import com.cameronpalmer.farris.storage.dao.BlogDAO;
import com.cameronpalmer.farris.storage.dao.XmlBlogDAO;

public class XmlDAOFactory extends DAOFactory {
	@Override
	public BlogDAO getBlogDAO() {
		return new XmlBlogDAO();
	}
}

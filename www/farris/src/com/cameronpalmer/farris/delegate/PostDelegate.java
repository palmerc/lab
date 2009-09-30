package com.cameronpalmer.farris.delegate;

import java.util.Date;
import java.util.List;
import java.util.UUID;

import com.cameronpalmer.farris.to.PostTO;

public interface PostDelegate {
	public List<PostTO> getPosts();
	public List<PostTO> getPosts(Date from, Date to);
	public List<PostTO> getPosts(int number);
	public PostTO getPost(UUID uuid);
	public void insertPost(PostTO post);
	public void updatePost(PostTO blog);
}

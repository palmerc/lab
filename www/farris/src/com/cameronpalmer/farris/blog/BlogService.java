package com.cameronpalmer.farris.blog;

import java.util.Date;
import java.util.List;
import java.util.UUID;

public interface BlogService {
	public List<Post> getPosts();
	public List<Post> getPosts(Date from, Date to);
	public List<Post> getPosts(int number);
	public Post getPost(UUID uuid);
	public void insertPost(Post post);
	public void updatePost(Post blog);
}

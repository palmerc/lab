package edu.unt.CSTalkie;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintStream;
import java.net.Socket;
import java.util.Vector;

class Worker extends ServerTalkie implements Runnable {
	Socket s;
	InputStream is;
	PrintStream ps;
	private int index = 0;
	private char current = 0;
	private char buffer[] = new char[1024];
	
	Worker() {
		s = null;
		is = null;
		ps = null;
	}
	
	synchronized void setSocket(Socket s) throws IOException {
		this.s = s;
		s.setTcpNoDelay(true);
		is = new BufferedInputStream(s.getInputStream());
		ps = new PrintStream(s.getOutputStream());
		notify();
	}
	
	public synchronized void run() {
		while (true) {
			if (s == null) {
				try {
					wait();
				} catch (InterruptedException e) {
					continue;
				}
			}
			
			try {
				handleClient();
			} catch (Exception e) {
				e.printStackTrace();
			}
			
		}
	}
	
	void handleClient() throws IOException {
		if (is.available() > 0) {
			current = (char) is.read();
			if ( current == Character.valueOf('\n') ) {
				buffer[index] = current;
				String msg = String.copyValueOf(buffer);
				for (int i = 0; i < buffer.length; ++i) {
					buffer[i] = 0;
				}
				index = 0;
				current = 0;
				broadcast(s.getInetAddress() + "> " + msg);
			} else {
				buffer[index] = current;
			}
			index++;
		}
	}
}
package edu.unt.CSTalkie;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Vector;

class ServerTalkie {
	static Vector threads = new Vector();
	
	public static void broadcast(String msg) {
		for (int i = 0; i < threads.size(); ++i) {
			Worker w = (Worker) threads.elementAt(i);
			w.ps.print(msg);
		}
	}
    
	public static void main(String[] a) throws Exception {
		int port = 8080;
		if (a.length > 0) {
			port = Integer.parseInt(a[0]);
		}
		
	    ServerSocket ss = null;
	    Socket s = null;
		
	    ss = new ServerSocket(port);
	    
	    System.out.println("Waiting on connection.");
	    while ( true ) {
	    	try {
	    		s = ss.accept();
	    		System.out.println("Incoming connection from: " + s.getInetAddress());
	    		Worker w = new Worker();
	    		w.setSocket(s);
	    		(new Thread(w)).start();
	    		threads.addElement(w);
	    	} catch (IOException e) {
	    		System.err.println(e);
	    	}
	    }
	}	
}
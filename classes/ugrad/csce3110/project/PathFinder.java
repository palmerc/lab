/*
 * PathFinder.java
 * Project for Data Structures TTh 5:30
 * Tyler Chamberlain, Cameron Palmer, Brian Stair, Kristina Ratliff 
 * 
 */

import javax.swing.*;
import java.awt.*;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class PathFinder {

   //Specify the look and feel to use.  Valid values:
   //null (use the default), "Metal", "System", "Motif", "GTK+"
   final static String LOOKANDFEEL = null;
   private static int nodeSize = 20;
   private static int gridSize = 20;
   private static int Xmax = gridSize * nodeSize;
   private static int Ymax = gridSize * nodeSize;
   private static boolean drawGridLines = true;
   private static int nodeCount = 0;  // Line number of count 
   private static int pathCount = 0;
   private static int[][] nodeList = new int[gridSize^2][3];
   private static int[][] pathList = new int[gridSize^4][3];
   //private static int[] shortList = new int[gridSize^2][3];
 private static int[][] shortList = {
   { 1, 3 },
   { 3, 6 },
   { 6, 5 },
   { 6, 7 }
};

   public PathFinder() {


	   
      String lookAndFeel = null;

      if (LOOKANDFEEL != null) {
         if (LOOKANDFEEL.equals("Metal")) {
            lookAndFeel = UIManager.getCrossPlatformLookAndFeelClassName();
         } else if (LOOKANDFEEL.equals("System")) {
            lookAndFeel = UIManager.getSystemLookAndFeelClassName();
         } else if (LOOKANDFEEL.equals("Motif")) {
            lookAndFeel = "com.sun.java.swing.plaf.motif.MotifLookAndFeel";
         } else if (LOOKANDFEEL.equals("GTK+")) { //new in 1.4.2
            lookAndFeel = "com.sun.java.swing.plaf.gtk.GTKLookAndFeel";
         } else {
            System.err.println("Unexpected value of LOOKANDFEEL specified: "
                  + LOOKANDFEEL);
            lookAndFeel = UIManager.getCrossPlatformLookAndFeelClassName();
         }

         try {
            UIManager.setLookAndFeel(lookAndFeel);
         } catch (ClassNotFoundException e) {
            System.err.println("Couldn't find class for specified look and feel:"
                  + lookAndFeel);
            System.err.println("Did you include the L&F library in the class path?");
            System.err.println("Using the default look and feel.");
         } catch (UnsupportedLookAndFeelException e) {
            System.err.println("Can't use the specified look and feel ("
                  + lookAndFeel
                  + ") on this platform.");
            System.err.println("Using the default look and feel.");
         } catch (Exception e) {
            System.err.println("Couldn't get specified look and feel ("
                  + lookAndFeel
                  + "), for some reason.");
            System.err.println("Using the default look and feel.");
            e.printStackTrace();
         }
      }

      //Make sure we have nice window decorations.
      JFrame.setDefaultLookAndFeelDecorated(true);

      JFrame f = new JFrame("PathFinder");
      Canvas canvas = new Canvas() {
         public void paint(Graphics g) {

   			//Draw grid lines
        	if (drawGridLines == true) {
	            g.setColor(Color.lightGray);
	            for (int i=0; i < Ymax+1; i+=nodeSize) {
	               g.drawLine(0, i, Xmax, i);
	            }
	            for (int i=0; i < Xmax+1; i+=nodeSize) {
	               g.drawLine(i, 0, i, Ymax);
	            }
        	}

			//Draw paths
            for (int i=0; i < pathList.length; i++) {
            	if (pathList[i][0] != -1) {
	            	g.setColor(Color.black);
	            	int n1 = pathList[i][0];
	            	int n2 = pathList[i][1];
	            	int x1 = nodeList[n1][0] * nodeSize + nodeSize/2;
	            	int y1 = nodeList[n1][1] * nodeSize + nodeSize/2;
	            	int x2 = nodeList[n2][0] * nodeSize + nodeSize/2;
	            	int y2 = nodeList[n2][1] * nodeSize + nodeSize/2;
	            	g.drawLine(x1, y1, x2, y2);
            	}
            }
            
            //Draw shortPath
        	g.setColor(Color.blue);
            for (int i=0; i < shortList.length; i++) {
            	if (shortList[i][0] != -1) {
	            	int n1 = shortList[i][0];
	            	int n2 = shortList[i][1];
	            	int x1 = nodeList[n1][0] * nodeSize + nodeSize/2 + 1;
	            	int y1 = nodeList[n1][1] * nodeSize + nodeSize/2 + 1;
	            	int x2 = nodeList[n2][0] * nodeSize + nodeSize/2 + 1;
	            	int y2 = nodeList[n2][1] * nodeSize + nodeSize/2 + 1;
	            	g.drawLine(x1, y1, x2, y2);

            	}
            }
                        
            //Draw nodes
            for (int i=0; i < nodeList.length; i++) {
            	if (nodeList[i][0] != -1) {
	            	if (nodeList[i][2]==1) {
	            		//Start Node (dark green)
	            		g.setColor(new Color(0, 200, 0));
	            	} else if (nodeList[i][2]==2) {
	            		//Destination Node
	            		g.setColor(Color.red);
	            	} else {
						//Regular Node
	            		g.setColor(Color.blue);
	            	}
	            	g.fillOval(nodeList[i][0]*nodeSize, nodeList[i][1]*nodeSize, nodeSize, nodeSize);
	            	g.setColor(Color.black);
	            	g.drawOval(nodeList[i][0]*nodeSize, nodeList[i][1]*nodeSize, nodeSize-1, nodeSize-1);
	            	
	            	//Draw node labels
	            	g.setColor(Color.white);
	            	if (i < 10) {
		            	g.drawString(""+i, nodeList[i][0]*nodeSize + 7, nodeList[i][1]*nodeSize + 14);
	            	} else {
		            	g.drawString(""+i, nodeList[i][0]*nodeSize + 3, nodeList[i][1]*nodeSize + 14);            		
	            	}
            	}
            	
            }
           
            //Draw weights
            for (int i=0; i < pathList.length; i++) {
            	if (pathList[i][0] != -1) {
	            	int n1 = pathList[i][0];
	            	int n2 = pathList[i][1];
	            	int x1 = nodeList[n1][0] * nodeSize + nodeSize/2;
	            	int y1 = nodeList[n1][1] * nodeSize + nodeSize/2;
	            	int x2 = nodeList[n2][0] * nodeSize + nodeSize/2;
	            	int y2 = nodeList[n2][1] * nodeSize + nodeSize/2;

	            	g.setColor(Color.white);
	            	g.fillOval( (x1+x2)/2-3, (y1+y2)/2-11, 13, 13 );
	            	
	            	g.setColor(Color.black);
	            	g.drawOval( (x1+x2)/2-3, (y1+y2)/2-11, 12, 12 );
	            	
	            	g.setColor(Color.red);
	            	g.drawString("" + pathList[i][2], (x1+x2)/2, (y1+y2)/2 );
	            	
	            	
            	}
            }
            
         }
      };

      canvas.setBackground(Color.white);
      f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

      f.getContentPane().add(canvas, BorderLayout.CENTER);
      //f.pack();
      f.setSize(Xmax+10,Ymax+34);

      //Display the window.
      f.setVisible(true);
   }
   
	private static void readFile(String filename)
		{
	  	try{
	  		String type;
	  		int arg1, arg2, arg3, arg4;
	  		
	        FileReader input = new FileReader(filename);
	        BufferedReader bufRead = new BufferedReader(input);
	        
	        String line;    // String that holds current file line
	        
	        // Read first line
	        line = bufRead.readLine();
	                
	        // Read through file one line at time.
	        while (line != null){
	        	//System.out.println(line);
	            Pattern p = Pattern.compile("^([A-Z]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+)$");
	            Matcher m = p.matcher(line);
	            if(m.find()) {
	            	type = m.group(1);
		            arg1 = Integer.valueOf( m.group(2) ).intValue();
		            arg2 = Integer.valueOf( m.group(3) ).intValue();
		            arg3 = Integer.valueOf( m.group(4) ).intValue();
		            if ( m.group(5) != null)
		            	arg4 = Integer.valueOf( m.group(5) ).intValue();
		            else
		            	arg4 = 0;
		        	
		            //System.out.println("type="+type+" arg1="+arg1+" 2="+arg2+" 3="+arg3+" 4="+arg4);
		            
		        	if ( type.equals("NODE") ) {
		        		//add node to list
		        		int[] newNode = new int[3];
		        		newNode[0] = arg2;
		        		newNode[1] = arg3;
		        		newNode[2] = arg4;
		        		nodeList[arg1] = newNode;
		                nodeCount++;
		        	} else if ( type.equals("PATH") ) {
		        		//add to path list
		        		int[] newPath = new int[3];
		        		newPath[0] = arg2;
		        		newPath[1] = arg3;
		        		newPath[2] = arg4;
		        		pathList[arg1] = newPath; 	
		        		pathCount ++;	        		
		        	} else {
		        		System.out.println("Invalid Line: " + line);
		        	}
		        	
	        	}
	        	line = bufRead.readLine();
	        }
	        
	        bufRead.close();
	        return;
	        
	    }catch (ArrayIndexOutOfBoundsException e){
	        /* If no file was passed on the command line, this expception is
	        generated. A message indicating how to the class should be
	        called is displayed */
	        System.out.println("Usage: java ReadFile filename\n");      
	
	        return;
	
	    }catch (IOException e){
	        // If another exception is generated, print a stack trace
	        e.printStackTrace();
	        return;
	    }	
			
	}; 
	
	//inits lists to have first field of each row as a -1, used later as check if empty
	public static void initLists() {
		
		for (int i = 0; i < nodeList.length; i++) {
			nodeList[i][0] = -1;	
		}
		
		for (int i = 0; i < pathList.length; i++) {
			pathList[i][0] = -1;
		}
		
	}

   public static void main(String[] argv) {
	   
	   initLists();
	   
	   readFile("input.txt");
	   
	   PathFinder x = new PathFinder();
     
   }
}
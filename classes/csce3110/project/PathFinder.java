/*
 * SwingApplication.java is a 1.4 example that requires
 * no other files.
 */
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.border.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.geom.*;

public class PathFinder {

   //Specify the look and feel to use.  Valid values:
   //null (use the default), "Metal", "System", "Motif", "GTK+"
   final static String LOOKANDFEEL = null;
   private int nodeSize = 20;
   private int gridSize = 20;
   private int Xmax = gridSize * nodeSize;
   private int Ymax = gridSize * nodeSize;
   private boolean drawGridLines = false;
   
   private int[][] nodeList = {
			  { 1, 2, 1 },
			  { 4, 9, 0 },
			  { 3, 3, 0 },
			  { 13, 9, 0 },
			  { 18, 2, 2 },
			  { 19, 19, 0 },
			  { 11, 2, 2},
			  { 17, 18, 0 },
			  { 8, 16, 0 },
			  { 2, 10, 2 },
			  { 3, 18, 0 },
			  { 7, 12, 0 }
			  
	  };
   
   private int[][] pathList = {
		   { 0, 2, 6 },
		   { 0, 1, 5 },
		   { 2, 3, 4 },
		   { 3, 4, 2 },
		   { 4, 5, 3 },
		   { 4, 6, 3 },
		   { 5, 7, 2 },
		   { 7, 8, 4 },
		   { 8, 9, 10 },
		   { 8, 10, 5 },
		   { 8, 11, 3 }
		  
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
            	g.setColor(Color.black);
            	int n1 = pathList[i][0];
            	int n2 = pathList[i][1];
            	int x1 = nodeList[n1][0] * nodeSize + nodeSize/2;
            	int y1 = nodeList[n1][1] * nodeSize + nodeSize/2;
            	int x2 = nodeList[n2][0] * nodeSize + nodeSize/2;
            	int y2 = nodeList[n2][1] * nodeSize + nodeSize/2;
            	g.drawLine(x1, y1, x2, y2);

            	//Draw weights
            	g.setColor(Color.red);
            	g.drawString("" + pathList[i][2], (x1+x2)/2, (y1+y2)/2 );
            }
            
            //Draw nodes
            for (int i=0; i < nodeList.length; i++) {
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
      };

      canvas.setBackground(Color.white);
      f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

      f.getContentPane().add(canvas, BorderLayout.CENTER);
      //f.pack();
      f.setSize(Xmax+10,Ymax+34);

      //Display the window.
      f.setVisible(true);
   }

   public static void main(String[] argv) {
     PathFinder x = new PathFinder();
   }
}
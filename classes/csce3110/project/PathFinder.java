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
            Color c = Color.gray;
            g.setColor(c);
            int Xmax = 300;
            int Ymax = 300;
            for (int i=0; i < Ymax; i+=10) {
               g.drawLine(0, i, Xmax, i);
            }
            for (int i=0; i < Xmax; i+=10) {
               g.drawLine(i, 0, i, Ymax);
            }
         }
      };
      
      canvas.setBackground(Color.white);   
      f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

      f.setSize(300,300); 
      f.add(canvas, BorderLayout.CENTER);
      f.add(canvas);

      //Display the window.
      f.setVisible(true);      
   }

   public static void main(String[] argv) {
     PathFinder x = new PathFinder();
   }
}
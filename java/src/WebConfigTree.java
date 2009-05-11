

import java.awt.Dimension;
import java.awt.GridLayout;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.util.Iterator;

import javax.swing.JEditorPane;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTree;
import javax.swing.UIManager;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreeSelectionModel;

import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import com.sun.org.apache.xerces.internal.util.SAXInputSource;

public class WebConfigTree extends JPanel implements TreeSelectionListener {
	private class NodeObject {
		public String label;
		public Node node;
		
		public NodeObject(String label, Node node) {
			this.label = label;
			this.node = node;
		}
		
		public String toString() {
			return label;
		}
	}
	
	private JEditorPane xmlPane;
	private JTree tree;

	// Optionally play with line styles. Possible values are
	// "Angled" (the default), "Horizontal", and "None".
	private static boolean playWithLineStyle = false;
	private static String lineStyle = "Horizontal";
	private static String fileName = null;
	private static Document document = null;

	// Optionally set the look and feel.
	private static boolean useSystemLookAndFeel = false;
	
	public WebConfigTree() {
		super(new GridLayout(1, 0));

		DefaultMutableTreeNode dmtn = createNodes(fileName);
		
		// Create a tree that allows one selection at a time.
		tree = new JTree(dmtn);
		tree.getSelectionModel().setSelectionMode(
				TreeSelectionModel.SINGLE_TREE_SELECTION);

		// Listen for when the selection changes.
		tree.addTreeSelectionListener(this);

		if (playWithLineStyle) {
			System.out.println("line style = " + lineStyle);
			tree.putClientProperty("JTree.lineStyle", lineStyle);
		}

		// Create the scroll pane and add the tree to it.
		JScrollPane treeView = new JScrollPane(tree);

		// Create the HTML viewing pane.
		xmlPane = new JEditorPane();
		xmlPane.setEditable(false);
		JScrollPane xmlView = new JScrollPane(xmlPane);

		// Add the scroll panes to a split pane.
		JSplitPane splitPane = new JSplitPane(JSplitPane.VERTICAL_SPLIT);
		splitPane.setTopComponent(treeView);
		splitPane.setBottomComponent(xmlView);

		Dimension minimumSize = new Dimension(400, 50);
		xmlView.setMinimumSize(minimumSize);
		treeView.setMinimumSize(minimumSize);
		splitPane.setDividerLocation(400);
		splitPane.setPreferredSize(new Dimension(500, 300));

		// Add the split pane to this panel.
		add(splitPane);
	}

	private DefaultMutableTreeNode createNodes(String fileName) {		
		SAXInputSource sis = new SAXInputSource();
		File fh = new File(fileName);
		InputStream is;
		try {
			is = new FileInputStream(fh);
			sis.setByteStream(is);
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		XmlReader xr = new XmlReader(sis.getInputSource());
		Document document = xr.getDocument();
		
		Node root = document.getDocumentElement();
		DefaultMutableTreeNode t = new DefaultMutableTreeNode(new NodeObject(root.getNodeName(), root));
		grow(t, root);
		
		return t;
	}

	private void grow(DefaultMutableTreeNode t, Node node) {
		if ( node == null ) {
			return;
		} else {
			NodeList children = node.getChildNodes();
			for ( int i = 0; i < children.getLength(); i++ ) {
				Node child = children.item(i);
				if ( child.getNodeType() == Node.TEXT_NODE && child.getNodeValue().trim().isEmpty() )
					continue;
				
				String name = null;
				NamedNodeMap attrs = child.getAttributes();
				if ( attrs != null ) {
					for ( int j = 0; j < attrs.getLength(); j++ ) {
						if ( "Name".equalsIgnoreCase(attrs.item(j).getNodeName()) ) {
							name = attrs.item(j).getNodeValue();
							break;
						}
					}
				}
				
				String label = child.getNodeName();
				if ( name != null ) {
					label = label.concat(": " + name);
				}
				
				NodeObject no = new NodeObject(label, child);
				DefaultMutableTreeNode x = new DefaultMutableTreeNode(no);
				
				t.add(x);
				grow(x, child);
			}
			return;
		}
	}

	/**
	 * Create the GUI and show it. For thread safety, this method should be
	 * invoked from the event dispatch thread.
	 */
	private static void createAndShowGUI() {
		if (useSystemLookAndFeel) {
			try {
				UIManager.setLookAndFeel(UIManager
						.getSystemLookAndFeelClassName());
			} catch (Exception e) {
				System.err.println("Couldn't use system look and feel.");
			}
		}

		// Create and set up the window.
		JFrame frame = new JFrame("WebConfig Tree Viewer");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		// Add content to the window.
		frame.add(new WebConfigTree());

		// Display the window.
		frame.pack();
		frame.setVisible(true);
	}
	
	public void valueChanged(TreeSelectionEvent e) {
		DefaultMutableTreeNode node = (DefaultMutableTreeNode) tree.getLastSelectedPathComponent();

		if (node == null)
			// Nothing is selected.	
			return;

		NodeObject nodeObj = (NodeObject) node.getUserObject();
		if ( nodeObj != null )
			displayObject(nodeObj.node);
	}
	
	public void displayObject(Node node) {
		StringBuilder sb = new StringBuilder();
		
		sb.append("Name: " + node.getNodeName() + "\n");
		
		sb.append("Attributes: " + "\n");
		NamedNodeMap nnm = node.getAttributes();
		if ( nnm != null ) {
			for ( int i = 0; i < nnm.getLength(); i++ ) {
				Node attr = nnm.item(i);
				sb.append("\t" + attr.getNodeName() + "=\"" + attr.getNodeValue() + "\"\n");
			}
		}
		
		sb.append("Value: " + node.getNodeValue() + "\n");
		
		xmlPane.setText(sb.toString());
	}

	public static void main(String[] args) {
		fileName = args[0];
		// Schedule a job for the event dispatch thread:
		// creating and showing this application's GUI.
		javax.swing.SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				createAndShowGUI();
			}
		});
	}
}

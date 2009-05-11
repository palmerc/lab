

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;

import com.sun.org.apache.xerces.internal.parsers.DOMParser;
import com.sun.org.apache.xerces.internal.util.SAXInputSource;

public class XmlReader {   
	private Document doc;
	
	public XmlReader(InputSource inputSource) {
		DOMParser p = new DOMParser();
		try {
			p.parse(inputSource);
		} catch (SAXException e) {
			System.err.println(e.getMessage());
		} catch (IOException e) {
			e.printStackTrace();
		}
		doc = p.getDocument();	
	}
	
	public Document getDocument() {
		return this.doc;
	}
	
	public void print(Node node) {
		int type = node.getNodeType();
		switch(type) {
			case Node.ATTRIBUTE_NODE:
				System.out.println("ATTRIBUTE_NODE");
				break;
			case Node.CDATA_SECTION_NODE:
				System.out.println("CDATA_SECTION_NODE");
				break;
			case Node.COMMENT_NODE:
				System.out.println("COMMENT_NODE - " + node.getNodeName() + node.getNodeValue());
				break;
			case Node.DOCUMENT_FRAGMENT_NODE:
				System.out.println("DOCUMENT_FRAGMENT_NODE");
				break;
			case Node.DOCUMENT_NODE:
				System.out.println("DOCUMENT_NODE");
				break;
			case Node.DOCUMENT_TYPE_NODE:
				System.out.println("DOCUMENT_TYPE_NODE");
				break;
			case Node.ELEMENT_NODE:
				System.out.println("ELEMENT_NODE - " + node.getNodeName());
				break;
			case Node.ENTITY_NODE:
				System.out.println("ENTITY_NODE");
				break;
			case Node.ENTITY_REFERENCE_NODE:
				System.out.println("ENTITY_REFERENCE_NODE");
				break;
			case Node.NOTATION_NODE:
				System.out.println("NOTATION_NODE");
				break;
			case Node.PROCESSING_INSTRUCTION_NODE:
				System.out.println("PROCESSING_INSTRUCTION_NODE");
				break;
			case Node.TEXT_NODE:
				System.out.println("TEXT_NODE - " + node.getNodeName());
				break;
			default:
				System.out.println(type);
		}
		NodeList children = node.getChildNodes();
		for ( int i = 0; i < children.getLength(); i++ ) {
			print(children.item(i));
		}
	}

	public String toString() {
		Node n = doc.getFirstChild();
		print(n);
		return null;
	}
	
	public static void main(String[] args) {
		String fileName = args[0];
			
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
		
		System.out.println(xr);
	}
}

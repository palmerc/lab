import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.Iterator;
import java.util.List;
import java.util.Properties;

/**
 * The simplest way I have come up with to deal with Java properties files
 * without having to use something like Apache Commons.
 * @author palmerc
 *
 */
public class JavaProperties {
	public static final String DEFAULT_ENCODING = "ISO-8859-1";
	
	/**
	 * This is to help out with Multi-line entries
	 * @param line
	 * @return true if the line has an even number of backslashes
	 */
	public static Boolean isNumberOfEndOfLineBackslashesEven(String line) {
		int backslashCount = 0;
		for ( int i = line.length() - 1; i >= 0 && line.charAt(i) == '\\'; i-- ) {
			backslashCount++;
		}
		return backslashCount % 2 == 0;
	}
	
	public static List<String> parseProperties(File file) {
		List<String> outputArray = new ArrayList<String>();
		Properties properties = new Properties();
		
		try {
			String fileLine = null;
			// Read in the file, loading each line into the array. Trim white space at beginning and end. 
			BufferedReader reader = new BufferedReader(new FileReader(file));
			List<String> lines = new ArrayList<String>();
			while ( (fileLine = reader.readLine()) != null ) {
				String trimmedLine = fileLine.trim();
				lines.add(trimmedLine);
			}
			reader.close();

			// Read the file in using the properties load method.
			reader = new BufferedReader(new FileReader(file));
			properties.load(reader);
			reader.close();
			
			
			// Go through the array of lines
			Iterator<String> iterator = lines.iterator();
			while ( iterator.hasNext() ) {
				String line = iterator.next();
				String outLine = null;
				
				// Read the line from the array and let properties find the key.
				Properties tempProp = new Properties();
				InputStream is = new ByteArrayInputStream(line.getBytes()) ;
				tempProp.load(is);
				Enumeration<Object> keys = tempProp.keys();
				// There should only ever be one item in tempProp
				while ( keys.hasMoreElements() ) {
					Object key = keys.nextElement();
					
					// Match the key we just found to a complete entry in our
					// complete key value hash table.
					if ( properties.containsKey(key) ) {
						String value = (String) properties.get(key);
						outLine = (key + " : " + value);
					}
					
					// Roll passed multi-line items
					while ( ! isNumberOfEndOfLineBackslashesEven(line) ) {
						line = iterator.next();
					}
				}
				
				// This should take care of whitespace and comments
				if ( outLine == null ) {
					outLine = line;
				}
				
				// Print out the line
				outputArray.add(outLine);
			}
		} catch (FileNotFoundException e) {
			System.err.println(e.getMessage());
		} catch (IOException e) {
			System.err.println(e.getMessage());
		}
		
		return outputArray;
	}
	
	
	
	/**
	 * Read in the file using properties load, and read the file into an array.
	 * We use the array to maintain order and get comments and empty lines.
	 * We use the Properties hash table to provide the key value pairs.
	 * @param args
	 */
	public static void main(String[] args) {
		File file = new File("/home/palmerc/Development/java-experiments/src/test.properties");
		
		List<String> lines = JavaProperties.parseProperties(file);
		for ( String line : lines ) {
			System.out.println(line);
		}
	}
}
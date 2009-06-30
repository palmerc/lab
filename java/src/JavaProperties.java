import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Properties;
import java.util.Map.Entry;

public class JavaProperties {
	public static void main(String[] args) {
		Properties properties = new Properties();
		File file = new File("/home/palmerc/Development/java-experiments/src/test.properties");
		FileInputStream fis;
		try {
			fis = new FileInputStream(file);
			properties.load(fis);
			PrintWriter out = new PrintWriter(System.out, true);
			for ( Entry<Object, Object> entry : properties.entrySet() ) {
				String key = entry.getKey().toString();
				String value = entry.getValue().toString();
				System.out.println(key + " : " + value);
			}
		} catch (FileNotFoundException e) {
			System.err.println(e.getMessage());
		} catch (IOException e) {
			System.err.println(e.getMessage());
		}
		
	}
}
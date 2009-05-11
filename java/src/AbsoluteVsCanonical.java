import java.io.File;
import java.io.IOException;

public class AbsoluteVsCanonical {

	public static void main(String[] args) {
		File f = new File(args[0]);
		System.out.println(f.toString());
		try {
			System.out.println("Canonical : " + f.getCanonicalPath());
			System.out.println("Absolute : " + f.getAbsolutePath());
			String test = f.getAbsolutePath();
			test = (new File(test)).getCanonicalPath();
			System.out.println("Canonical too: " + test);
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
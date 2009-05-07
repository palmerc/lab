import java.util.UUID;

// http://en.wikipedia.org/wiki/UUID

public class Uuid {
	public static void main(String[] args) {
		UUID test = UUID.randomUUID(); // Type 4
		System.out.println(test);
	}
}
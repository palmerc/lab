import java.util.*;

public class BinaryCode {
	public static String encode(String P) {
		String Q = new String();
		int first, middle, last;
		
		Q = "";
		for(int i = 0; i < P.length(); ++i) {
			if(i > 0)
				first = Integer.parseInt(String.valueOf(P.charAt(i-1)));
			else
				first = 0;
			middle = Integer.parseInt(String.valueOf(P.charAt(i)));
			if(i < P.length() - 1)
				last = Integer.parseInt(String.valueOf(P.charAt(i+1)));
			else
				last = 0;
				
			int temp = first + middle + last;
			Q += temp;
		}
		return Q;
	}

	public static String[] decode(String Q) {
		String[] P = new String[2];
		int[] Pdigits = new int[51];
		
		P[0] = "";
		Pdigits[0] = 0;
		if(Q.length() <= 50) {
			if((Q.length() == 1) && (Integer.parseInt(String.valueOf(Q.charAt(0))) != 0)) {
				P[0] = "NONE";
			} else {
				for(int i = 0; i < Q.length(); ++i) {
					if(i < 1)
						Pdigits[i+1] = Integer.parseInt(String.valueOf(Q.charAt(i))) - Pdigits[i];
					else 
						Pdigits[i+1] = Integer.parseInt(String.valueOf(Q.charAt(i))) - Pdigits[i-1] - Pdigits[i];
			
					if((Pdigits[i+1] != 0) && (Pdigits[i+1] != 1)) {
						P[0] = "NONE";
						break;
					}
				}
			}
		} else {
			P[0] = "NONE";
		}
		
		if(P[0].length() == 0) {
			for(int i=0; i < Q.length(); ++i) {
				P[0] += Pdigits[i];
			}
		}
		
		P[1] = "";
		Pdigits[0] = 1;
		if(Q.length() <= 50) {
			if((Q.length() == 1) && (Integer.parseInt(String.valueOf(Q.charAt(0))) != 1)) {
				P[1] = "NONE";
			} else {
				for(int i = 0; i < Q.length(); ++i) {
					if(i < 1)
						Pdigits[i+1] = Integer.parseInt(String.valueOf(Q.charAt(i))) - Pdigits[i];
					else 
						Pdigits[i+1] = Integer.parseInt(String.valueOf(Q.charAt(i))) - Pdigits[i-1] - Pdigits[i];
				
					if((Pdigits[i+1] != 0) && (Pdigits[i+1] != 1)) {
						P[1] = "NONE";
						break;
					}
				}
			}
		} else {
			P[1] = "NONE";
		}
		
		if(P[1].length() == 0) {
			for(int i=0; i < Q.length(); ++i) {
				P[1] += Pdigits[i];
			}
		}
		
		if(!P[0].equals("NONE"))
			if(!Q.equals(encode(P[0])))
				P[0] = "NONE";
				
		if(!P[1].equals("NONE"))
			if(!Q.equals(encode(P[1])))
				P[1] = "NONE";
		
		return P;
	}
    public static void main (String args[]) {
        String[] messages = new String[2];
		
		// The provided tests.
		messages = decode("123210122");
		System.out.println(messages[0] +" "+ messages[1]);
		if(messages[0].equals("011100011") && messages[1].equals("NONE")) 
			System.out.println("Success\n");
		else
			System.out.println("Failure\n");
			
		messages = decode("11");
		System.out.println(messages[0] +" "+ messages[1]);
		if(messages[0].equals("01") && messages[1].equals("10")) 
			System.out.println("Success\n");
		else
			System.out.println("Failure\n");
				
		messages = decode("22111");
		System.out.println(messages[0] +" "+ messages[1]);
		if(messages[0].equals("NONE") && messages[1].equals("11001")) 
			System.out.println("Success\n");
		else
			System.out.println("Failure\n");
			
        messages = decode("123210120");
		System.out.println(messages[0] +" "+ messages[1]);
		if(messages[0].equals("NONE") && messages[1].equals("NONE")) 
			System.out.println("Success\n");
		else
			System.out.println("Failure\n");
			
		messages = decode("3");
		System.out.println(messages[0] +" "+ messages[1]);
		if(messages[0].equals("NONE") && messages[1].equals("NONE")) 
			System.out.println("Success\n");
		else
			System.out.println("Failure\n");
			
		messages = decode("12221112222221112221111111112221111");
		System.out.println(messages[0] +" "+ messages[1]);
		if(messages[0].equals("01101001101101001101001001001101001") && messages[1].equals("10110010110110010110010010010110010"))
			System.out.println("Success\n");
		else
			System.out.println("Failure\n");
		
		messages = decode("1");
		System.out.println(messages[0] +" "+ messages[1]);
		if(messages[0].equals("NONE") && messages[1].equals("1"))
			System.out.println("Success\n");
		else
			System.out.println("Failure\n");
			
		messages = decode("2");
		System.out.println(messages[0] +" "+ messages[1]);
		if(messages[0].equals("NONE") && messages[1].equals("NONE"))
			System.out.println("Success\n");
		else
			System.out.println("Failure\n");
		
		messages = decode("111");
		System.out.println(messages[0] +" "+ messages[1]);
		if(messages[0].equals("010") && messages[1].equals("NONE"))
			System.out.println("Success\n");
		else
			System.out.println("Failure\n");

		messages = decode("122");
		System.out.println(messages[0] +" "+ messages[1]);
		if(messages[0].equals("011") && messages[1].equals("NONE"))
			System.out.println("Success\n");
		else
			System.out.println("Failure\n");

		messages = decode("12221112221112222222222221112222221111112221112221");
		System.out.println(messages[0] +" "+ messages[1]);
		if(messages[0].equals("01101001101001101101101101001101101001001101001101") && messages[1].equals("10110010110010110110110110010110110010010110010110"))
			System.out.println("Success\n");
		else
			System.out.println("Failure\n");

		messages = decode("11112222222221112222221111112221111112221112221111");
		System.out.println(messages[0] +" "+ messages[1]);
		if(messages[0].equals("01001101101101001101101001001101001001101001101001") && messages[1].equals("10010110110110010110110010010110010010110010110010"))
			System.out.println("Success\n");
		else
			System.out.println("Failure\n");
	}
}

/*
Problem Statement:

	Let's say you have a binary string such as the following:
		011100011
	One way to encrypt this string is to add to each digit the sum of its adjacent digits. For example, the above 
	string would become:
		123210122
	In particular, if P is the original string, and Q is the encrypted string, then Q[i] = P[i-1] + P[i] + P[i+1]
	for all digit positions i. Characters off the left and right edges of the string are treated as zeroes.
	
	An encrypted string given to you in this format can be decoded as follows (using 123210122 as an example):
		Assume P[0] = 0.
		Because Q[0] = P[0] + P[1] = 0 + P[1] = 1, we know that P[1] = 1.
		Because Q[1] = P[0] + P[1] + P[2] = 0 + 1 + P[2] = 2, we know that P[2] = 1.
		Because Q[2] = P[1] + P[2] + P[3] = 1 + 1 + P[3] = 3, we know that P[3] = 1.
		Repeating these steps gives us P[4] = 0, P[5] = 0, P[6] = 0, P[7] = 1, and P[8] = 1.
		We check our work by noting that Q[8] = P[7] + P[8] = 1 + 1 = 2. Since this equation works out, we are finished, 
		and we have recovered one possible original string.

	Now we repeat the process, assuming the opposite about P[0]:
		Assume P[0] = 1.
		Because Q[0] = P[0] + P[1] = 1 + P[1] = 0, we know that P[1] = 0.
		Because Q[1] = P[0] + P[1] + P[2] = 1 + 0 + P[2] = 2, we know that P[2] = 1.
		Now note that Q[2] = P[1] + P[2] + P[3] = 0 + 1 + P[3] = 3, which leads us to the conclusion that P[3] = 2. 
		However, this violates the fact that each character in the original string must be '0' or '1'. Therefore,
		there exists no such original string P where the first digit is '1'.

		Note that this algorithm produces at most two decodings for any given encrypted string. There can never be more 
		than one possible way to decode a string once the first binary digit is set.
	
		Given a String message, containing the encrypted string, return a String[] with exactly two elements. The first
		element should contain the decrypted string assuming the first character is '0'; the second element should assume
		the first character is '1'. If one of the tests fails, return the string "NONE" in its place. For the above example, 
		you should return {"011100011", "NONE"}.

Definition:
	Class: BinaryCode
	Method: decode
	Parameters: String
	Returns: String[]
	Method signature: String[] decode(String message) (be sure your method is public)

	Constraints - 
		message will contain between 1 and 50 characters, inclusive.
		Each character in message will be either '0', '1', '2', or '3'.

Examples:
0)	"123210122"
	Returns: { "011100011",  "NONE" }
	The example from above.

1)	"11"
	Returns: { "01",  "10" }
	We know that one of the digits must be '1', and the other must be '0'. We return both cases.

2)	"22111"
	Returns: { "NONE",  "11001" }
	Since the first digit of the encrypted string is '2', the first two digits of the original string must be '1'. 
	Our test fails when we try to assume that P[0] = 0.

3)	"123210120"	
	Returns: { "NONE",  "NONE" }
	This is the same as the first example, but the rightmost digit has been changed to something inconsistent with the
	rest of the original string. No solutions are possible.

4)	"3"
	Returns: { "NONE",  "NONE" }

5)	"12221112222221112221111111112221111"
	Returns: { "01101001101101001101001001001101001", "10110010110110010110010010010110010" }

This problem statement is the exclusive and proprietary property of TopCoder, Inc. Any unauthorized use or reproduction of 
this information without the prior written consent of TopCoder, Inc. is strictly prohibited. (c)2003, TopCoder, Inc. All 
rights reserved.

*/

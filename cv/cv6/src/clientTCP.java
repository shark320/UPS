import java.io.*;
import java.net.*;
import java.nio.CharBuffer;

class clientTCP
{
	private static final String HOST = "147.228.67.113";
	private static final Integer PORT = 4242;

	private static final String MAGIC = "KIVUPS";

	public static String constructPayload(String message){
		int length = message.length();
		return String.format("%02d%s", message.length(), message);
	}

	public static String constructNickname(String nick){
		String msg = constructPayload(nick);
		return String.format("%s%s%04d%s",MAGIC, "nick",msg.length(),msg);
	}

	public static String constructChat(String chat){
		String msg = String.format("%03d%s", chat.length(), chat);
		return String.format("%s%s%04d%s",MAGIC, "chat",msg.length(),msg);
	}


	public static void main(String argv[]) throws Exception {
		Socket socket = new Socket(HOST, PORT);
		InetAddress adresa = socket.getInetAddress();
		System.out.print("Pripojuju se na : "+adresa.getHostAddress()+" se jmenem : "+adresa.getHostName()+"\n" );

		OutputStream oos = socket.getOutputStream();
		String nickMsg = constructNickname("test");
		System.out.println("Sending nickname: " + nickMsg);
		oos.write(nickMsg.getBytes());
		oos.flush();
		String chatMsg = constructChat("Test spam");
		System.out.println("Sending message: " + chatMsg);
		oos.write(chatMsg.getBytes());
		oos.flush();
		for (int i=0; i<1000; ++i){
			String msgSpam = constructChat("Spam!");
			//System.out.println("Sending message: " + msgSpam);
			oos.write(msgSpam.getBytes());
			oos.flush();
		}
//		String chatMsg = constructChat("Ale on spadne ne?");
//		System.out.println("Sending message: " + chatMsg);
//		oos.write(chatMsg.getBytes());
//		oos.flush();
		try{
			InputStreamReader ois = new InputStreamReader(socket.getInputStream());
			char[] cbuf = new char[512];
			ois.read(cbuf);
			String message = new String(cbuf);
			System.out.println("Message Received: " + message.trim());
			ois.close();
		}catch(Exception e){
			System.err.println(e);
		}



		oos.close();
		socket.close();
	}
}

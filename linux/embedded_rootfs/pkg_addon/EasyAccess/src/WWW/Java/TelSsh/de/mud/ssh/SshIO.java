/**
 * SshIO 
 * --
 * 
 * This file implments the SSH protocol 1.5
 *
 * $Id: SshIO.java,v 1.1 2007-06-18 12:30:41 jane Exp $
 *
 * The protocol version used in this document is SSH protocol version 1.5.
 * This file is part of "The Java Ssh Applet".
 */

package de.mud.ssh;

import java.io.IOException;
//import java.security.SecureRandom; //not supported by netscape
import de.mud.ssh.MD5; 

public abstract class SshIO 
{

  private MD5 md5 = new MD5();

  /**
   * variables for the connection
   */
  private String identification_string = ""; //("SSH-<protocolmajor>.<protocolminor>-<version>\n")
  private String identification_string_sent = "SSH-1.5-Java Ssh 1.1 (16/09/99) leo@mud.de, original by Cedric Gourio (javassh@france-mail.com)\n";

  /**
   * Debug level. This results in additional diagnostic messages on the
   * java console.
   */
  private static int debug = 0;

  /**
   * State variable for Ssh negotiation reader
   */
  private boolean encryption = false;
  private SshCrypto crypto;
  SshPacket lastPacketReceived;

  String cipher_type = "IDEA";
	
					
  private String login = "", password = ""; 
  //nobody is to access those fields  : better to use pivate, nobody knows :-)

  public String dataToSend = null;

  public String hashHostKey = null;  // equals to the applet parameter if any 

  byte lastPacketSentType;


   
  // phase : handleBytes
  private int phase = 0;
  private final int PHASE_INIT 			= 0;
  private final int PHASE_SSH_RECEIVE_PACKET	= 1;
	

		
  //handlePacket
  //messages
  //  The supported packet types and the corresponding message numbers are
  //	given in the following table.  Messages with _MSG_ in their name may
  //	be sent by either side.  Messages with _CMSG_ are only sent by the
  //  client, and messages with _SMSG_ only by the server.
  //
  private final int SSH_MSG_DISCONNECT =		1;
  private final int SSH_SMSG_PUBLIC_KEY =		2;
  private final int SSH_CMSG_SESSION_KEY =		3;	
  private final int SSH_CMSG_USER =			4;
  private final int SSH_CMSG_AUTH_PASSWORD =		9;
  private final int SSH_CMSG_REQUEST_PTY =		10;
  private final int SSH_CMSG_EXEC_SHELL =		12;
  private final int SSH_SMSG_SUCCESS =			14;
  private final int SSH_SMSG_FAILURE =			15;
  private final int SSH_CMSG_STDIN_DATA =		16;
  private final int SSH_SMSG_STDOUT_DATA =		17;
  private final int SSH_SMSG_STDERR_DATA =		18;
  private final int SSH_SMSG_EXITSTATUS =		20;
  private final int SSH_CMSG_EXIT_CONFIRMATION =	33;
  private final int SSH_MSG_DEBUG =	36;

  //used in getPacket
  private int position = 0;				// used to know, how far we are in packet_length_array[], padding[] ...


  //
  // encryption types
  //
  private int SSH_CIPHER_NONE = 0;	 // No encryption
  private int SSH_CIPHER_IDEA = 1;  // IDEA in CFB mode		(patented)
  private int SSH_CIPHER_DES  = 2;  // DES in CBC mode
  private int SSH_CIPHER_3DES = 3;  // Triple-DES in CBC mode
  private int SSH_CIPHER_TSS  = 4;  // An experimental stream cipher

  private int SSH_CIPHER_RC4  = 5;  // RC4			(patented)

  private int SSH_CIPHER_BLOWFISH  = 6;	// Bruce Scheiers blowfish (public d)


		
  //	
  // authentication methods 
  //
  private final int SSH_AUTH_RHOSTS =		1;   //.rhosts or /etc/hosts.equiv
  private final int SSH_AUTH_RSA =			2;   //pure RSA authentication
  private final int SSH_AUTH_PASSWORD =	3;   //password authentication, implemented !
  private final int SSH_AUTH_RHOSTS_RSA =  4;   //.rhosts with RSA host authentication



  private boolean cansenddata = false;

  /**
   * Initialise SshIO
   */
  public SshIO() {
    encryption = false;
  }

  public void setLogin(String user) {
    if(user == null) user = "";
    login = user;
  }

  public void setPassword(String password) {
    if(password == null) password = "";
    this.password = password;
  }

  /**
   * Read data from the remote host. Blocks until data is available. 
   * 
   * Returns an array of bytes that will be displayed.
   * 
   */
  synchronized public byte[] handleSSH(byte[] b) throws IOException {
    byte[] result = packetDone(handleBytes(b, 0, b.length));

    while(lastPacketReceived != null && lastPacketReceived.toBeFinished) {
      byte[] buff = lastPacketReceived.unfinishedBuffer;
      int start = lastPacketReceived.positionInUnfinishedBuffer;

      if(buff != null) {
        byte[] rest = packetDone(handleBytes(buff, start, buff.length));
	if(rest != null) {
	  if(result != null) {
	    byte[] tmp = new byte[rest.length + result.length];
            System.arraycopy(result, 0, tmp, 0, result.length);
	    System.arraycopy(rest, 0, tmp, result.length , rest.length);
	    result = tmp;
	} else
	  result=rest;
      }
    }
    } // while
    /*
    for(int i = 0; result != null && i < result.length; i++) 
      System.err.print(result[i]+":"+(char)result[i]+" ");
    */

    return result;
  }


  private byte[] packetDone(SshPacket packet) throws IOException {
    if(packet == null) return null;
    lastPacketReceived = packet;
    byte result[] = handlePacket(lastPacketReceived.getType(),
                                 lastPacketReceived.getData());
    return result;
 }

	
  protected abstract void write(byte[] buf) throws IOException;

  public abstract String getTerminalType();

  byte[] one = new byte[1];
  private void write(byte b) throws IOException {
    one[0] = b;
    write(one);
  }

  public void disconnect() {
    // System.err.println("In Disconnect");
    login = "";
    password = ""; 
    phase=0;
    encryption = false;
    lastPacketReceived = null;
  }


  synchronized public void sendData(String str) throws IOException {
    if(debug > 1) System.out.println( "SshIO.send(" + str + ")" );
    if (dataToSend==null) dataToSend = str;
    else dataToSend += str;
    if (cansenddata) {
	Send_SSH_CMSG_STDIN_DATA(dataToSend);
	dataToSend = null;
    }
  }

  private SshPacket handleBytes(byte buff[], int offset, int count) 
    throws IOException {
		
    if(debug > 1) 
      System.out.println("SshIO.getPacket(" + buff + "," + count + ")" );
		
    byte b;  		// of course, byte is a signed entity (-128 -> 127)
    int boffset = offset;	// offset in the buffer received 
	
		
    while(boffset < count) {
      b=buff[boffset++];	
      switch(phase) {
      case PHASE_INIT: 
	// both sides MUST send an identification string of the form 
	// "SSH-protoversion-softwareversion comments", 
	// followed by newline character(ascii 10 = '\n' or '\r') 				
	identification_string += (char) b;
	if (b=='\n') { 
	  phase++; 
	  write(identification_string_sent.getBytes());
	  position = 0;
	  byte[] data = SshMisc.createString(identification_string);
	  byte packet_type = SSH_SMSG_STDOUT_DATA;
	  return createPacket(packet_type, data);
	}
	break;
      case PHASE_SSH_RECEIVE_PACKET:
	SshPacket result = lastPacketReceived.getPacketfromBytes(buff,boffset-1,count,encryption,crypto);
	return 	result;
      } // switch(phase) 
    } 	//while(boffset < count) 
		
	
    return null;
  }//getPacket
		

  //
  // Create a packet 
  //

  private SshPacket createPacket(byte newType, byte[] newData)
  throws IOException { 
    return new SshPacket(newType, newData,encryption,crypto);
  } 
	
  private byte[] handlePacket(byte packetType, byte[] packetData) 
    throws IOException { //the message to handle is data and its length is 

    //if(debug > 1) w
    //System.out.println("SshIO.handlePacket("+data+","+ (packet_length - 5) +")");
		
    byte b;  		// of course, byte is a signed entity (-128 -> 127)
    int boffset = 0;	//offset in the buffer received 

    //we have to deal with data....
			
    if(debug > 0)
      System.out.println("1 packet to handle, type "+packetType);


    switch(packetType) {
    case SSH_MSG_DISCONNECT:
      String str = SshMisc.getString(boffset, packetData);
      disconnect();
      return str.getBytes();

    case SSH_SMSG_PUBLIC_KEY:
      byte[] anti_spoofing_cookie = new  byte[8];	//8 bytes
      byte[] server_key_bits = new  byte[4];		//32-bit int
      byte[] server_key_public_exponent;		//mp-int
      byte[] server_key_public_modulus;			//mp-int
      byte[] host_key_bits = new  byte[4];		//32-bit int
      byte[] host_key_public_exponent;			//mp-int
      byte[] host_key_public_modulus;			//mp-int
      byte[] protocol_flags = new  byte[4];		//32-bit int
      byte[] supported_ciphers_mask = new  byte[4];	//32-bit int
      byte[] supported_authentications_mask = new  byte[4];	//32-bit int
			
      for(int i=0;i<=7;i++) anti_spoofing_cookie[i] = packetData[boffset++];
      for(int i=0;i<=3;i++) server_key_bits[i] = packetData[boffset++];


      server_key_public_exponent = SshMisc.getMpInt(boffset,packetData); //boffset is not modified :-(
      boffset += server_key_public_exponent.length+2;

      server_key_public_modulus = SshMisc.getMpInt(boffset,packetData);
      boffset += server_key_public_modulus.length+2;

      for(int i=0;i<=3;i++) host_key_bits[i] = packetData[boffset++];

      host_key_public_exponent = SshMisc.getMpInt(boffset,packetData);
      boffset += host_key_public_exponent.length+2;

      host_key_public_modulus = SshMisc.getMpInt(boffset,packetData); // boffset can not be modified (Java = crap Language)

      boffset += host_key_public_modulus.length+2;
      for(int i=0;i<4;i++) protocol_flags[i] = packetData[boffset++];
      for(int i=0;i<4;i++) supported_ciphers_mask[i] = packetData[boffset++];
      for(int i=0;i<4;i++) supported_authentications_mask[i] = packetData[boffset++];

	// we have completely received the PUBLIC_KEY
	// we prepare the answer ...

      byte ret[] = Send_SSH_CMSG_SESSION_KEY(
	anti_spoofing_cookie, server_key_public_modulus,
	host_key_public_modulus, supported_ciphers_mask,
	server_key_public_exponent, host_key_public_exponent
      );
      if (ret != null)
	  return ret;
				
	// we check if MD5(server_key_public_exponent) is equals to the 
	// applet parameter if any .
      if (hashHostKey!=null && hashHostKey.compareTo("")!=0) {
	// we compute hashHostKeyBis the hash value in hexa of 
	// host_key_public_modulus
	byte[] Md5_hostKey = md5.hash(host_key_public_modulus);
	String hashHostKeyBis = "";
	for(int i=0; i < Md5_hostKey.length; i++) {
	  String hex = "";
	  int[] v = new int[2];
	  v[0] = (Md5_hostKey[i]&240)>>4;
	  v[1] = (Md5_hostKey[i]&15);
	  for (int j=0; j<1; j++)
	    switch (v[j]) {
	    case 10 : hex +="a"; break;
	    case 11 : hex +="b"; break;
	    case 12 : hex +="c"; break;
	    case 13 : hex +="d"; break;
	    case 14 : hex +="e"; break;
	    case 15 : hex +="f"; break;
	    default : hex +=String.valueOf(v[j]);break;
	    }
	  hashHostKeyBis = hashHostKeyBis + hex;
	}
	//we compare the 2 values
	if (hashHostKeyBis.compareTo(hashHostKey)!=0) {
	  login = password = "";
	  return ("\nHash value of the host key not correct \r\n"
		 +"login & password have been reset \r\n"
	  	 +"- erase the 'hashHostKey' parameter in the Html\r\n"
	  	 +"(it is used for auhentificating the server and "
	  	 +"prevent you from connecting \r\n"
	  	 +"to any other)\r\n").getBytes();
	}
      }
      break;

    case SSH_SMSG_SUCCESS:
      if(debug > 0)
        System.out.println("SSH_SMSG_SUCCESS (last packet was "+lastPacketSentType+")");
      if (lastPacketSentType==SSH_CMSG_SESSION_KEY) { 
        //we have succefully sent the session key !! (at last :-) )
	Send_SSH_CMSG_USER();
	break;
      }

      if (lastPacketSentType==SSH_CMSG_USER) { 
        // authentication is NOT needed for this user 
	Send_SSH_CMSG_REQUEST_PTY(); //request a pseudo-terminal
	return "\nEmpty password login.\r\n".getBytes();
      }

      if (lastPacketSentType==SSH_CMSG_AUTH_PASSWORD) {// password correct !!!
	//yahoo
	System.out.println("login succesful");

	//now we have to start the interactive session ...
	Send_SSH_CMSG_REQUEST_PTY(); //request a pseudo-terminal
	return "\nLogin & password accepted\r\n".getBytes();
      }

      if (lastPacketSentType==SSH_CMSG_REQUEST_PTY) {// pty accepted !!
        /* we can send data with a pty accepted ... no need for a shell. */
        cansenddata = true;
	if (dataToSend != null ) {
	    Send_SSH_CMSG_STDIN_DATA(dataToSend);
	    dataToSend = null;
	}
	Send_SSH_CMSG_EXEC_SHELL(); //we start a shell
	break;
      }
      if (lastPacketSentType==SSH_CMSG_EXEC_SHELL) {// shell is running ...
        /* empty */
      }

      break;

    case SSH_SMSG_FAILURE:
      if (lastPacketSentType==SSH_CMSG_AUTH_PASSWORD) {// password incorrect ???
        System.out.println("failed to log in");
        disconnect();
	return "\nLogin & password not accepted\r\n".getBytes();
      }
      if (lastPacketSentType==SSH_CMSG_USER) { 
        // authentication is needed for the given user 
	// (in most cases that's true)
	Send_SSH_CMSG_AUTH_PASSWORD();
	break;
      }

      if (lastPacketSentType==SSH_CMSG_REQUEST_PTY) {// pty not accepted !!
	break;
      }
      break;

    case SSH_SMSG_STDOUT_DATA: //receive some data from the server
      str = SshMisc.getString(0, packetData);
      return str.getBytes();

    case SSH_SMSG_STDERR_DATA: //receive some error data from the server
      //	if(debug > 1) 
      str = "Error : " + SshMisc.getString(0, packetData);
      System.out.println("SshIO.handlePacket : " + "STDERR_DATA " + str );
      //StrByte = new byte[str.length()];
      //str.getBytes(0, str.length(),	StrByte, 0);
      //return(StrByte);
      return str.getBytes();
			
    case SSH_SMSG_EXITSTATUS: //sent by the server to indicate that 
                              // the client program has terminated.
	//32-bit int   exit status of the command
      int value = (packetData[0]<<24)+(packetData[1]<<16)
                + (packetData[2]<<8)+(packetData[3]);
      Send_SSH_CMSG_EXIT_CONFIRMATION();
      System.out.println("SshIO : Exit status " + value );
      disconnect();
      break;

   case SSH_MSG_DEBUG:
      str = SshMisc.getString(0, packetData);
      if(debug > 0) {
        System.out.println("SshIO.handlePacket : " + " DEBUG " + str );

      // bad bad bad bad bad. We should not do actions in DEBUG messages,
      // but apparently some SSH demons does not send SSH_SMSG_FAILURE for
      // just USER CMS.
/*
      if(lastPacketSentType==SSH_CMSG_USER) { 
        Send_SSH_CMSG_AUTH_PASSWORD();
        break;
      }
*/
        return str.getBytes();
      } 
      return "".getBytes();

    default: 
      System.err.print("SshIO.handlePacket : Packet Type unknown: "+packetType);
      break;
		
    }//	switch(b)
    return null;
  } // handlePacket

  private void sendPacket(SshPacket packet) throws IOException {  
    write(packet.getBytes()); 
    lastPacketSentType = packet.getType();
  } 

  // 
  // Send_SSH_CMSG_SESSION_KEY
  // Create : 
  // the session_id, 
  // the session_key, 
  // the Xored session_key, 
  // the double_encrypted session key
  // send SSH_CMSG_SESSION_KEY
  // Turn the encryption on (initialise the block cipher)
  //

  private byte[] Send_SSH_CMSG_SESSION_KEY(byte[] anti_spoofing_cookie,
					   byte[] server_key_public_modulus,
					   byte[] host_key_public_modulus,
					   byte[] supported_ciphers_mask,
					   byte[] server_key_public_exponent,
					   byte[] host_key_public_exponent)
	throws IOException {
		
    String str;
    int boffset;
		
    byte cipher_types;		//encryption types
    byte[] session_key;		//mp-int

    // create the session id
    //	session_id = md5(hostkey->n || servkey->n || cookie) //protocol V 1.5. (we use this one)
    //	session_id = md5(servkey->n || hostkey->n || cookie) //protocol V 1.1.(Why is it different ??)
    //
		
    byte[] session_id_byte = new byte[host_key_public_modulus.length+server_key_public_modulus.length+anti_spoofing_cookie.length];

		System.arraycopy(host_key_public_modulus,0,session_id_byte,0,host_key_public_modulus.length);

		System.arraycopy(server_key_public_modulus,0,session_id_byte,host_key_public_modulus.length,server_key_public_modulus.length);

		System.arraycopy(anti_spoofing_cookie,0,session_id_byte,host_key_public_modulus.length+server_key_public_modulus.length,anti_spoofing_cookie.length);

    byte[] hash_md5 = md5.hash(session_id_byte); 


    //	SSH_CMSG_SESSION_KEY : Sent by the client
    //	    1 byte       cipher_type (must be one of the supported values)
    // 	    8 bytes      anti_spoofing_cookie (must match data sent by the server)
    //	    mp-int       double-encrypted session key (uses the session-id)
    //	    32-bit int   protocol_flags
    //
    if ((supported_ciphers_mask[3] & (byte)(1<<SSH_CIPHER_BLOWFISH))!=0) {
      cipher_types = (byte)SSH_CIPHER_BLOWFISH;
      cipher_type = "Blowfish";
    } else {
      if ((supported_ciphers_mask[3] & (1<<SSH_CIPHER_IDEA)) != 0) {
	cipher_types = (byte)SSH_CIPHER_IDEA;
	cipher_type = "IDEA";
      } else {
	if ((supported_ciphers_mask[3] & (1<<SSH_CIPHER_3DES)) != 0) {
	  cipher_types = (byte)SSH_CIPHER_3DES;
	  cipher_type = "DES3";
	} else {
	  System.err.println("SshIO: remote server does not supported IDEA, BlowFish or 3DES, support cypher mask is "+supported_ciphers_mask[3]+".\n");
	  disconnect();
	  return "\rRemote server does not support IDEA/Blowfish/3DES blockcipher, closing connection.\r\n".getBytes();
	}
      }
    }
    System.out.println("SshIO: Using "+cipher_type+" blockcipher.\n");


    // 	anti_spoofing_cookie : the same 
    //      double_encrypted_session_key :
    //		32 bytes of random bits
    //		Xor the 16 first bytes with the session-id
    //		encrypt with the server_key_public (small) then the host_key_public(big) using RSA.
    //
		
    //32 bytes of random bits
    byte[] random_bits1 = new byte[16], random_bits2 = new byte[16];
		

    /// java.util.Date date = new java.util.Date(); ////the number of milliseconds since January 1, 1970, 00:00:00 GMT. 
    //Math.random()   a pseudorandom double between 0.0 and 1.0. 
    random_bits2 = random_bits1 =
      // md5.hash("" + Math.random() * (new java.util.Date()).getDate());
      md5.hash("" + Math.random() * (new java.util.Date()).getTime());

    random_bits1 = md5.hash(SshMisc.addArrayOfBytes(md5.hash(password+login), 
                            random_bits1));
    random_bits2 = md5.hash(SshMisc.addArrayOfBytes(md5.hash(password+login),
                            random_bits2));

    // SecureRandom random = new java.security.SecureRandom(random_bits1); //no supported by netscape :-(
    // random.nextBytes(random_bits1);
    // random.nextBytes(random_bits2);

    session_key  = SshMisc.addArrayOfBytes(random_bits1,random_bits2);

    //Xor the 16 first bytes with the session-id
    byte[] session_keyXored  = SshMisc.XORArrayOfBytes(random_bits1,hash_md5);
    session_keyXored = SshMisc.addArrayOfBytes(session_keyXored, random_bits2);

    //We encrypt now!!
    byte[] encrypted_session_key = 
      SshCrypto.encrypteRSAPkcs1Twice(session_keyXored,
				      server_key_public_exponent,
				      server_key_public_modulus,
				      host_key_public_exponent,
				      host_key_public_modulus);

    //	protocol_flags :protocol extension   cf. page 18
    byte[] protocol_flags = new  byte[4];	//32-bit int
    protocol_flags [0] = protocol_flags [1] = 
    protocol_flags [2] = protocol_flags [3] = 0;

    //set the data
    int length = 1 + //cipher_type
      anti_spoofing_cookie.length + 
      encrypted_session_key.length +
      protocol_flags.length;


    byte[] data = new byte[length];
    boffset = 0;
    data[boffset++] = (byte) cipher_types;

    for (int i=0; i<8; i++)
	data[boffset++] = anti_spoofing_cookie[i];

    for (int i=0; i<encrypted_session_key.length; i++)
	data[boffset++] = encrypted_session_key[i];

    for (int i=0; i<4; i++)
	data[boffset++] = protocol_flags[i];
		
    //set the packet_type
    byte packet_type = SSH_CMSG_SESSION_KEY;
    SshPacket packet = createPacket(packet_type, data);
    sendPacket(packet);

    crypto = new SshCrypto(cipher_type,session_key);
    encryption=true;
    return null;
  } //Send_SSH_CMSG_SESSION_KEY

  /**
   * SSH_CMSG_USER
   * string   user login name on server
   */
  private byte[] Send_SSH_CMSG_USER() throws IOException {
    if(debug > 0) System.err.println("Send_SSH_CMSG_USER("+login+")");
    byte[] data = SshMisc.createString(login);
    byte packet_type = SSH_CMSG_USER;
    SshPacket packet = createPacket(packet_type, data);
    sendPacket(packet);
    return null;
  } //Send_SSH_CMSG_USER

  /**
   * Send_SSH_CMSG_AUTH_PASSWORD
   * string   user password
   */
  private byte[] Send_SSH_CMSG_AUTH_PASSWORD() throws IOException {
    byte[] data = SshMisc.createString(password); 
    byte packet_type = SSH_CMSG_AUTH_PASSWORD;
    SshPacket packet = createPacket(packet_type, data);
    sendPacket(packet);
    return null;
  } //Send_SSH_CMSG_AUTH_PASSWORD

  /**
   * Send_SSH_CMSG_EXEC_SHELL
   *  (no arguments)
   *   Starts a shell (command interpreter), and enters interactive
   *   session mode.
   */
  private byte[] Send_SSH_CMSG_EXEC_SHELL() throws IOException {
    byte[] data = null;
    byte packet_type = SSH_CMSG_EXEC_SHELL;
    SshPacket packet = createPacket(packet_type, data);
    sendPacket(packet);
    lastPacketSentType = packet_type;
    return null;
  } //Send_SSH_CMSG_EXEC_SHELL

  /**
   * Send_SSH_CMSG_STDIN_DATA
   *  
   */
  private byte[] Send_SSH_CMSG_STDIN_DATA(String str) throws IOException {
    byte[] data = SshMisc.createString(str);
    byte packet_type = SSH_CMSG_STDIN_DATA;
    SshPacket packet = createPacket(packet_type, data);
    sendPacket(packet);
    return null;
  } //Send_SSH_CMSG_STDIN_DATA
	
  /**
   * Send_SSH_CMSG_REQUEST_PTY
   *   string       TERM environment variable value (e.g. vt100)
   *   32-bit int   terminal height, rows (e.g., 24)
   *   32-bit int   terminal width, columns (e.g., 80)
   *   32-bit int   terminal width, pixels (0 if no graphics) (e.g., 480)
   */
  private byte[] Send_SSH_CMSG_REQUEST_PTY() throws IOException {
    byte[] termType = SshMisc.createString(getTerminalType()); // terminal type
    byte[] row = new byte[4];
    row[3] = (byte) 24;			// terminal height
    byte[] col = new byte[4];
    col[3] = (byte) 80;			// terminal width
    byte[] XPixels = new byte[4];
    //XPixels[2] = (byte)(480/256);
    //XPixels[3] = (byte)(480%256);
    byte[] YPixels = new byte[4];
    //YPixels[2] = (byte)(640/256);
    //YPixels[3] = (byte)(640%256);	
    byte[] terminalModes = new byte[1];
    terminalModes[0] =  0;

    byte [] data = new byte[termType.length + 4*4 + terminalModes.length];
    int offset = 0;
    for (int i=0; i<termType.length; i++) data[offset++] = termType[i];
    for (int i=0; i<4; i++) data[offset++] = row[i];
    for (int i=0; i<4; i++) data[offset++] = col[i];
    for (int i=0; i<4; i++) data[offset++] = XPixels[i];
    for (int i=0; i<4; i++) data[offset++] = YPixels[i];
    for (int i=0; i<terminalModes.length; i++) 
      data[offset++] = terminalModes[i];

    byte packet_type =  SSH_CMSG_REQUEST_PTY;

    SshPacket packet = createPacket(packet_type, data);
    sendPacket(packet);
    return null;
  } //Send_SSH_CMSG_REQUEST_PTY

  private byte[] Send_SSH_CMSG_EXIT_CONFIRMATION() throws IOException {
    byte packet_type = SSH_CMSG_EXIT_CONFIRMATION;
    SshPacket packet = createPacket(packet_type, null);
    sendPacket(packet);
    return null;
  }
}// class SshIO

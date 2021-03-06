package com.code;

public class Dialer extends Thread 
{
    private native int  dialConnection (String[] args, int timeOut); 
    private native int  closeConnection (String[] args);
	private native String resolveHostName(String strHost);
	
    String[] param;
    SSLApplet applet = null;
    int retVal;
    int timeOut = 0;
    boolean stopped = false;

    static 
      {
        System.load("/vpclient/libdialer.so");
      }

    public String getResolvedHost()
      {
	      return param[0];
      }  
     
    public	Dialer
    (String[] args, String strLogFile, boolean isLoggingEnabled, boolean isFullTunnelEnabled) 
      { 
        super("Dialer");
        param = new String[5];
        param[0] = resolveHostName(args[0]);
        param[1] = args[1];
        param[2] = (isLoggingEnabled) ? strLogFile : "";
        param[3] = ""+isFullTunnelEnabled;
	param[4] = args[2];
      }

    public void setContainer(SSLApplet applet)
      {
        this.applet = applet;
      }

    public void setTimeout(int timeOut)
      {
        this.timeOut = timeOut;
      }

    public void run()
      {
        /*  JNI call to initiate ppp connection */
        Debug.logMessage("About to invoke jni function - dialConnection");

        retVal = dialConnection (param, timeOut);

        Debug.logMessage("dialConnection returned "+retVal);

        if(applet != null)
          {
	        Debug.logMessage("Notifying applet");
          	
            applet.notifyExitStatus(retVal);
          }
		Debug.logMessage("Dialer Thread about to finish");      	
		
        return ;
      }

    public void stopConnection()
      {
        if(stopped)
          return;

        /*  JNI call to terminate ppp connection */
        Debug.logMessage("About to invoke jni function - closeConnection");
        
        retVal = closeConnection(param);

        Debug.logMessage("closeConnection returned "+retVal);
        
        stopped = true;
      }
}

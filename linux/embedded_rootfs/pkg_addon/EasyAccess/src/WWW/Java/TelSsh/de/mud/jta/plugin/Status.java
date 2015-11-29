/*
 * This file is part of "The Java Telnet Application".
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * "The Java Telnet Application" is distributed in the hope that it will be 
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

package de.mud.jta.plugin;

import de.mud.jta.Plugin;
import de.mud.jta.PluginBus;
import de.mud.jta.VisualPlugin;
import de.mud.jta.event.OnlineStatusListener;
import de.mud.jta.event.SocketListener;
import de.mud.jta.event.ConfigurationListener;
import de.mud.jta.PluginConfig;

import java.awt.Component;
import java.awt.Panel;
import java.awt.BorderLayout;
import java.awt.Menu;
import java.awt.Label;
import java.awt.Color;
import java.awt.Font;

import java.util.Hashtable;
import java.net.URL;
import java.io.InputStreamReader;
import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.IOException;


public class Status extends Plugin implements VisualPlugin, Runnable {

  private final static int debug = 1;

  private Label status;
  private Label host;
  private Panel sPanel;

  private String address, port;

  private String infoURL;
  private int interval;
  private Thread infoThread;

  private Hashtable ports = new Hashtable();

  public Status(PluginBus bus, final String id) {
    super(bus, id);

    // setup the info
    bus.registerPluginListener(new ConfigurationListener() {
      public void setConfiguration(PluginConfig config) {
        infoURL = config.getProperty("Status", id, "info");
	if(infoURL != null)
	  host.setAlignment(Label.CENTER);
        String tmp;
	if((tmp = config.getProperty("Status", id, "font")) != null) {
          String font = tmp;
          int style = Font.PLAIN, fsize = 12;
          if((tmp = config.getProperty("Status", id, "fontSize")) != null)
            fsize = Integer.parseInt(tmp);
          String fontStyle = config.getProperty("Status", id, "fontStyle");
          if(fontStyle == null || fontStyle.equals("plain"))
            style = Font.PLAIN;
          else if(fontStyle.equals("bold"))
            style = Font.BOLD;
          else if(fontStyle.equals("italic"))
            style = Font.ITALIC;
          else if(fontStyle.equals("bold+italic"))
            style = Font.BOLD | Font.ITALIC;
          host.setFont(new Font(font, style, fsize));
        }

	if((tmp = config.getProperty("Status", id, "foreground")) != null)
	  host.setForeground(Color.decode(tmp));

	if((tmp = config.getProperty("Status", id, "background")) != null)
	  host.setBackground(Color.decode(tmp));

	if(config.getProperty("Status", id, "interval") != null) {
	  try {
	    interval = Integer.parseInt(
	      config.getProperty("Status", id, "interval"));
	    infoThread = new Thread(Status.this);
	    infoThread.start();
	  } catch(NumberFormatException e) {
	    Status.this.error("interval is not a number");
	  }
	}
      }
    });


    // fill port hashtable
    ports.put("22", "ssh");
    ports.put("23", "telnet");
    ports.put("25", "smtp");
   
    sPanel = new Panel(new BorderLayout());

    host = new Label("Not connected.", Label.LEFT);

    bus.registerPluginListener(new SocketListener() {
	//Rain -->
      //public void connect(String tunnelServer, String session, String addr, int p, String service) {
	  public void connect(String tunnelServer, String session, String addr, int p, String service, int https_port) {
	  //<-- Rain
        address = addr;
	if(address == null || address.length() == 0)
	  address = "<unknown host>";
	if(ports.get(""+p) != null)
	  port = (String)ports.get(""+p);
	else
	  port = ""+p;
        if(infoURL == null)
	  host.setText("Trying "+address+" "+port+" ...");
      }
      public void disconnect() {
        if(infoURL == null)
	  host.setText("Not connected.");
      }
    });

    sPanel.add("Center", host);

    status = new Label("offline", Label.CENTER);

    bus.registerPluginListener(new OnlineStatusListener() {
      public void online() {
        status.setText("online");
	status.setBackground(Color.green);
	if(infoURL == null)
	  host.setText("Connected to "+address+" "+port);
      }
      public void offline() {
        status.setText("offline");
	status.setBackground(Color.red);
        if(infoURL == null)
	  host.setText("Not connected.");
      }
    });

    sPanel.add("East", status);

  }

  public void run() {
    URL url = null;
    try {
      url = new URL(infoURL);
    } catch(Exception e) {
      error("infoURL is not valid: "+e);
      infoURL = null;
      return;
    }

    while(url != null && infoThread != null) {
      try {
        BufferedReader content = 
	  new BufferedReader(new InputStreamReader(url.openStream()));
	try {
	  String line;
	  while((line = content.readLine()) != null) {
	    if(line.startsWith("#")) {
	      String color = line.substring(1,7);
	      line = line.substring(8);
	      host.setForeground(Color.decode("#"+color));
	    }
	    host.setText(line);
	    infoThread.sleep(10*interval);
	  }
	} catch(IOException e) {
	  error("error while loading info ...");
	}
	infoThread.sleep(100*interval);
      } catch(Exception e) {
        error("error retrieving info content: "+e);
	e.printStackTrace();
	host.setForeground(Color.red);
	host.setText("error retrieving info content");
	infoURL = null;
	return;
      }
    } 
  }      

  public Component getPluginVisual() {
    return sPanel;
  }

  public Menu getPluginMenu() {
    return null;
  }
}

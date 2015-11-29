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
import de.mud.jta.FilterPlugin;
import de.mud.jta.PluginBus;
import de.mud.jta.PluginConfig;
import de.mud.jta.event.LocalEchoRequest;
import de.mud.jta.event.SocketListener;
import de.mud.jta.event.ConfigurationListener;
import de.mud.jta.event.OnlineStatus;

// import java.io.InputStream;
// import java.io.OutputStream;
import java.io.IOException;

public class Shell extends Plugin implements FilterPlugin {

  protected String shellCommand;

  private HandlerPTY pty;

  public Shell(final PluginBus bus, final String id) {
    super(bus, id);

    bus.registerPluginListener(new ConfigurationListener() {
      public void setConfiguration(PluginConfig cfg) {
        String tmp;
	if((tmp = cfg.getProperty("Shell", id, "command")) != null) {
	  shellCommand = tmp;
	  // System.out.println("Shell: Setting config " + tmp); // P3
        } else {
	  // System.out.println("Shell: Not setting config"); // P3
	  shellCommand = "/bin/sh";
        }
      }
    });

    bus.registerPluginListener(new SocketListener() {
      // we do actually ignore these parameters
	  //Rain -->
      //public void connect(String tunnelServer, String session, String host, int port, String service) {
	  public void connect(String tunnelServer, String session, String host, int port, String service, int https_port) {
	  //<-- Rain
        // XXX Fix this together with window size changes
        // String ttype = (String)bus.broadcast(new TerminalTypeRequest());
        // String ttype = getTerminalType();
        // if(ttype == null) ttype = "dumb";

	// XXX Add try around here to catch missing DLL/.so.
	pty = new HandlerPTY();

        if(pty.start(shellCommand) == 0) {
	  bus.broadcast(new OnlineStatus(true));
        } else {
	  bus.broadcast(new OnlineStatus(false));
        }
      }
      public void disconnect() {
        bus.broadcast(new OnlineStatus(false));
        pty = null;
      }
    });
  }

  public void setFilterSource(FilterPlugin plugin) {
    // we do not have a source other than our socket
  }

  public int read(byte[] b) throws IOException {
    if(pty == null) return 0;
    int ret = pty.read(b);
    if(ret <= 0) {
      throw new IOException("EOF on PTY");
    }
    return ret;
  }

  public void write(byte[] b) throws IOException {
    if(pty != null) pty.write(b);
  }
}

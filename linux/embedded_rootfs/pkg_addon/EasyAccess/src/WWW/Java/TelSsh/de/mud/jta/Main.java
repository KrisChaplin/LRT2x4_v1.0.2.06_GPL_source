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
package de.mud.jta;

import de.mud.jta.event.FocusStatusListener;
import de.mud.jta.event.OnlineStatusListener;
import de.mud.jta.event.ReturnFocusRequest;
import de.mud.jta.event.SocketRequest;

import java.awt.*;
import java.awt.datatransfer.Clipboard;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.FileInputStream;
import java.io.IOException;
import java.net.URL;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Properties;

public class Main {

  private final static int debug = 0;

  private final static boolean personalJava = false;

  /** holds the last focussed plugin */
  private static Plugin focussedPlugin;

  /** holds the system clipboard or our own */
  private static Clipboard clipboard;
  private static HelpFrame helpFrame;

  public static void main(String args[]) {
    final Properties options = new Properties();
    try {
      options.load(Main.class.getResourceAsStream("/de/mud/jta/default.conf"));
    } catch (IOException e) {
      System.err.println("jta: cannot load default.conf");
    }
    String error = parseOptions(options, args);
    if (error != null) {
      System.err.println(error);
      System.err.println("usage: de.mud.jta.Main [-plugins pluginlist] "
                         + "[-addplugin plugin] "
                         + "[-config url_or_file] "
                         + "[-term id] [host [port]]");
      System.exit(0);
    }

    String cfg = options.getProperty("Main.config");
    if (cfg != null)
      try {
        options.load(new URL(cfg).openStream());
      } catch (IOException e) {
        try {
          options.load(new FileInputStream(cfg));
        } catch (Exception fe) {
          System.err.println("jta: cannot load " + cfg);
        }
      }

    final String host = options.getProperty("Socket.host");
    final String port = options.getProperty("Socket.port");
    final String tunnelServer = new String("10.0.0.103");
    final String service = new String("SSH");
	//Rain
	final String https_port = new String("443");
	
    final Frame frame = new Frame("jta: " + host + (port.equals("23")?"":" " + port));

    // set up the clipboard
    try {
      clipboard = frame.getToolkit().getSystemClipboard();
    } catch (Exception e) {
      System.err.println("jta: system clipboard access denied");
      System.err.println("jta: copy & paste only within the JTA");
      clipboard = new Clipboard("de.mud.jta.Main");
    }

    // configure the application and load all plugins
    final Common setup = new Common(options);

    setup.registerPluginListener(new OnlineStatusListener() {
      public void online() {
        frame.setTitle("jta: " + host + (port.equals("23")?"":" " + port));
      }

      public void offline() {
        frame.setTitle("jta: offline");
      }
    });

    // register a focus status listener, so we know when a plugin got focus
    setup.registerPluginListener(new FocusStatusListener() {
      public void pluginGainedFocus(Plugin plugin) {
        if (Main.debug > 0)
          System.err.println("Main: " + plugin + " got focus");
        focussedPlugin = plugin;
      }

      public void pluginLostFocus(Plugin plugin) {
        // we ignore the lost focus
        if (Main.debug > 0)
          System.err.println("Main: " + plugin + " lost focus");
      }
    });

    Hashtable componentList = setup.getComponents();
    Enumeration names = componentList.keys();
    while (names.hasMoreElements()) {
      String name = (String) names.nextElement();
      Component c = (Component) componentList.get(name);
      if (options.getProperty("layout." + name) == null) {
        System.err.println("jta: no layout property set for '" + name + "'");
        frame.add("South", c);
      } else
        frame.add(options.getProperty("layout." + name), c);
    }

    if (!personalJava) {

      frame.addWindowListener(new WindowAdapter() {
        public void windowClosing(WindowEvent evt) {
          setup.broadcast(new SocketRequest());
          frame.setVisible(false);
          frame.dispose();
          System.exit(0);
        }
      });

      // add a menu bar
      MenuBar mb = new MenuBar();
      Menu file = new Menu("File");
      file.setShortcut(new MenuShortcut(KeyEvent.VK_H, true));
      MenuItem tmp;
      file.add(tmp = new MenuItem("Connect"));
      tmp.addActionListener(new ActionListener() {
        public void actionPerformed(ActionEvent evt) {
		//Rain -->
          //setup.broadcast(new SocketRequest(tunnelServer, "admin:AccessPoint", host, Integer.parseInt(port), service));
		  setup.broadcast(new SocketRequest(tunnelServer, "admin:AccessPoint", host, Integer.parseInt(port), service, Integer.parseInt(https_port)));
		//<-- Rain
        }
      });
      file.add(tmp = new MenuItem("Disconnect"));
      tmp.addActionListener(new ActionListener() {
        public void actionPerformed(ActionEvent evt) {
          setup.broadcast(new SocketRequest());
        }
      });
      file.add(new MenuItem("-"));
      file.add(tmp = new MenuItem("Print"));
      tmp.addActionListener(new ActionListener() {
        public void actionPerformed(ActionEvent evt) {
          if (setup.getComponents().get("Terminal") != null) {
            PrintJob printJob =
                    frame.getToolkit().getPrintJob(frame, "JTA Terminal", null);
            // return if the user clicked cancel
            if (printJob == null) return;
            ((Component) setup.getComponents().get("Terminal"))
                    .print(printJob.getGraphics());
            printJob.end();
          }
        }
      });
      file.add(new MenuItem("-"));
      file.add(tmp = new MenuItem("Exit"));
      tmp.addActionListener(new ActionListener() {
        public void actionPerformed(ActionEvent evt) {
          frame.dispose();
          System.exit(0);
        }
      });
      mb.add(file);

      Menu edit = new Menu("Edit");
      edit.setShortcut(new MenuShortcut(KeyEvent.VK_H, true));
      edit.add(tmp = new MenuItem("Copy"));
      tmp.addActionListener(new ActionListener() {
        public void actionPerformed(ActionEvent evt) {
          if (focussedPlugin instanceof VisualTransferPlugin)
            ((VisualTransferPlugin) focussedPlugin).copy(clipboard);
        }
      });
      edit.add(tmp = new MenuItem("Paste"));
      tmp.addActionListener(new ActionListener() {
        public void actionPerformed(ActionEvent evt) {
          if (focussedPlugin instanceof VisualTransferPlugin)
            ((VisualTransferPlugin) focussedPlugin).paste(clipboard);
        }
      });
      mb.add(edit);

      Hashtable menuList = setup.getMenus();
      names = menuList.keys();
      while (names.hasMoreElements()) {
        String name = (String) names.nextElement();
        mb.add((Menu) menuList.get(name));
      }

      Menu help = new Menu("Help");
      help.setShortcut(new MenuShortcut(KeyEvent.VK_HELP, true));
      help.add(tmp = new MenuItem("General"));
      tmp.addActionListener(new ActionListener() {
        public void actionPerformed(ActionEvent e) {
          if (helpFrame == null) {
            helpFrame = new HelpFrame(options.getProperty("Help.url"));
          }
          helpFrame.setVisible(true);
        }
      });
      mb.setHelpMenu(help);

      frame.setMenuBar(mb);

    } // !personalJava

    frame.pack();

    if ((new Boolean(options.getProperty("Applet.detach.fullscreen"))
            .booleanValue()))
      frame.setSize(frame.getToolkit().getScreenSize());
    else
      frame.pack();

    frame.show();

	//Rain -->
    //setup.broadcast(new SocketRequest(tunnelServer, "admin:AccessPoint", host, Integer.parseInt(port), service));
	setup.broadcast(new SocketRequest(tunnelServer, "admin:AccessPoint", host, Integer.parseInt(port), service, Integer.parseInt(https_port)));
	//<-- Rain
    /* make sure the focus goes somewhere to start off with */
    setup.broadcast(new ReturnFocusRequest());
  }

  /**
   * Parse the command line argumens and override any standard options
   * with the new values if applicable.
   * <P><SMALL>
   * This method did not work with jdk 1.1.x as the setProperty()
   * method is not available. So it uses now the put() method from
   * Hashtable instead.
   * </SMALL>
   * @param options the original options
   * @param args the command line parameters
   * @return a possible error message if problems occur
   */
  private static String parseOptions(Properties options, String args[]) {
    boolean host = false, port = false;
    for (int n = 0; n < args.length; n++) {
      if (args[n].equals("-config"))
        if (!args[n + 1].startsWith("-"))
          options.put("Main.config", args[++n]);
        else
          return "missing parameter for -config";
      else if (args[n].equals("-plugins"))
        if (!args[n + 1].startsWith("-"))
          options.put("plugins", args[++n]);
        else
          return "missing parameter for -plugins";
      else if (args[n].equals("-addplugin"))
        if (!args[n + 1].startsWith("-"))
          options.put("plugins", args[++n] + "," + options.get("plugins"));
        else
          return "missing parameter for -addplugin";
      else if (args[n].equals("-term"))
        if (!args[n + 1].startsWith("-"))
          options.put("Terminal.id", args[++n]);
        else
          return "missing parameter for -term";
      else if (!host) {
        options.put("Socket.host", args[n]);
        host = true;
      } else if (host && !port) {
        options.put("Socket.port", args[n]);
        port = true;
      } else
        return "unknown parameter '" + args[n] + "'";
    }
    return null;
  }
}

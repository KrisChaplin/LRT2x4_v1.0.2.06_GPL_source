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

package de.mud.telnet;

import java.util.Vector;

public class ScriptHandler {

  /** debugging level */
  private final static int debug = 0;

  private int matchPos;		// current position in the match
  private byte[] match;		// the current bytes to look for
  private boolean done = true;	// nothing to look for!

  /**
   * Setup the parser using the passed string. 
   * @param match the string to look for
   */
  public void setup(String match) {
    if(match == null) return;
    this.match = match.getBytes();
    matchPos = 0;
    done = false;
  }

  /**
   * Try to match the byte array s against the match string.
   * @param s the array of bytes to match against
   * @param length the amount of bytes in the array
   * @return true if the string was found, else false
   */
  public boolean match(byte[] s, int length) {
    if(done) return true;
    for(int i = 0; !done && i < length; i++) {
      if(s[i] == match[matchPos]) {
        // the whole thing matched so, return the match answer 
        // and reset to use the next match
        if(++matchPos >= match.length) {
          done = true;
          return true;
        }
      } else
        matchPos = 0; // get back to the beginning
    }
    return false;
  }
}

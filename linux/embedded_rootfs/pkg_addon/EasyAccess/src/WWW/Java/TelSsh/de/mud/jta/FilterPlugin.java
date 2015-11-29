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

import java.io.IOException;

public interface FilterPlugin {
  /**
   * Set the source plugin where we get our data from and where the data
   * sink (write) is. The actual data handling should be done in the
   * read() and write() methods.
   * @param source the data source
   */
  public void setFilterSource(FilterPlugin source) 
    throws IllegalArgumentException;

  /**
   * Read a block of data from the back end.
   * @param b the buffer to read the data into
   * @return the amount of bytes actually read
   */
  public int read(byte[] b)
    throws IOException;

  /**
   * Write a block of data to the back end.
   * @param b the buffer to be sent
   */
  public void write(byte[] b)
    throws IOException;
}

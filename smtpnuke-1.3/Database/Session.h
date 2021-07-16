// Session.h
/*
Copyright (C) 2001-2005  Anders Hedstrom

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef _SESSION_H
#define _SESSION_H

class Session
{
public:
                        Session();
  virtual               ~Session();

  int			Connect(const char *) { return -1; }
  int			Connect(const char *,const char *,const char *) { return -1; }
  int			Connect(const char *filename,int mode);
  void                  Disconnect();

  void *                GetHandle();

  void                  SetBusy(bool);
  bool                  IsBusy();

private:
  SQLITE::sqlite *      m_sqlite;
  bool                  m_bBusy;
};


#endif // _SESSION_H

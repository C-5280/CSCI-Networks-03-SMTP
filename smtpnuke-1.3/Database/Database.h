// Database.h
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

#ifndef _DATABASE_H
#define _DATABASE_H

#include <vector>
#include "Query.h"

class Session;
typedef std::vector<Session *> sessions_t;

class Database
{
public:
			Database(const std::string &);
                        Database(char *filename,int mode);
  virtual               ~Database();

  Session * grabdb();
  void                            freedb(Session *odb);
  Query *       GetQuery();
  Query *       GetQuery(const std::string &);

  short                 errcode();
  void                  debug(short val);
  short                 debug();

  void                  SetEscapeChar( char );
  char                  GetEscapeChar();
  void                  SetDecimalChar( char );
  char                  GetDecimalChar();

  void			GetErrorText(char *) {}
  
private:
  char                  *filename;
  int                   mode;
  sessions_t            m_sessions;
  short                 errc;
  short                 _debug;
  char                  m_escape;
  char                  m_decimal;
};


#endif // _DATABASE_H

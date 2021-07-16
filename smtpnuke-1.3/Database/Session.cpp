// Session.cpp
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
namespace SQLITE {
#include <sqlite/sqlite.h>
}
// STL include begin
#ifdef WIN32
#pragma warning(push, 3)
#endif
//
#include <vector>
#include <string>
//
#ifdef WIN32
#pragma warning(pop)
#endif
// STL include end
using std::string;

#include "Session.h"


Session::Session()
:m_sqlite(NULL)
,m_bBusy(false)
{
}


Session::~Session()
{
  Disconnect();
}


int Session::Connect(const char *filename,int mode)
{
  char *errmsg;
  m_sqlite = SQLITE::sqlite_open(filename,mode,&errmsg);
  return 0;
}


void Session::Disconnect()
{
  if (m_sqlite)
  {
    SQLITE::sqlite_close(m_sqlite);
    m_sqlite = NULL;
  }
}


void *Session::GetHandle()
{
  return m_sqlite;
}


void Session::SetBusy(bool b)
{
  m_bBusy = b;
}


bool Session::IsBusy()
{
  return m_bBusy;
}



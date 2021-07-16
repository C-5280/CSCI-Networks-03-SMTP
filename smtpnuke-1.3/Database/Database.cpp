// Database.cpp
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
using std::string; // fix broken include files

#include "Session.h"
#include "Database.h"


#define DEB(x) x


Database::Database(const string &f)
:filename(new char[f.size() + 1])
,mode(0)
{
	errc = 0;
	m_escape = '\\';

	strcpy(filename,f.c_str());
	
	freedb(grabdb());		// open one connection
}

Database::Database(char *f,int m)
:filename(new char[strlen(f) + 1])
,mode(m)
{
	errc = 0;
	m_escape = '\\';

	strcpy(filename,f);
	
	freedb(grabdb());		// open one connection
}

Database::~Database()
{
	sessions_t::iterator it;
	if (filename)
		delete filename;
	for (it = m_sessions.begin(); it != m_sessions.end(); it++)
	{
		Session *p = *it;
		p -> Disconnect();
	}
	while (m_sessions.size())
	{
		it = m_sessions.begin();
		if ((*it) -> IsBusy())
		{
			fprintf(stderr,"destroying Database object before Connect object(s)\n");
		}
		delete *it;
		m_sessions.erase(it);
	}
}

Session *Database::grabdb()
{
	sessions_t::iterator it;
	for (it = m_sessions.begin(); it != m_sessions.end(); it++)
	{
		if (!(*it) -> IsBusy())
		{
			(*it) -> SetBusy(true);
			return *it;
		}
	}
	Session *tmp = new Session();
	m_sessions.insert(m_sessions.end(), tmp);
DEB(	printf("number of Database sessions: %d\n",m_sessions.size());)
	tmp -> Connect(filename, mode);
	tmp -> SetBusy(true);
	return tmp;
}

void Database::freedb(Session *odb)
{
	odb -> SetBusy(false);
}

short Database::errcode()
{
	return errc;
}

void Database::debug(short val)
{
	_debug = val;
}

short Database::debug()
{
	return _debug;
}


Query *Database::GetQuery()
{
	return new Query( this );
}


Query *Database::GetQuery(const string &sql)
{
	return new Query( this, sql.c_str() );
}


void Database::SetEscapeChar(char ch)
{
	m_escape = ch;
}

char Database::GetEscapeChar()
{
	return m_decimal;
}


void Database::SetDecimalChar(char ch)
{
	m_decimal = ch;
}

char Database::GetDecimalChar()
{
	return m_decimal;
}



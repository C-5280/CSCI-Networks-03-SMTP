// NukeHandler.cpp
/*
Copyright (C) 2004  Anders Hedstrom

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

#ifdef _WIN32
#pragma warning(disable:4786)
#endif
//#include <stdio.h>

#include "SMTPSocket.h"
#include "FwdSocket.h"
#include "NukeHandler.h"
#ifdef _WIN32
//#define random rand
#endif

#define DEB(x)


NukeHandler::NukeHandler()
:SocketHandler()
,m_db(NULL)
{
	InitDB();
}


NukeHandler::~NukeHandler()
{
}


void NukeHandler::ViewSockets()
{
	for (socket_m::iterator it = m_sockets.begin(); it != m_sockets.end(); it++)
	{
		SOCKET s = (*it).first;
		Socket *p = (*it).second;
		if (dynamic_cast<SMTPSocket *>(p))
		{
			printf("%4d: SMTPSocket\n",s);
		}
		else
		if (dynamic_cast<FwdSocket *>(p))
		{
			printf("%4d: FwdSocket\n",s);
		}
	}
}


void NukeHandler::InitDB()
{
	FILE *fil = fopen("nuke.db","rb");
	bool exists = false;
	if (fil)
	{
		exists = true;
		fclose(fil);
	}
	m_db = new Database("nuke.db");
	if (!exists)
	{
		Query q(m_db);
		q.execute("create table sender ("
			   "num integer primary key,"
			   "email string,"
			   "status string,"
			   "forward string"
			  ")");
		q.execute("create table recipient ("
			   "num integer primary key,"
			   "email string,"
			   "status string,"
			   "forward string"
			  ")");
		q.execute("create table mailinst ("
			   "id integer primary key,"
			   "host string,"
			   "mail string,"
			   "rcpt string,"
			   "h_from string,"
			   "h_to string,"
			   "h_subject string,"
			   "host_spam string,"
			   "mail_spam string,"
			   "from_spam string,"
			   "subject_spam string"
			  ")");
	}
}



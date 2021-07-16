// FwdSocket.cpp
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
//#include <assert.h>
//#include <stdio.h>
#ifdef _WIN32
#define strcasecmp stricmp
#endif

#include "socket_include.h"
#include "ISocketHandler.h"
#include "Parse.h"
#include "SMTPSocket.h"
#include "FwdSocket.h"

/*
#ifdef _DEBUG
#define DEB(x) x
#else
#define DEB(x)
#endif
*/
/*
#define DEB(x) { \
	FILE *fil = fopen("C:\\deb.log","at"); \
	x; \
	fclose(fil); \
}
*/
#define DEB(x)


FwdSocket::FwdSocket(ISocketHandler& h)
:TcpSocket(h)
,m_remote(NULL)
{
	SetLineProtocol();
}


FwdSocket::~FwdSocket()
{
	if (m_remote && Handler().Valid(m_remote) )
	{
//		m_remote -> SetCloseAndDelete();
	}
}


void FwdSocket::OnLine(const std::string& line)
{
	Parse pa(line,"-");
	long status = pa.getvalue();
	if (status == 550)
	{
		if (m_remote && Handler().Valid(m_remote) )
		{
			m_remote -> SetFake();
		}
		m_remote -> Send("250 Ok\r\n");
		SetCloseAndDelete();
	}
	else
	if (status != 250)
	{
		printf("<%s\n",line.c_str());
	}
	if (m_remote && Handler().Valid(m_remote) )
	{
		m_remote -> Send(line + "\r\n");
	}
}


void FwdSocket::OnDelete()
{
	if (m_remote && Handler().Valid(m_remote) )
	{
//		m_remote -> SetCloseAndDelete();
	}
}



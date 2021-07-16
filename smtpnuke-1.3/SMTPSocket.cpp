// SMTPSocket.cpp
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
#include "Parse.h"
#include "FwdSocket.h"
#include "Mailinst.h"
#include "NukeHandler.h"
#include "SMTPSocket.h"

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


SMTPSocket::SMTPSocket(ISocketHandler& h)
:TcpSocket(h)
,m_remote(NULL)
,m_cmd_data(false)
,m_cmd_quit(false)
{
	SetLineProtocol();
}


SMTPSocket::~SMTPSocket()
{
	if (m_remote && Handler().Valid(m_remote) )
	{
//		m_remote -> SetCloseAndDelete();
	}
}


void SMTPSocket::OnLine(const std::string& line)
{
	Parse pa(line,":");
	std::string cmd = pa.getword();

	if (m_cmd_data)
	{
		if (cmd == "From" && line.substr(0,5) == "From:" && !m_header_from.size())
		{
			m_header_from = pa.getrest();
			printf(" %s\n",line.c_str());
		}
		else
		if (cmd == "To" && line.substr(0,3) == "To:" && !m_header_to.size())
		{
			m_header_to = pa.getrest();
			printf(" %s\n",line.c_str());
		}
		else
		if (cmd == "Subject" && line.substr(0,8) == "Subject:" && !m_header_subject.size())
		{
			m_header_subject = pa.getrest();
			printf(" %s\n",line.c_str());
		}
		else
		if (cmd == ".")
		{
			if (m_fake)
			{
				printf(" *** FAKE <221\n");
				Send("221 2.0.0 g2.alhem.net closing connection\r\n");
			}
			m_cmd_data = false;
		}
		m_data += line + "\r\n";
	}
	else
	if (!strcasecmp(cmd.c_str(), "MAIL") )
	{
		m_cmd_mail = pa.getrest();
		printf(" %s\n",line.c_str());
		if (!strstr(m_cmd_mail.c_str(),"@") || !strstr(m_cmd_mail.c_str(),"."))
		{
			SetCloseAndDelete();
		}
	}
	else
	if (!strcasecmp(cmd.c_str(), "RCPT") )
	{
		m_cmd_rcpt = pa.getrest();
		printf(" %s\n",line.c_str());
	}
	else
	if (!strcasecmp(cmd.c_str(), "DATA") )
	{
		if (m_fake)
		{
			printf(" *** FAKE <354\n");
			Send("354 Enter mail, end with \".\" on a line by itself\r\n");
		}
		m_cmd_data = true;
	}
	else
	if (!strcasecmp(cmd.c_str(), "QUIT") )
		m_cmd_quit = true;
	else
	if (!m_cmd_data)
	{
		printf("Command: %s\n",line.c_str());
	}
	if (m_remote && Handler().Valid(m_remote) && !m_fake)
	{
		m_remote -> Send(line + "\r\n");
	}
	if (m_cmd_quit)
	{
		db::Mailinst mq(*static_cast<NukeHandler&>(Handler()).GetDatabase(),
			GetRemoteHostname(),
			get_email(m_cmd_mail),
			get_email(m_cmd_rcpt),
			get_email(m_header_from),
			get_email(m_header_to),
			m_header_subject);
		mq.save();
//		printf(" %s\n %s\n",m_cmd_mail.c_str(),m_header_from.c_str());
//		printf(" %s\n %s\n",m_cmd_rcpt.c_str(),m_header_to.c_str());
//		printf(" %s\n",m_header_subject.c_str());
//		printf("\n");
		SetCloseAndDelete();
	}
}


void SMTPSocket::OnDelete()
{
	if (m_remote && Handler().Valid(m_remote) )
	{
//		m_remote -> SetCloseAndDelete();
	}
}


void SMTPSocket::OnAccept()
{
	printf(" *** Remote Address: '%s'\n",GetRemoteHostname().c_str());
	if (Utility::isipv4(GetRemoteHostname()))
	{
		// create a 'Spammer'
		SetCloseAndDelete();
		return;
	}
	m_remote = new FwdSocket(Handler());
	m_remote -> Open("mail.alhem.net",279);
	m_remote -> SetDeleteByHandler();
	m_remote -> SetRemote(this);
	Handler().Add(m_remote);
}


std::string SMTPSocket::get_email(const std::string& em)
{
	char *tmp = new char[em.size() + 1];
	strcpy(tmp, em.c_str());
	char *p = strstr(tmp,"<");
	if (p)
	{
		char *p2 = strstr(p,">");
		if (p2)
		{
			*p2 = 0;
			std::string str = p + 1;
			delete tmp;
			return str;
		}
	}
	delete tmp;
	return em;
}



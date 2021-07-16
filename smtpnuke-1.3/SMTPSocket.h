// SMTPSocket.h
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

#ifndef _SMTPSOCKET_H
#define _SMTPSOCKET_H

#include <string>
#include "TcpSocket.h"
#include "Utility.h"
class FwdSocket;


/** This socket accepts incoming connections from a mail client,
	opens another connection to the mail server, then forwards
	traffic from the client to the mail server.
	\sa FwdSocket */
class SMTPSocket : public TcpSocket
{
public:
	SMTPSocket(ISocketHandler&);
	~SMTPSocket();

	void OnLine(const std::string&);
	void OnDelete();
	void OnAccept();

	std::string get_email(const std::string& );
	void SetFake(bool x = true) { m_fake = x; }

private:
	FwdSocket *m_remote;
	std::string m_cmd_mail;
	std::string m_cmd_rcpt;
	bool m_cmd_data;
	bool m_cmd_quit;
	std::string m_header_from;
	std::string m_header_to;
	std::string m_header_subject;
	std::string m_data;
	bool m_fake;
};




#endif // _SMTPSOCKET_H

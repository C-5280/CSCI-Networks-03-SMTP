// FwdSocket.h
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

#ifndef _FWDSOCKET_H
#define _FWDSOCKET_H

#include <string>
#include "TcpSocket.h"
#include "Utility.h"
class SMTPSocket;

/** This socket connects to the smtp server and forwards traffic
	from the smtp server back to the client. 
	\sa SMTPSocket */
class FwdSocket : public TcpSocket
{
public:
	FwdSocket(ISocketHandler&);
	~FwdSocket();

	void OnLine(const std::string&);
	void OnDelete();
	void SetRemote(SMTPSocket *p) { m_remote = p; }

private:
	SMTPSocket *m_remote;
};




#endif // _FWDSOCKET_H

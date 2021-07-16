// Sender.h
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

#ifndef _SENDER_H
#define _SENDER_H

#include <string>
#include "../Minion/Database/Database.h"
#include <Utility.h>
#include "Table.h"

namespace db {

/** Database sender table. */
class Sender : public Table
{
public:
	Sender(Database &);
	Sender(Database &,long);
	Sender(Database &,const std::string&);
	Sender(Database &,const std::string& host,
		const std::string& mail,
		const std::string& rcpt,
		const std::string& h_from,
		const std::string& h_to,
		const std::string& h_subject);
	~Sender();

	long m_id;
	std::string m_host;
	std::string m_mail;
	std::string m_rcpt;
	std::string m_h_from;
	std::string m_h_to;
	std::string m_h_subject;
	bool m_host_spam;
	bool m_mail_spam;
	bool m_from_spam;
	bool m_subject_spam;

	void save();

private:
	void Spawn(const std::string& );
};


} // namespace db

#endif // _SENDER_H
/*
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
*/

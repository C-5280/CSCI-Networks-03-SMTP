// Sender.cpp
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

//#include <stdio.h>

#include "Sender.h"


namespace db {


Sender::Sender(Database& db) : Table(db)
{
	m_new = true;
	m_host_spam = false;
	m_mail_spam = false;
	m_from_spam = false;
	m_subject_spam = false;
}


Sender::Sender(Database& db,long id) : Table(db)
{
	std::string sql;
	sql = "select * from mailinst where id=" + l2string(id);
	Spawn(sql);
}


Sender::Sender(Database& db,const std::string& sql) : Table(db)
{
	Spawn(sql);
}


Sender::Sender(Database& db,const std::string& host,
	const std::string& mail,
	const std::string& rcpt,
	const std::string& h_from,
	const std::string& h_to,
	const std::string& h_subject) : Table(db)
{
	std::string sql;
	sql = "select * from mailinst where host='" + sqlsafe(host) + "' and ";
	sql += "mail='" + sqlsafe(mail) + "' and ";
	sql += "rcpt='" + sqlsafe(rcpt) + "' and ";
	sql += "h_from='" + sqlsafe(h_from) + "' and ";
	sql += "h_to='" + sqlsafe(h_to) + "' and ";
	sql += "h_subject='" + sqlsafe(h_subject) + "'";
	Spawn(sql);
	m_host = host;
	m_mail = mail;
	m_rcpt = rcpt;
	m_h_from = h_from;
	m_h_to = h_to;
	m_h_subject = h_subject;
}


Sender::~Sender()
{
}


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
void Sender::Spawn(const std::string& sql)
{
	Query q(GetDatabase());
	m_new = true;
	m_host_spam = false;
	m_mail_spam = false;
	m_from_spam = false;
	m_subject_spam = false;
	q.get_result(sql);
	if (q.fetch_row())
	{
		m_id = q.getval();
		m_host = q.getstr();
		m_mail = q.getstr();
		m_rcpt = q.getstr();
		m_h_from = q.getstr();
		m_h_to = q.getstr();
		m_h_subject = q.getstr();
		m_host_spam = q.getval() ? true : false;
		m_mail_spam = q.getval() ? true : false;
		m_from_spam = q.getval() ? true : false;
		m_subject_spam = q.getval() ? true : false;
		m_new = false;
	}
	q.free_result();
}


void Sender::save()
{
	Query q(GetDatabase());
	std::string sql;
	if (m_new)
	{
		sql = "insert into mailinst values(NULL"
			",'" + sqlsafe(m_host) + "'"
			",'" + sqlsafe(m_mail) + "'"
			",'" + sqlsafe(m_rcpt) + "'"
			",'" + sqlsafe(m_h_from) + "'"
			",'" + sqlsafe(m_h_to) + "'"
			",'" + sqlsafe(m_h_subject) + "'";
		sql += m_host_spam ? ",1" : ",0";
		sql += m_mail_spam ? ",1" : ",0";
		sql += m_from_spam ? ",1" : ",0";
		sql += m_subject_spam ? ",1" : ",0";
		sql += ")";
printf("SQL: '%s'\n",sql.c_str());
		q.execute(sql);
		m_id = q.insert_id();
	}
	else
	{
		sql = "update mailinst set "
			"host='" + sqlsafe(m_host) + "',"
			"mail='" + sqlsafe(m_mail) + "',"
			"rcpt='" + sqlsafe(m_rcpt) + "',"
			"h_from='" + sqlsafe(m_h_from) + "',"
			"h_to='" + sqlsafe(m_h_to) + "',"
			"h_subject='" + sqlsafe(m_h_subject) + "',";
		sql += m_host_spam ? "host_spam=1," : "host_spam=0,";
		sql += m_mail_spam ? "mail_spam=1," : "mail_spam=0,";
		sql += m_from_spam ? "from_spam=1," : "from_spam=0,";
		sql += m_subject_spam ? "subject_spam=1 " : "subject_spam=0 ";
		sql = sql + "where id=" + l2string(m_id);
printf("SQL: '%s'\n",sql.c_str());
		q.execute(sql);
	}
}


} // namespace db

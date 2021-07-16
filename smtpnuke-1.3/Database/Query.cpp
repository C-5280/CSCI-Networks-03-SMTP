// Query.cpp
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
#include <string.h>
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
#include "Database.h"
#include "Query.h"


Query::Query()
:db(NULL)
,odb(NULL)
,res(NULL)
,m_tmpstrbase(NULL)
,rowcount(0)
,_debug(0)
,_syslog(0)
,m_fetchrows(0)
,m_results(0)
,m_ncolumns(0)
{
}

Query::Query(Database *dbin)
:db(dbin)
,odb(dbin -> grabdb())
,res(NULL)
,m_tmpstrbase(NULL)
,rowcount(0)
,_debug(dbin -> debug())
,_syslog(0)
,m_fetchrows(0)
,m_results(0)
,m_ncolumns(0)
{
}

Query::Query(Database *dbin,const char *sql)
:db(dbin)
,odb(dbin -> grabdb())
,res(NULL)
,m_tmpstrbase(NULL)
,rowcount(0)
,_debug(dbin -> debug())
,_syslog(0)
,m_fetchrows(0)
,m_results(0)
,m_ncolumns(0)
{
	if (!execute(sql))
		fprintf(stderr,"query failed: '%s'\n",sql);
}

// destructor

Query::~Query()
{
	TMPSTR *tmpstr;

	while (m_tmpstrbase)
	{
		tmpstr = m_tmpstrbase -> next;
		delete m_tmpstrbase->buffer;
		delete m_tmpstrbase;
		m_tmpstrbase = tmpstr;
	}
//printf("~Query()\n");
	if (res)
	{
		SQLITE::sqlite_free_table(res);
		res = NULL;
	}
	if (odb)
		db -> freedb(odb);
}


short Query::execute(const std::string &sql)
{
	return execute(sql.c_str());
}


short Query::execute(const char *sql)
{		// query, no result
	if (db && odb && res)
		fprintf(stderr,"query busy\n");
	if (db && odb && !res)
	{
		SQLITE::sqlite *db = static_cast<SQLITE::sqlite *>(odb -> GetHandle());
		char *errmsg;
		m_results = 0;
		m_fetchrows = 1;
		m_ncolumns = 0;
		if (SQLITE::sqlite_get_table(db,sql,&res,&m_results,&m_ncolumns,&errmsg) != SQLITE_OK)
		{
			fprintf(stderr,"query failed: '%s'  because: '%s'\n",sql,errmsg);
		}
		else
		{
			free_result();
			return 1;
		}
	}
	return 0;
}


// methods using db specific api calls
bool Query::get_result(const std::string &sql)
{
	return get_result(sql.c_str());
}


bool Query::get_result(const char *sql,char **attrs)
{	// query, result
	if (attrs); // UNUSED_ALWAYS
	if (db && odb && res)
		fprintf(stderr,"query busy\n");
	if (db && odb && !res)
	{
		SQLITE::sqlite *db = static_cast<SQLITE::sqlite *>(odb -> GetHandle());
		char *errmsg;
		m_results = 0;
		m_fetchrows = 1;
		m_ncolumns = 0;
		if (SQLITE::sqlite_get_table(db,sql,&res,&m_results,&m_ncolumns,&errmsg) != SQLITE_OK)
		{
			fprintf(stderr,"query failed: '%s'  because: '%s'\n",sql,errmsg);
		}
		else
		{
			return 1;
		}
	}
	return false;
}

void Query::free_result()
{
	if (db && odb && res)
	{
		SQLITE::sqlite_free_table(res);
		res = NULL;
	}
}

bool Query::fetch_row()
{
	rowcount = 0;
	if (db && odb && res)
	{
		if (m_fetchrows <= m_results)
		{
			for (int i = 0; i < m_ncolumns; i++)
				row[i] = res[m_fetchrows * m_ncolumns + i];
			m_fetchrows++;
			return true;
		}
	}
	return false;
}

long Query::insert_id()
{
	if (db && odb)
	{
		SQLITE::sqlite *db = static_cast<SQLITE::sqlite *>(odb -> GetHandle());
		return SQLITE::sqlite_last_insert_rowid(db);
	}
	else
	{
		return -1;
	}
}

long Query::num_rows()
{
	return db && odb && res ? m_results : 0;
}

// data retreival methods

char *Query::getstr(int x)
{
	if (db && odb && res && row)
		return row[x] ? row[x] : (char *)"";
	else
		return NULL;
}

char *Query::getstr()
{
	return Query::getstr(rowcount++);
}

double Query::getnum(int x)
{
	return db && odb && res && row && row[x] ? atof(row[x]) : 0;
}

long Query::getval(int x)
{
	return db && odb && res && row && row[x] ? atol(row[x]) : 0;
}

double Query::getnum()
{
	return getnum(rowcount++);
}

long Query::getval()
{
	return getval(rowcount++);
}

double Query::get_num(const std::string &sql)
{
	return get_num(sql.c_str());
}

double Query::get_num(const char *sql)
{
	double l = 0;
	if (Query::get_result(sql))
	{
		if (Query::fetch_row())
			l = Query::getnum();
		Query::free_result();
	}
	return l;
}

long Query::get_count(const std::string &sql)
{
	return get_count(sql.c_str());
}

long Query::get_count(const char *sql)
{
	long l = 0;
	if (Query::get_result(sql))
	{
		if (Query::fetch_row())
			l = Query::getval();
		Query::free_result();
	}
	return l;
}

char *Query::get_string(const std::string &sql)
{
	return get_string(sql.c_str());
}

char *Query::get_string(const char *sql)
{
static 	char slask[999];
	*slask = 0;
	if (Query::get_result(sql))
	{
		if (Query::fetch_row())
			strcpy(slask,Query::getstr());
		Query::free_result();
	}
	return slask;
}

void Query::debug(short val)
{ 
	_debug = val; 
}

void Query::syslog(short val)
{ 
	_syslog = val; 
}

//MYSQL_FIELD *	STDCALL mysql_fetch_field(MYSQL_RES *result);
/*
MYSQL_FIELD *Query::fetch_field()
{
	return db && odb && res ? mysql_fetch_field(res) : NULL;
}
*/

char *Query::fetch_fieldname()
{
	if (res)
		return res[rowcount]; // rowcount is colcount actually
	return NULL;
}


/*
void Query::safestr(char *s) 
{
	char *s2 = new char[strlen(s) * 2 + 2];
	int i,j = 0;

	*s2 = 0;
	for (i = 0; i < (int)strlen(s); i++)
		switch (s[i])
		{
		case '\'':
		case '\\':
		case 34:
			s2[j++] = '\\';
		default:
			s2[j++] = s[i];
		}
	s2[j] = 0;
	strcpy(s,s2);
	delete s2;
}


void Query::unsafestr(char *s) 
{
	int i;

	for (i = 0; i < (int)strlen(s); i++)
		if (s[i] == '\\')
			memmove(s + i,s + i + 1,strlen(s + i) + 1);
}
*/


bool Query::GotMore()
{
  return (m_fetchrows < m_results) ? true : false;
}


// ================================%METHOD%====================================
// Metod                allocsafestr
// Åtkomst              public
//                      Typ             Namn            Beskrivning / Status
//                      --------------- --------------- -----------------------           
// Parametrar           
//
// Returnerar           
//
// Beskrivning          Allocate a temp storage for conversion
// ================================%ENDMETHOD%=================================
char *Query::allocsafestr(const char *s)
{
  char                  *str = new char[strlen(s) * 2 + 4];

  safestr(s, str);
  return str;
} // allocsafestr


// ================================%METHOD%====================================
// Metod                safestr
// Åtkomst              public
//                      Typ             Namn            Beskrivning / Status
//                      --------------- --------------- -----------------------           
// Parametrar           
//
// Returnerar           
//
// Beskrivning          Convert a string to a sql safe string
// ================================%ENDMETHOD%=================================
void Query::safestr(const char *s,char *s2) 
{
  int register          i;
  int register          j = 0;

  *s2 = 0;
  for (i = 0; i < (int)strlen(s); i++)
  {
    switch (s[i])
    {
    case '\'':
    case '\\':
    case 34:
      s2[j++] = '\\';
    default:
      s2[j++] = s[i];
    }
  }
  s2[j] = 0;
} // safestr


void Query::safestr(std::string &str)
{
	std::string s2;
	for (size_t i = 0; i < str.size(); i++)
	{
		switch (str[i])
		{
		case '\'':
		case '\\':
		case 34:
			s2 += '\\';
		default:
			s2 += str[i];
		}
	}
	str = s2;
}


// ================================%METHOD%====================================
// Metod                unsafestr
// Åtkomst              public
//                      Typ             Namn            Beskrivning / Status
//                      --------------- --------------- -----------------------           
// Parametrar           
//
// Returnerar           
//
// Beskrivning          Convert an sql safe string to a normal string
// ================================%ENDMETHOD%=================================
void Query::unsafestr(char *s) 
{
  int                   i;

  for (i = 0; i < (int)strlen(s); i++)
    if (s[i] == '\\')
      memmove(s + i,s + i + 1,strlen(s + i) + 1);
} // unsafestr


// ================================%METHOD%====================================
// Metod                gettmpstr
// Åtkomst              public
//                      Typ             Namn            Beskrivning / Status
//                      --------------- --------------- -----------------------           
// Parametrar           
//
// Returnerar           
//
// Beskrivning          Allocate a temporary storage
// ================================%ENDMETHOD%=================================
char *Query::gettmpstr(int l)
{
  TMPSTR                *tmp = new TMPSTR;

  tmp->next = m_tmpstrbase;
  tmp->buffer = new char[l + 1];
  tmp->temporary = 0;
  m_tmpstrbase = tmp;
  return tmp->buffer;
} // gettmpstr


string Query::XMLSafe(const string &str)
{
	string str2 = "";
	char *s = (char *)str.c_str();

	for (int i = 0; i < (int)str.size(); i++)
	{
		switch (s[i])
		{
		case '&':
			str2 += "&amp;";
			break;
		case '<':
			str2 += "&lt;";
			break;
		case '>':
			str2 += "&gt;";
			break;
		case '"':
			str2 += "&quot;";
			break;
		case '\'':
			str2 += "&apos;";
			break;
		default:
			str2 += s[i];
		}
	}
	return str2;
}


// ================================%METHOD%====================================
// Metod                ReturnRowAsXML
// Åtkomst              public
//                      Typ             Namn            Beskrivning / Status
//                      --------------- --------------- -----------------------           
// Parametrar           
//
// Returnerar           
//
// Beskrivning          Returns current row as an xml string
// ================================%ENDMETHOD%=================================
/*
void Query::ReturnRowAsXML(string &xml,const string &strObjectName,long dwFlags)
{
  string                typ;
  string                str;
  bool                  endtag =   (dwFlags & DQ_XML_ENDTAG) ? true : false;
  bool                  cdata =    (dwFlags & DQ_XML_CDATASTRINGS) ? true : false;
  bool                  typeinfo = (dwFlags & DQ_XML_TYPEINFO) ? true : false;
  bool                  append =   (dwFlags & DQ_XML_APPEND) ? true : false;
  bool                  xmlbegin = (dwFlags & DQ_XML_BEGIN) ? true : false;
  bool                  xmlend =   (dwFlags & DQ_XML_END) ? true : false;
  char                  slask[32000];

  if (db && odb && res && row)
  {
    if (!append)
    {
      xml = "";         // det e bra
    }
    if (xmlbegin)
    {
      xml += "<xml version=\"1.0\" encoding=\"ISO-8859-1\">\n";
    }
    if (strObjectName != "")
    {
      xml += "<";
      xml += strObjectName;
      xml += ">\n";
    }

    MYSQL_FIELD *field;
    int x;
    field = fetch_field();
    x = 0;
    while (field)
    {
      if (typeinfo)
      {
      }
      typ = "";
      if (!*getstr(x))
      {
        sprintf(slask,"<%s%s/>", field -> name, typ.c_str());
        str = slask;
      } 
      else
      {
        if (cdata) // && (col->nCType == SQL_C_CHAR || col->nCType == SQL_C_TCHAR))
        {
          sprintf(slask,"<%s%s><![CDATA[%s]]></%s>\n", field -> name, typ.c_str(), XMLSafe(getstr(x)).c_str(), field -> name);
          str = slask;
        } 
        else
        {
          sprintf(slask,"<%s%s>%s</%s>\n", field -> name, typ.c_str(), XMLSafe(getstr(x)).c_str(), field -> name);
          str = slask;
        }
      }
      xml += str;

      field = fetch_field();
      x++;
    }
    if (endtag && strObjectName != "")
    {
      xml += "</";
      xml += strObjectName;
      xml += ">\n";
    }
    if (xmlend)
    {
      xml += "</xml>\n";
    }
  }
}
*/
bool Query::getcol(std::string &colname,std::string &coltype,int &width,int &dec,int &nullable)
{
	if (res)
	{
		colname = res[rowcount];
		return true;
	}
	return false;
}

bool Query::getcol(int x,std::string &colname,std::string &coltype,int &width,int &dec,int &nullable)
{
	if (res && x >= 0 && x < m_ncolumns)
	{
		colname = res[x];
		return true;
	}
	return false;
}



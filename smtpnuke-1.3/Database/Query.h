// Query.h
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

#ifndef _QUERY_H
#define _QUERY_H


#define DQ_XML_ENDTAG 1
#define DQ_XML_CDATASTRINGS 2
#define DQ_XML_TYPEINFO 4
#define DQ_XML_APPEND 8
#define DQ_XML_BEGIN 16
#define DQ_XML_END 32

class Database;
class Session;


typedef struct tmpstrstruct 
{
  struct tmpstrstruct   *next;
  char                  *buffer;
  short                 temporary;
} TMPSTR;

class Query
{
public:
                        Query();
                        Query(Database *dbin);
                        Query(Database *dbin,const char *sql);
  virtual               ~Query();

  short                 execute(const char *sql);    // query, no result
  short			execute(const std::string &);

// methods using db specific api calls

  bool                  get_result(const char *sql,char **attrs = NULL);  // query, result
  bool			get_result(const std::string &);
  void                  free_result();
  bool                  fetch_row();
  long                  insert_id();
  long                  num_rows();

// data retreival methods

  char *                get_string(const char *sql);
  double                get_num(const char *sql);
  long                  get_count(const char *sql);

  char *                get_string(const std::string &);
  double                get_num(const std::string &);
  long                  get_count(const std::string &);

  char *                getstr(int x);
  double                getnum(int x);
  long                  getval(int x);

  char *                getstr();
  double                getnum();
  long                  getval();

  void                  debug(short val);
  void                  syslog(short val);

//SQLITE_FIELD *  STDCALL mysql_fetch_field(SQLITE_RES *result);

//  SQLITE_FIELD *         fetch_field();
  char *                fetch_fieldname();

// this breaks sql2class 02-04-30
//  void                  safestr(char *s);
//  void                  unsafestr(char *s);

  bool                  GotMore();

  char *                allocsafestr(const char *s);
  void                  safestr(const char *src,char *dest);
  char *                gettmpstr(int);
  void			safestr(std::string &);

  std::string           XMLSafe(const std::string &);
  void                  ReturnRowAsXML(std::string &,const std::string & = "",long = DQ_XML_ENDTAG) {}

  int                   num_cols() { return m_ncolumns; }
  bool                  getcol(std::string &colname,std::string &coltype,int &width,int &dec,int &nullable);
  bool                  getcol(int x,std::string &colname,std::string &coltype,int &width,int &dec,int &nullable);

private:
  void                  unsafestr(char *s);
  //
  Database *db;
  Session *odb;
//  SQLITE_RES             *res;
//  SQLITE_ROW             row;
  char **               res;
  char *                row[256];
//  int                   row; // 1 .. m_ncolumns
  TMPSTR                *m_tmpstrbase;
  short                 rowcount;
  short                 _debug;
  short                 _syslog;
  int                   m_fetchrows;
  int                   m_results; // number of rows returned from query
  int                   m_ncolumns; // number of columns in result
};



#endif // _QUERY_H

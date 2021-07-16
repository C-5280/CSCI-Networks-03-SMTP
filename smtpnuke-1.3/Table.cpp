// Table.cpp
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

#include "Table.h"


namespace db {


Table::Table(Database& db) : m_db(db)
{
	m_new = true;
}


Table::~Table()
{
}


std::string Table::sqlsafe(const std::string& input)
{
	std::string str;
	for (size_t i = 0; i < input.size(); i++)
	{
		if (input[i] == '\'')
			str += "'";
		str += input[i];
	}
	return str;
}


} // namespace db

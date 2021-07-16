// Table.h
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

#ifndef _TABLE_H
#define _TABLE_H

#include <string>
#include "../Minion/Database/Database.h"
#include <Utility.h>

namespace db {

/** Database table base class. */
class Table 
{
public:
	Table(Database &);
	virtual ~Table();

	Database *GetDatabase() { return &m_db; }

	virtual void save() = 0;

protected:
	virtual void Spawn(const std::string& ) = 0;
	std::string sqlsafe(const std::string& );
	bool m_new;

private:
	//
	Database& m_db;
};

} // namespace db

#endif // _TABLE_H

// config.cpp
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

#ifdef _WIN32
#pragma warning(disable:4786)
#endif
#include <stdio.h>
#include "Parse.h"

#include "config.h"

#define C slask[strlen(slask) - 1]
#ifdef SOCKETS_NAMESPACE
using namespace SOCKETS_NAMESPACE;
#endif

	config_m config;
static	std::string config_filename;


static void read_config(const std::string& filename)
{
	FILE *fil = fopen(filename.c_str(),"rt");
	config_filename = filename;
	if (fil)
	{
		char slask[1000];
		std::string key;
		std::string value;
		fgets(slask,1000,fil);
		while (!feof(fil))
		{
			while (strlen(slask) && (C == 13 || C == 10))
				C = 0;
			Parse pa(slask);
			pa.getword(key);
			pa.getrest(value);
			config[key] = value;
			//
			fgets(slask,1000,fil);
		}
		fclose(fil);
	}
}


void write_config() //const std::string& filename)
{
	FILE *fil = fopen(config_filename.c_str(),"wt");
	if (fil)
	{
		for (config_m::iterator it = config.begin(); it != config.end(); it++)
		{
			std::string key = (*it).first;
			std::string value = (*it).second;
			if (key[0] != '.')
				fprintf(fil,"%s %s\n",key.c_str(),value.c_str());
		}
		fclose(fil);
	}
}


void parse_config(int argc,char *argv[],const std::string& filename)
{
	read_config( filename );
	for (int i = 1; i < argc; i++)
		if (argv[i][0] == '-' && i < argc - 1)
		{
			config[argv[i] + 1] = argv[i + 1];
			i++;
		}
	write_config(); // filename );
}



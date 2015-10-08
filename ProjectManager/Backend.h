/*
 * Neo Project Manager
 * Copyright (C) 2015  Yannick Pflanzer <www.neo-engine.de>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * 
 */

#ifndef BACKEND_H
#define BACKEND_H

#include <map>
#include <string>

#ifndef WIN32
#define SEPERATOR "/"
#else
#define SEPERATOR "\\"
#endif

class Backend
{
public:
	struct Project
	{
		std::string name;
		std::string path;
	};

private:
	std::map<std::string, Project> m_projectRegistry;
	void saveRegistry(const char* where);
	
public:
	Backend();

	void createProject(const char* where, const char* name);
	void importProject(const char* where, const char* name);
	void deleteProject(const char* name);
	
	void updateEngine(const char* projectName);
	void openEditor(const char* projectName);
	void publish(const char* projectName);
	
	void loadProjectRegistry();
	void saveProjectRegistry();
	
	Project getProject(const char* name) { return m_projectRegistry[name]; }
	std::map<std::string, Project>& getRegistry() { return m_projectRegistry; }
};

#endif // BACKEND_H

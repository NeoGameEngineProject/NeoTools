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

#include "Backend.h"
#include <exception>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <cstdio>
#include <fstream>
#include <sstream>

bool copyDir(const char* from, const char* to, const char* pattern = "*")
{
	QDir target(to);
	if(!target.exists() && !target.mkpath("."))
		return false;
	
	QDir src(from);
	QStringList list;
	list << pattern;
	
	for(const QFileInfo &info : src.entryInfoList(list, QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot))
	{
		QString srcPath = from;
		srcPath = srcPath + SEPERATOR + info.fileName();
		
		QString destPath = to;
		destPath = destPath + SEPERATOR + info.fileName();
		
		if(info.isDir())
		{
			if(!copyDir(srcPath.toStdString().c_str(), destPath.toStdString().c_str()))
				return false;
		}
		else if(info.isFile())
		{
			if(!QFile::copy(srcPath, destPath))
				return false;
		}
		else
		{
			qDebug() << "Can't copy item: " << info.filePath();
			return false;
		}
	}
	
	return true;
}

Backend::Backend()
{

}

void Backend::createProject(const char* where, const char* name)
{
	QDir target(where);
	if(!target.exists() && !target.mkpath("."))
		throw std::exception();
	
	QString dest = target.absolutePath() + SEPERATOR + name;
	target.mkdir(name);
	
	/*copyDir("assets", (dest + SEPERATOR + "assets").toStdString().c_str());
	copyDir(".", dest.toStdString().c_str(), "*.so*");
	
	QFile::copy("NeoPlayer", dest + SEPERATOR + "NeoPlayer");*/
	
	Project p;
	p.name = name;
	p.path = where;
	
	m_projectRegistry[name] = p;
	updateEngine(name);
	saveProjectRegistry();
}

void Backend::deleteProject(const char* name)
{
	m_projectRegistry.erase(name);
}

void Backend::updateEngine(const char* projectName)
{
	if(m_projectRegistry.find(projectName) == m_projectRegistry.end())
		return;
	
	Project p = m_projectRegistry[projectName];	
	QString dest = QString(p.path.c_str()) + SEPERATOR + projectName;

	copyDir("assets", (dest + SEPERATOR + "assets").toStdString().c_str());
	copyDir(".", dest.toStdString().c_str(), "*.so*");
	
	QFile::copy("NeoPlayer", dest + SEPERATOR + "NeoPlayer");
}

void Backend::loadProjectRegistry()
{
	QString dir = QDir::homePath() + SEPERATOR + ".neo-project-manager";
		
	std::ifstream in((dir + SEPERATOR + "registry.conf").toStdString().c_str());
	if(!in) return;
	
	std::string name, path, line;
	Project p;
	while(!in.eof())
	{
		std::getline(in, line, '\n');
		if(line.empty()) continue;
		
		std::stringstream ss(line);
		std::getline(ss, name, ':');
		std::getline(ss, path);
		
		p.name = name;
		p.path = path;
		
		m_projectRegistry[name] = p;
	}
	
	in.close();
}

void Backend::saveProjectRegistry()
{
	QDir configDir(QDir::homePath() + SEPERATOR + ".neo-project-manager");
	
	if(!configDir.exists() && !configDir.mkpath("."))
		return;
	
	std::ofstream out((configDir.absolutePath() + SEPERATOR + "registry.conf").toStdString().c_str());
	if(!out) return;
	
	std::string name, path;
	Project p;
	for(auto p : m_projectRegistry)
	{
		out << p.second.name << ":" << p.second.path << std::endl;
	}
	
	out.close();
}

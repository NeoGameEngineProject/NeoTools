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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_newprojectwizard.h"

#include <QDebug>
#include <QFileDialog>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	backend.loadProjectRegistry();
	
	auto reg = backend.getRegistry();
	for(auto p : reg)
		ui->projectList->addItem(p.second.name.c_str());
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::createProjectCallback()
{
	ProjectWizard wizard;
	wizard.show();
	
	while(wizard.isVisible()) QApplication::processEvents();
	
	QString name = wizard.ui->projectNameEdit->text();
	QString path = wizard.ui->projectDirEdit->text();
	
	if(name.isEmpty() || path.isEmpty()) return;
	
	backend.createProject(path.toStdString().c_str(), name.toStdString().c_str());
	
	ui->projectList->addItem(name);	
}

void MainWindow::deleteProjectCallback()
{
	if(ui->projectList->currentIndex().row() == -1)
		return;
		
	backend.deleteProject(ui->projectList->currentItem()->text().toStdString().c_str());
	delete ui->projectList->currentItem();
	
	backend.saveProjectRegistry();
}

void MainWindow::updateEngineCallback()
{
	if(ui->projectList->currentIndex().row() == -1)
		return;

	QString name = ui->projectList->currentItem()->text();
	backend.updateEngine(name.toStdString().c_str());
}

void MainWindow::selectedProjectChangedCallback(QString name)
{
	Backend::Project project = backend.getProject(name.toStdString().c_str());
	ui->nameEdit->setText(project.name.c_str());
	ui->projectDirEdit->setText(project.path.c_str());
	
}

void MainWindow::launchEditorCallback()
{
	if(ui->projectList->currentIndex().row() == -1)
		return;
	
	Backend::Project project = backend.getProject(ui->projectList->currentItem()->text().toStdString().c_str());
	QProcess::startDetached((project.path + SEPERATOR + "NeoPlayer").c_str());
}

ProjectWizard::ProjectWizard()
{
	ui = new Ui::NewProjectWizard;
	ui->setupUi(this);
	
	QObject::connect(ui->pushButton, SIGNAL(clicked()), 
			 this, SLOT(searchDirectoryCallback()));
}

ProjectWizard::~ProjectWizard()
{
	delete this->ui;
}

void ProjectWizard::searchDirectoryCallback()
{
	QString dir = QFileDialog::getExistingDirectory();
	
	if(!dir.isEmpty())
		ui->projectDirEdit->setText(dir);
}


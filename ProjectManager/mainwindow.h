#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWizard>
#include <QDialog>
#include <Backend.h>

namespace Ui {
class MainWindow;
class NewProjectWizard;
}

class ProjectWizard : public QWizard
{
    Q_OBJECT
   
public:
    ProjectWizard();
    ~ProjectWizard();
    
    Ui::NewProjectWizard* ui;

public slots:
	void searchDirectoryCallback();

public:
	void setUi(Ui::NewProjectWizard* ui) { this->ui = ui; }
	
};

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void createProjectCallback();
	void updateEngineCallback();
	void selectedProjectChangedCallback(QString name);
	void launchEditorCallback();
	void deleteProjectCallback();
	
private:
	Ui::MainWindow *ui;
	Backend backend;
};

#endif // MAINWINDOW_H

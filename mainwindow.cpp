#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionAbout_triggered()
{
    QString about_text = "A simple Pathfinder race and class editor for Fantasy Grounds";
    QMessageBox::about(this, "About", about_text);
}

void MainWindow::on_actionOpen_triggered()
{
    current_file_path = QFileDialog::getOpenFileName(this, "Open file");
    QFile file(current_file_path);
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, "..", "file not opened");
            return;
    }

    // TODO Add unzip module and open and parse of definition.xml
}

void MainWindow::on_actionSave_triggered()
{
    if (current_file_path == "") {
        QMessageBox::warning(this, "..", "No module currently opened");
        return;
    }
    // TODO Generate new definition.xml and zip with module
}

void MainWindow::on_actionSave_as_triggered()
{
    current_file_path = QFileDialog::getSaveFileName(this, "Open file");
    QFile file(current_file_path);
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, "..", "file not opened");
            return;
    }
    // TODO Generate new definition.xml and zip with module
}

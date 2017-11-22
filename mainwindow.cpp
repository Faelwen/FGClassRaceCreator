#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include <QtXml>


#include <JlCompress.h>

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
            QMessageBox::warning(this, "Error", "File not opened");
            return;
    }

    QStringList module_content = JlCompress::getFileList(current_file_path);

    if (module_content.isEmpty()) {
        QMessageBox::warning(this, "Error", "File doesn't appear to be a Fantasy Grounds module.");
        return;
    }

    bool contains_definition = false;
    foreach(QString item, module_content) {
        if (item == "db.xml") {
            contains_definition = true;
        }
    }
    if (!contains_definition) {
        QMessageBox::warning(this, "Error", "Module doesn't contain db.xml file.");
        return;
    }

    QStringList files_extracted = JlCompress::extractDir(current_file_path, QDir::currentPath());
    QFile definition_xml ("db.xml");
    if (!definition_xml.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "Cannot open db.xml file.");
        return;
    }

    QDomDocument *dom = new QDomDocument("db");
    if(!dom->setContent(&definition_xml)) {
        definition_xml.close();
        QMessageBox::warning(this, "Error", "Cannot parse db.xml file.");
        return;
    }

    QDomElement dom_element = dom->documentElement();
    QDomNode root = dom_element.firstChild();
    QDomNode node = root;

    // TODO parse xml and insert results in mainwindow
    while(!node.isNull()) {
       QDomElement element = node.toElement();
       if(!element.isNull())
       {

           node = node.nextSibling();
       }

    }
}

void MainWindow::on_actionSave_triggered()
{
    if (current_file_path == "") {
        QMessageBox::warning(this, "Error", "No module currently opened");
        return;
    }
    // TODO Generate new definition.xml and zip with module
}

void MainWindow::on_actionSave_as_triggered()
{
    current_file_path = QFileDialog::getSaveFileName(this, "Open file");
    QFile file(current_file_path);
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, "Error", "file not opened");
            return;
    }
    // TODO Generate new definition.xml and zip with module
}



void MainWindow::on_listWidget_races_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->listWidget_races->viewport()->mapToGlobal(pos);
    QMenu myMenu;
    myMenu.addAction("New");
    myMenu.addAction("Delete");
    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem) {
        if (selectedItem->text() == "New") {
            QListWidgetItem *new_item = new QListWidgetItem("New Race", Q_NULLPTR, QListWidgetItem::Type);
            new_item->setFlags(new_item->flags() | Qt::ItemIsEditable);
            ui->listWidget_races->addItem(new_item);

    //    ui->listWidget_races->addItem("New Race");
        }
        else if (selectedItem->text() == "Delete") {
            int row = ui->listWidget_races->currentRow();
            ui->listWidget_races->takeItem(row);
        }
    }
}

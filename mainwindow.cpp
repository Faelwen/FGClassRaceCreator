#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include <QtXml>

#include <QDebug>

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
    QString about_text = "A simple Pathfinder race and class editor for Fantasy Grounds\n Author: Faelwen";
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


// Empty QListWidget without deleting the QListWidgetItems
void MainWindow::empty_list(QListWidget * list)
{
    int max = list->count();
    for(int i = 0; i < max; i++) {
        list->takeItem(0);
    }
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
            race_descriptions.insert(new_item, "");
            QList<QListWidgetItem*> new_trait_list;
            race_traits.insert(new_item, new_trait_list);
        }
        else if (selectedItem->text() == "Delete") {
            race_descriptions.remove(ui->listWidget_races->currentItem());
            race_traits.remove(ui->listWidget_races->currentItem());
            race_alternative_traits.remove(ui->listWidget_races->currentItem());
            ui->listWidget_racial_traits->clear();
            ui->listWidget_alernative_racial_traits->clear();
            delete ui->listWidget_races->currentItem();
        }
    }
}

void MainWindow::on_textEdit_race_description_textChanged()
{
    race_descriptions[ ui->listWidget_races->currentItem()] = ui->textEdit_race_description->document()->toHtml();
}

void MainWindow::on_listWidget_races_itemSelectionChanged()
{
    if (ui->listWidget_races->currentRow() >= 0) {
        ui->textEdit_race_description->document()->setHtml(race_descriptions[ui->listWidget_races->currentItem()]);
        empty_list(ui->listWidget_racial_traits);
        empty_list(ui->listWidget_alernative_racial_traits);
        QListWidgetItem* item;
        foreach (item, race_traits[ui->listWidget_races->currentItem()]) {
            if(item) {
                ui->listWidget_racial_traits->addItem(item);
            }
        }
        foreach (item, race_alternative_traits[ui->listWidget_races->currentItem()]) {
            if(item) {
                ui->listWidget_alernative_racial_traits->addItem(item);
            }
        }
    }
    if (ui->listWidget_racial_traits->currentRow() < 0) {
        ui->textEdit_racial_trait_description->document()->clear();
    }
    if (ui->listWidget_alernative_racial_traits->currentRow() < 0) {
        ui->textEdit_alternative_trait_description->document()->clear();
    }
}

void MainWindow::on_listWidget_racial_traits_customContextMenuRequested(const QPoint &pos)
{
    if (ui->listWidget_races->currentRow() >= 0) {
        QPoint globalPos = ui->listWidget_racial_traits->viewport()->mapToGlobal(pos);
        QMenu myMenu;
        myMenu.addAction("New");
        myMenu.addAction("Delete");
        QAction* selectedItem = myMenu.exec(globalPos);
        if (selectedItem) {
            if (selectedItem->text() == "New") {
                QListWidgetItem *new_item = new QListWidgetItem("New Trait", Q_NULLPTR, QListWidgetItem::Type);
                new_item->setFlags(new_item->flags() | Qt::ItemIsEditable);
                ui->listWidget_racial_traits->addItem(new_item);
                race_traits[ui->listWidget_races->currentItem()].append(new_item);
            }
            else if (selectedItem->text() == "Delete") {
                race_traits[ui->listWidget_races->currentItem()].removeOne(ui->listWidget_racial_traits->currentItem());
                delete ui->listWidget_racial_traits->currentItem();
            }
        }
    }
}

void MainWindow::on_listWidget_alernative_racial_traits_customContextMenuRequested(const QPoint &pos)
{
    if (ui->listWidget_races->currentRow() >= 0) {
        QPoint globalPos = ui->listWidget_alernative_racial_traits->viewport()->mapToGlobal(pos);
        QMenu myMenu;
        myMenu.addAction("New");
        myMenu.addAction("Delete");
        QAction* selectedItem = myMenu.exec(globalPos);
        if (selectedItem) {
            if (selectedItem->text() == "New") {
                QListWidgetItem *new_item = new QListWidgetItem("New Alternative Trait", Q_NULLPTR, QListWidgetItem::Type);
                new_item->setFlags(new_item->flags() | Qt::ItemIsEditable);
                ui->listWidget_alernative_racial_traits->addItem(new_item);
                race_alternative_traits[ui->listWidget_races->currentItem()].append(new_item);
            }
            else if (selectedItem->text() == "Delete") {
                race_alternative_traits[ui->listWidget_races->currentItem()].removeOne(ui->listWidget_alernative_racial_traits->currentItem());
                delete ui->listWidget_alernative_racial_traits->currentItem();
            }
        }
    }
}

void MainWindow::on_textEdit_racial_trait_description_textChanged()
{
    if(ui->listWidget_races->currentRow() >= 0 && ui->listWidget_racial_traits->currentRow() >= 0) {
        race_traits_descriptions[ui->listWidget_racial_traits->currentItem()] = ui->textEdit_racial_trait_description->document()->toHtml();
    }
}

void MainWindow::on_textEdit_alternative_trait_description_textChanged()
{
    if(ui->listWidget_races->currentRow() >= 0 && ui->listWidget_alernative_racial_traits->currentRow() >= 0) {
        race_alternative_traits_descriptions[ui->listWidget_alernative_racial_traits->currentItem()] = ui->textEdit_alternative_trait_description->document()->toHtml();
    }
}

void MainWindow::on_listWidget_racial_traits_itemSelectionChanged()
{
    if (ui->listWidget_races->currentRow() >= 0 && ui->listWidget_racial_traits->currentRow() >= 0) {
        ui->textEdit_racial_trait_description->document()->setHtml(race_traits_descriptions[ui->listWidget_racial_traits->currentItem()]);
    }
}

void MainWindow::on_listWidget_alernative_racial_traits_itemSelectionChanged()
{
    if (ui->listWidget_races->currentRow() >= 0 && ui->listWidget_alernative_racial_traits->currentRow() >= 0) {
        ui->textEdit_alternative_trait_description->document()->setHtml(race_alternative_traits_descriptions[ui->listWidget_alernative_racial_traits->currentItem()]);
    }
}

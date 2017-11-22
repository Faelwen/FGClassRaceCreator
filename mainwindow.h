#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_actionAbout_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void on_listWidget_races_customContextMenuRequested(const QPoint &pos);

    void on_textEdit_race_description_textChanged();

    void on_listWidget_races_itemSelectionChanged();

private:
    Ui::MainWindow *ui;
    QString current_file_path = "";
    QHash<QListWidgetItem *, QString> race_descriptions;
};

#endif // MAINWINDOW_H

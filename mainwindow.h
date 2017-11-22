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

    void on_listWidget_racial_traits_customContextMenuRequested(const QPoint &pos);

    void on_listWidget_alernative_racial_traits_customContextMenuRequested(const QPoint &pos);

    void on_textEdit_racial_trait_description_textChanged();

    void on_textEdit_alternative_trait_description_textChanged();

    void on_listWidget_racial_traits_itemSelectionChanged();

    void on_listWidget_alernative_racial_traits_itemSelectionChanged();

private:
    Ui::MainWindow *ui;
    QString current_file_path = "";
    QHash<QListWidgetItem*, QList<QListWidgetItem*>> race_traits;
    QHash<QListWidgetItem*, QVector<QListWidgetItem*>> race_alternative_traits;
    QHash<QListWidgetItem*, QString> race_descriptions;
    QHash<QListWidgetItem*, QString> race_traits_descriptions;
    QHash<QListWidgetItem*, QString> race_alternative_traits_descriptions;
    void empty_list(QListWidget*);
};

#endif // MAINWINDOW_H

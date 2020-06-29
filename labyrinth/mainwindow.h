#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

#include "Manager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    char genRandom();
    string createRandomPW();
    void saveManager(Manager);
    void loadManager(Manager*);

private:
    Ui::MainWindow *ui;
    QPoint mLastMousePosition;
    bool mMoving;
    Manager *manager;
    int currentMasterIndex;

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
private slots:
    void removeLineEditFocus(QLineEdit* lineEdit);
    void on_inputPW_returnPressed();
    void on_createID_clicked();
    void goToNextPage();
    void goToPreviousPage();
    void on_searchPW_returnPressed();
    void on_inputID_returnPressed();
    void on_logOut_clicked();
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_searchPW_textChanged(const QString &arg1);
    void on_confirmPW_returnPressed();
    void on_registerButton_clicked();
    void on_editButton_clicked();
    void on_clipboard_clicked();
    void on_registerCancle_clicked();
    void on_registerID_returnPressed();
    void on_registerPW_returnPressed();
    void on_closeButton_clicked();
    void on_minimizeButton_clicked();
    void on_addPW_clicked();
    void on_removePW_clicked();
    void on_generatePW_clicked();
    void loadData();

    void setEditModeStyle(QLineEdit*);
    void removeEditStyle(QLineEdit*);
    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_categoryWidget_itemSelectionChanged();
    void on_checkFavorite_toggled(bool checked);
    void on_categoryCombo_currentIndexChanged(int index);
};

#endif // MAINWINDOW_H

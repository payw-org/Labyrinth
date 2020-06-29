#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMouseEvent"
#include "QPropertyAnimation"
#include "QGraphicsOpacityEffect"
#include "QClipboard"
#include "QFile"

#include "Manager.h"
#include "MasterID.h"
#include "NormalID.h"
#include "lqlistwidgetitem.h"
#include "Encryption.h"
#include "File.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

static const char alphanum[] =
"0123456789"
"!@#$%^&*"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";

int stringLength = sizeof(alphanum) - 1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(1);
    ui->infoStack->setCurrentIndex(0);

    // remove textbox blue border on macOS
    ui->inputID->setAttribute(Qt::WA_MacShowFocusRect, 0);
    ui->inputPW->setAttribute(Qt::WA_MacShowFocusRect, 0);
    ui->registerID->setAttribute(Qt::WA_MacShowFocusRect, 0);
    ui->registerPW->setAttribute(Qt::WA_MacShowFocusRect, 0);
    ui->confirmPW->setAttribute(Qt::WA_MacShowFocusRect, 0);
    ui->searchPW->setAttribute(Qt::WA_MacShowFocusRect, 0);
    ui->listWidget->setAttribute(Qt::WA_MacShowFocusRect, 0);
    ui->categoryWidget->setAttribute(Qt::WA_MacShowFocusRect, 0);
    ui->urlArea->setAttribute(Qt::WA_MacShowFocusRect, 0);
    ui->pwArea->setAttribute(Qt::WA_MacShowFocusRect, 0);
    ui->idArea->setAttribute(Qt::WA_MacShowFocusRect, 0);

    ui->registerContainer->setGeometry(ui->registerContainer->x()-600, ui->registerContainer->y(),
                                       ui->registerContainer->width(), ui->registerContainer->height());

    //hide objects
//    ui->loginWarning->hide();
    ui->createNewText->hide();
    ui->generatePW->hide();

    // gui initialize
    removeEditStyle(ui->urlArea);
    removeEditStyle(ui->pwArea);
    removeEditStyle(ui->idArea);

    // create objects
    manager = new Manager();
    loadManager(manager);

    // select All category first
    ui->categoryWidget->setCurrentRow(1);

    srand(time(0));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveManager(Manager manager)
{
    Encryption encrypt;
    string stringData;
    stringData = manager.dataToString();
    encrypt.setDeString(stringData);
    encrypt.encrypt("monkey");
    File file;
    file.setText(encrypt.getEnString());
    file.writeFile();
//    std::cout<<"saved"<<std::endl;

}

void MainWindow::loadManager(Manager *manager)
{
    Encryption encrypt;
    string stringData;
    File file;
    file.readFile();
    encrypt.setEnString(file.getText());
    encrypt.decrypt("monkey");
    manager->stringToData(encrypt.getDeString());
}

void MainWindow::removeLineEditFocus(QLineEdit* lineEdit)
{
    lineEdit->clearFocus();
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        mMoving = true;
        mLastMousePosition = event->pos();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if( event->buttons().testFlag(Qt::LeftButton) && mMoving)
    {
        this->move(this->pos() + (event->pos() - mLastMousePosition));
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        mMoving = false;
    }
}

void MainWindow::loadData()
{
    for (int i=0; i<manager->getMasterVector()->at(currentMasterIndex).getNormalID()->size(); i++)
    {
    LQListWidgetItem *item = new LQListWidgetItem(manager->getMasterVector()->at(currentMasterIndex).getNormalID()->at(i));
    item->setText(QString::fromStdString( manager->getMasterVector()->at(currentMasterIndex).getNormalID()->at(i).getUrl()));
    item->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter );
    ui->listWidget->addItem(item);
    }
}

void MainWindow::on_inputPW_returnPressed()
{
    MasterID master(ui->inputID->text().toUtf8().constData(), ui->inputPW->text().toUtf8().constData());

    if (manager->findMasterID(master) == true) {
        currentMasterIndex = manager->getIndexOfFoundMasterID(master);
//        currentMaster = &(manager->getMasterVector().at(currentMasterIndex));
        loadData();

        ui->inputPW->clearFocus();
        ui->searchPW->clearFocus();
        ui->loginWarning->clear();

    QWidget *logo = ui->logo;
    QPropertyAnimation *animation = new QPropertyAnimation(logo, "pos");
    animation->setDuration(800);
    animation->setEasingCurve(QEasingCurve::InOutExpo);
    animation->setStartValue(logo->pos());
    animation->setEndValue(QPoint(logo->x(), logo->y() + 75));

    QWidget *inputContainer = ui->inputContainer;
    QPropertyAnimation *animation2 = new QPropertyAnimation(inputContainer, "pos");
    animation2->setDuration(800);
    animation2->setEasingCurve(QEasingCurve::InOutExpo);
    animation2->setStartValue(QPoint(inputContainer->x(), inputContainer->y()));
    animation2->setEndValue(QPoint(inputContainer->x(), inputContainer->y() + 500));


    QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect();
    QWidget *logo2 = ui->logo;
    QPropertyAnimation *fadeOut = new QPropertyAnimation(opacity, "opacity");
    logo2->setGraphicsEffect(opacity);
    fadeOut->setEasingCurve(QEasingCurve::InOutExpo);
    fadeOut->setDuration(1800);
    fadeOut->setStartValue(1);
    fadeOut->setEndValue(0);

    fadeOut->start();
    fadeOut->pause();
    animation->start();
    animation2->start();
    connect(animation, SIGNAL(finished()), fadeOut, SLOT(resume()));
    connect(fadeOut, SIGNAL(finished()), this, SLOT(goToNextPage()));

    QGraphicsOpacityEffect *opacity2 = new QGraphicsOpacityEffect();
    QPropertyAnimation *fadeIn = new QPropertyAnimation(opacity2, "opacity");
    ui->infoPage->setGraphicsEffect(opacity2);
    fadeIn->setEasingCurve(QEasingCurve::InOutExpo);
    fadeIn->setDuration(500);
    fadeIn->setStartValue(0);
    fadeIn->setEndValue(1);
    fadeIn->start();
    fadeIn->pause();

    connect(fadeOut, SIGNAL(finished()), fadeIn, SLOT(resume()));

    // load normalID data from masterID

    }

    else {
        ui->loginWarning->setText("Invalid id or wrong password.");
        QPropertyAnimation *shiver = new QPropertyAnimation(ui->inputContainer, "pos");
        shiver->setDuration(500);
        shiver->setEasingCurve(QEasingCurve::OutElastic);
        shiver->setStartValue(QPoint(0, ui->inputContainer->y()));
        shiver->setKeyValueAt(0, QPoint(50, ui->inputContainer->y()));
        shiver->setEndValue(QPoint(0, ui->inputContainer->y()));
        shiver->start();
    }
}

void MainWindow::on_createID_clicked()
{

    QWidget *logo = ui->logo;
    QPropertyAnimation *animation = new QPropertyAnimation(logo, "pos");
    animation->setDuration(800);
    animation->setStartValue(logo->pos());
    animation->setEndValue(QPoint(logo->x()+600, logo->y()));
    animation->setEasingCurve(QEasingCurve::InOutExpo);
    animation->start();

    QPropertyAnimation *animation2 = new QPropertyAnimation(ui->inputContainer, "pos");
    animation2->setDuration(800);
    animation2->setStartValue(ui->inputContainer->pos());
    animation2->setEndValue(QPoint(ui->inputContainer->x()+600, ui->inputContainer->y()));
    animation2->setEasingCurve(QEasingCurve::InOutExpo);
    animation2->start();

    connect(animation, SIGNAL(finished()), this, SLOT(goToPreviousPage()));


    QWidget *stacked = ui->registerContainer;
    QPropertyAnimation *animation3 = new QPropertyAnimation(stacked, "pos");
    animation3->setDuration(800);
    animation3->setStartValue(stacked->pos());
    animation3->setEndValue(QPoint(stacked->x()+600, stacked->pos().y()));
    animation3->setEasingCurve(QEasingCurve::InOutExpo);

    animation3->start();animation3->pause();
    connect(animation, SIGNAL(finished()), animation3, SLOT(resume()));

}

void MainWindow::goToNextPage()
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+1);
}

void MainWindow::goToPreviousPage()
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()-1);
}

void MainWindow::on_searchPW_returnPressed()
{
    ui->searchPW->clearFocus();
}

void MainWindow::on_inputID_returnPressed()
{
    ui->inputPW->setFocus();
}

void MainWindow::on_logOut_clicked()
{
    ui->registerID->clear();
    ui->registerPW->clear();
    ui->confirmPW->clear();
    ui->infoStack->setCurrentIndex(0);
    ui->listWidget->clear();
    ui->categoryWidget->setCurrentRow(1);


    /* animation */
    QWidget *logo = ui->logo;
    QPropertyAnimation *animation = new QPropertyAnimation(logo, "pos");
    animation->setDuration(800);
    animation->setEasingCurve(QEasingCurve::InOutExpo);
    animation->setStartValue(logo->pos());
    animation->setEndValue(QPoint(logo->x(), logo->y() - 75));

    QWidget *inputContainer = ui->inputContainer;
    QPropertyAnimation *animation2 = new QPropertyAnimation(inputContainer, "pos");
    animation2->setDuration(800);
    animation2->setEasingCurve(QEasingCurve::InOutExpo);
    animation2->setStartValue(QPoint(inputContainer->x(), inputContainer->y()));
    animation2->setEndValue(QPoint(inputContainer->x(), inputContainer->y() - 500));


    QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect();
    QWidget *logo2 = ui->logo;
    QPropertyAnimation *fadeOut = new QPropertyAnimation(opacity, "opacity");
    logo2->setGraphicsEffect(opacity);
    fadeOut->setEasingCurve(QEasingCurve::InOutExpo);
    fadeOut->setDuration(1500);
    fadeOut->setStartValue(0);
    fadeOut->setEndValue(1);

    QGraphicsOpacityEffect *opacity2 = new QGraphicsOpacityEffect();
    QPropertyAnimation *fadeIn = new QPropertyAnimation(opacity2, "opacity");
    ui->infoPage->setGraphicsEffect(opacity2);
    fadeIn->setEasingCurve(QEasingCurve::InOutExpo);
    fadeIn->setDuration(300);
    fadeIn->setStartValue(1);
    fadeIn->setEndValue(0);
    fadeIn->start();
    connect(fadeIn, SIGNAL(finished()), this, SLOT(goToPreviousPage()));

    animation->start();
    animation->pause();
    animation2->start();
    animation2->pause();
    fadeOut->start();
    fadeOut->pause();

    connect(fadeIn, SIGNAL(finished()), fadeOut, SLOT(resume()));
    connect(fadeOut, SIGNAL(finished()), animation, SLOT(resume()));
    connect(fadeOut, SIGNAL(finished()), animation2, SLOT(resume()));
    /* animation */

    ui->inputID->clear();
    ui->inputPW->clear();
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    ui->infoStack->setCurrentIndex(1);
//    QListWidgetItem *oitem = ui->listWidget->selectedItems().at(0);
    LQListWidgetItem *litem = dynamic_cast<LQListWidgetItem*>(item);
    ui->urlArea->setText(QString::fromStdString(litem->getNormal()->getUrl()));
    ui->idArea->setText(QString::fromStdString(litem->getNormal()->getID()));
    ui->pwArea->setText(QString::fromStdString(litem->getNormal()->getPW()));
    ui->categoryCombo->setCurrentIndex(litem->getNormal()->getCategory());
    ui->checkFavorite->setChecked(litem->getNormal()->getBookmark());
}

void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (current == NULL){
        ui->infoStack->setCurrentIndex(0);
    }
    else {
    ui->infoStack->setCurrentIndex(1);
//    QListWidgetItem *oitem = ui->listWidget->selectedItems().at(0);
    LQListWidgetItem *litem = dynamic_cast<LQListWidgetItem*>(current);
    ui->urlArea->setText(QString::fromStdString(litem->getNormal()->getUrl()));
    ui->idArea->setText(QString::fromStdString(litem->getNormal()->getID()));
    ui->pwArea->setText(QString::fromStdString(litem->getNormal()->getPW()));
    }
}

void MainWindow::on_searchPW_textChanged(const QString &arg1)
{   

}

void MainWindow::on_confirmPW_returnPressed()
{
    on_registerButton_clicked();
}

void MainWindow::on_registerButton_clicked()
{
    if (ui->registerPW->text() == ui->confirmPW->text()){
    ui->confirmPW->clearFocus();

    MasterID *newMaster = new MasterID(ui->registerID->text().toUtf8().constData(), ui->registerPW->text().toUtf8().constData());
    manager->addMasterID(*newMaster);

//    saveManager(*manager);

    // if verified

    QWidget *widget = ui->registerContainer;
    QPropertyAnimation *animation = new QPropertyAnimation(widget, "pos");
    animation->setDuration(800);
    animation->setStartValue(widget->pos());
    animation->setEndValue(QPoint(widget->x()-600, widget->y()));
    animation->setEasingCurve(QEasingCurve::InOutExpo);
    animation->start();

    QWidget *logo = ui->logo;
    QPropertyAnimation *animation2 = new QPropertyAnimation(logo, "pos");
    animation2->setDuration(800);
    animation2->setStartValue(logo->pos());
    animation2->setEndValue(QPoint(logo->x()-600, logo->y()));
    animation2->setEasingCurve(QEasingCurve::InOutExpo);
    animation2->start();
    animation2->pause();

    QPropertyAnimation *animation3 = new QPropertyAnimation(ui->inputContainer, "pos");
    animation3->setDuration(800);
    animation3->setStartValue(ui->inputContainer->pos());
    animation3->setEndValue(QPoint(ui->inputContainer->x()-600, ui->inputContainer->y()));
    animation3->setEasingCurve(QEasingCurve::InOutExpo);
    animation3->start();
    animation3->pause();

    connect(animation, SIGNAL(finished()), this, SLOT(goToNextPage()));
    connect(animation, SIGNAL(finished()), animation2, SLOT(resume()));
    connect(animation, SIGNAL(finished()), animation3, SLOT(resume()));
    }
}

void MainWindow::on_editButton_clicked()
{
    if (!(ui->urlArea->isEnabled())) {

        // Edit start
        setEditModeStyle(ui->urlArea);
        setEditModeStyle(ui->pwArea);
        setEditModeStyle(ui->idArea);
        ui->clipboard->hide();
        ui->createNewText->setText("Edit mode");
        ui->createNewText->show();
        ui->checkFavorite->hide();
        ui->categoryCombo->hide();

        ui->generatePW->show();
            ui->editButton->setText("Done");
            ui->urlArea->setEnabled(true);
            ui->pwArea->setEnabled(true);
            ui->idArea->setEnabled(true);
            ui->pwArea->setEchoMode(QLineEdit::Normal);
            ui->urlArea->setFocus();
    }
    else {

        removeEditStyle(ui->urlArea);
        removeEditStyle(ui->pwArea);
        removeEditStyle(ui->idArea);

        ui->generatePW->hide();
        ui->editButton->setText("Edit");
        ui->urlArea->setEnabled(false);
        ui->pwArea->setEnabled(false);
        ui->idArea->setEnabled(false);
        ui->pwArea->setEchoMode(QLineEdit::Password);
        ui->createNewText->hide();
        ui->clipboard->show();
        ui->checkFavorite->show();
        ui->categoryCombo->show();
//        ui->categoryCombo->

        // Add finished
        if (ui->createNewText->text() == QString("Create a new item"))
        {
            // need serialization



            int category = ui->categoryCombo->currentIndex();
            bool bookmarked = ui->checkFavorite->isChecked();
//            std::cout << bookmarked << std::endl;

            // create a new item
            NormalID *newNormal = new NormalID(ui->urlArea->text().toUtf8().constData(), ui->idArea->text().toUtf8().constData(),
                                            ui->pwArea->text().toUtf8().constData(), category, bookmarked);

            // create a new list item
            LQListWidgetItem *lqitem = new LQListWidgetItem(*newNormal);

            manager->getMasterVector()->at(currentMasterIndex).addID(*newNormal);

            lqitem->setText(ui->urlArea->text().toUtf8().constData());
            lqitem->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter );
            ui->listWidget->addItem(lqitem);
            lqitem->setSelected(true);
            ui->listWidget->setEnabled(true);
            ui->searchPW->clearFocus();

            saveManager(*manager);
        }

        // edit finished
        else {
            // need serialization
//            std::cout<<"edit finished"<<std::endl;
            LQListWidgetItem *item = dynamic_cast<LQListWidgetItem*>(ui->listWidget->selectedItems().at(0));
            int currentNormalIndex = manager->getMasterVector()->at(currentMasterIndex).getIndexOfFoundNormalID(*item->getNormal());
//            manager->getMasterVector()->at(currentMasterIndex).deleteID(*item->getNormal());
            item->getNormal()->setURL(ui->urlArea->text().toUtf8().constData());
            item->getNormal()->setNormalID(ui->idArea->text().toUtf8().constData());
            item->getNormal()->setNormalPW(ui->pwArea->text().toUtf8().constData());
//            manager->getMasterVector()->at(currentMasterIndex).addID(*item->getNormal());
            manager->getMasterVector()->at(currentMasterIndex).getNormalID()->at(currentNormalIndex).setURL(ui->urlArea->text().toUtf8().constData());
            manager->getMasterVector()->at(currentMasterIndex).getNormalID()->at(currentNormalIndex).setNormalID(ui->idArea->text().toUtf8().constData());
            manager->getMasterVector()->at(currentMasterIndex).getNormalID()->at(currentNormalIndex).setNormalPW(ui->pwArea->text().toUtf8().constData());

            saveManager(*manager);
        }
    }
}

void MainWindow::on_clipboard_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->pwArea->text());
}

void MainWindow::on_registerCancle_clicked()
{
    /* animation */
    QWidget *widget = ui->registerContainer;
    QPropertyAnimation *animation = new QPropertyAnimation(widget, "pos");
    animation->setDuration(800);
    animation->setStartValue(widget->pos());
    animation->setEndValue(QPoint(widget->x()-600, widget->y()));
    animation->setEasingCurve(QEasingCurve::InOutExpo);
    animation->start();

    QWidget *logo = ui->logo;
    QPropertyAnimation *animation2 = new QPropertyAnimation(logo, "pos");
    animation2->setDuration(800);
    animation2->setStartValue(logo->pos());
    animation2->setEndValue(QPoint(logo->x()-600, logo->y()));
    animation2->setEasingCurve(QEasingCurve::InOutExpo);
    animation2->start();
    animation2->pause();

    QPropertyAnimation *animation3 = new QPropertyAnimation(ui->inputContainer, "pos");
    animation3->setDuration(800);
    animation3->setStartValue(ui->inputContainer->pos());
    animation3->setEndValue(QPoint(ui->inputContainer->x()-600, ui->inputContainer->y()));
    animation3->setEasingCurve(QEasingCurve::InOutExpo);
    animation3->start();
    animation3->pause();

    connect(animation, SIGNAL(finished()), this, SLOT(goToNextPage()));
    connect(animation, SIGNAL(finished()), animation2, SLOT(resume()));
    connect(animation, SIGNAL(finished()), animation3, SLOT(resume()));
    /* animation */
}

void MainWindow::on_registerID_returnPressed()
{
    ui->registerPW->setFocus();
}

void MainWindow::on_registerPW_returnPressed()
{
    ui->confirmPW->setFocus();
}

void MainWindow::on_closeButton_clicked()
{
    close();
}

void MainWindow::on_minimizeButton_clicked()
{
    this->setWindowState(Qt::WindowMinimized);
}

void MainWindow::on_addPW_clicked()
{
    setEditModeStyle(ui->urlArea);
    setEditModeStyle(ui->idArea);
    setEditModeStyle(ui->pwArea);

    ui->editButton->setText("Add");
    ui->urlArea->setEnabled(true);
    ui->idArea->setEnabled(true);
    ui->pwArea->setEnabled(true);
    ui->urlArea->clear();
    ui->idArea->clear();
    ui->pwArea->clear();
    ui->pwArea->setEchoMode(QLineEdit::Normal);
    ui->urlArea->setFocus();
    ui->clipboard->hide();
    ui->createNewText->setText("Create a new item");
    ui->createNewText->show();
    ui->listWidget->setEnabled(false);
    ui->generatePW->show();
    ui->checkFavorite->setChecked(false);
    ui->checkFavorite->hide();
    ui->categoryCombo->hide();
    ui->categoryCombo->setCurrentIndex(0);


    ui->listWidget->clearSelection();

    ui->infoStack->setCurrentIndex(1);
}

void MainWindow::on_removePW_clicked()
{
    if (ui->listWidget->count() !=0){
    LQListWidgetItem *item = dynamic_cast<LQListWidgetItem*>(ui->listWidget->selectedItems().at(0));
    manager->getMasterVector()->at(currentMasterIndex).deleteID(*(item->getNormal()));
    ui->listWidget->takeItem(ui->listWidget->row(item));
    ui->infoStack->setCurrentIndex(0);
    ui->listWidget->clearSelection();
    }
}

void MainWindow::on_generatePW_clicked()
{
    ui->pwArea->setText(QString::fromStdString(createRandomPW()));
}

void MainWindow::setEditModeStyle(QLineEdit *edit)
{
    edit->setStyleSheet("QLineEdit {"
                        "color:#ED2771;"
                        "font-size:20px;"
                        "border:1px solid #DADFE1;"
                        "margin-bottom:10px;"
                        "border-radius:5px;"
                        "background-color:#ecf0f1;}"
                        "QLineEdit:focus {"
                        "color:#34495E;"
                        "background-color:#E4F1FE;}");
}

void MainWindow::removeEditStyle(QLineEdit *edit)
{
    edit->setStyleSheet("QLineEdit {"
                        "color:#ED2771;"
                        "font-size:20px;"
                        "border:none;"
                        "margin-bottom:10px;"
                        "background-color:transparent;}"
                        "QLineEdit:focus {"
                        "color:white;"
                        "background-color:#34495e;}");
}

void MainWindow::on_categoryWidget_itemSelectionChanged()
{
    QListWidgetItem *item = ui->categoryWidget->selectedItems().at(0);
    if (item->text() == "All")
    {
//        std::cout<<"here"<<std::endl;
        for (int i=0; i<ui->listWidget->count(); i++){
        ui->listWidget->setRowHidden(i, false);
//        std::cout<<"for loop"<<std::endl;
        }
    }

    else if (item->text() == "Favorites")
    {
        for (int i=0; i<ui->listWidget->count();i++)
        {
            if (dynamic_cast<LQListWidgetItem*>(ui->listWidget->item(i))->getNormal()->getBookmark())
                ui->listWidget->setRowHidden(i, false);
            else
                ui->listWidget->setRowHidden(i, true);
        }
    }

    else if (item->text() == "Portal")
    {
        for (int i=0; i<ui->listWidget->count();i++)
        {
            if (dynamic_cast<LQListWidgetItem*>(ui->listWidget->item(i))->getNormal()->getCategory()==1)
                ui->listWidget->setRowHidden(i, false);
            else
                ui->listWidget->setRowHidden(i, true);
        }
    }

    else if (item->text() == "SNS")
    {
        for (int i=0; i<ui->listWidget->count();i++)
        {
            if (dynamic_cast<LQListWidgetItem*>(ui->listWidget->item(i))->getNormal()->getCategory()==2)
                ui->listWidget->setRowHidden(i, false);
            else
                ui->listWidget->setRowHidden(i, true);
        }
    }

    else if (item->text() == "Shopping")
    {
        for (int i=0; i<ui->listWidget->count();i++)
        {
            if (dynamic_cast<LQListWidgetItem*>(ui->listWidget->item(i))->getNormal()->getCategory()==3)
                ui->listWidget->setRowHidden(i, false);
            else
                ui->listWidget->setRowHidden(i, true);
        }
    }

    else if (item->text() == "Bank")
    {
        for (int i=0; i<ui->listWidget->count();i++)
        {
            if (dynamic_cast<LQListWidgetItem*>(ui->listWidget->item(i))->getNormal()->getCategory()==4)
                ui->listWidget->setRowHidden(i, false);
            else
                ui->listWidget->setRowHidden(i, true);
        }
    }

    else if (item->text() == "School")
    {
        for (int i=0; i<ui->listWidget->count();i++)
        {
            if (dynamic_cast<LQListWidgetItem*>(ui->listWidget->item(i))->getNormal()->getCategory()==5)
                ui->listWidget->setRowHidden(i, false);
            else
                ui->listWidget->setRowHidden(i, true);
        }
    }

    else if (item->text() == "Game")
    {
        for (int i=0; i<ui->listWidget->count();i++)
        {
            if (dynamic_cast<LQListWidgetItem*>(ui->listWidget->item(i))->getNormal()->getCategory()==6)
                ui->listWidget->setRowHidden(i, false);
            else
                ui->listWidget->setRowHidden(i, true);
        }
    }

    else if (item->text() == "Music")
    {
        for (int i=0; i<ui->listWidget->count();i++)
        {
            if (dynamic_cast<LQListWidgetItem*>(ui->listWidget->item(i))->getNormal()->getCategory()==7)
                ui->listWidget->setRowHidden(i, false);
            else
                ui->listWidget->setRowHidden(i, true);
        }
    }

    else if (item->text() == "Movie")
    {
        for (int i=0; i<ui->listWidget->count();i++)
        {
            if (dynamic_cast<LQListWidgetItem*>(ui->listWidget->item(i))->getNormal()->getCategory()==8)
                ui->listWidget->setRowHidden(i, false);
            else
                ui->listWidget->setRowHidden(i, true);
        }
    }
}

void MainWindow::on_checkFavorite_toggled(bool checked)
{
    LQListWidgetItem *item = dynamic_cast<LQListWidgetItem*>(ui->listWidget->selectedItems().at(0));
    item->getNormal()->setBookmark(checked);
    int currentNormalIndex = manager->getMasterVector()->at(currentMasterIndex).getIndexOfFoundNormalID(*item->getNormal());
    manager->getMasterVector()->at(currentMasterIndex).getNormalID()->at(currentNormalIndex).setBookmark(checked);

    //update bookmarked tab immediately
    on_categoryWidget_itemSelectionChanged();
    saveManager(*manager);
}

void MainWindow::on_categoryCombo_currentIndexChanged(int index)
{
    if (ui->listWidget->count()!=0)
    {
    LQListWidgetItem *item = dynamic_cast<LQListWidgetItem*>(ui->listWidget->selectedItems().at(0));
    item->getNormal()->setCategory(index);
    int currentNormalIndex = manager->getMasterVector()->at(currentMasterIndex).getIndexOfFoundNormalID(*item->getNormal());
    manager->getMasterVector()->at(currentMasterIndex).getNormalID()->at(currentNormalIndex).setCategory(index);

    //update bookmarked tab immediately
    on_categoryWidget_itemSelectionChanged();
    saveManager(*manager);
    }
}

char MainWindow::genRandom()
{

   return alphanum[rand() % stringLength];
}

string MainWindow::createRandomPW()
{
   srand(time(0));
   std::string Str;
   for (unsigned int i = 0; i < 15; ++i)
   {
      Str += genRandom();

   }

   return Str;

}

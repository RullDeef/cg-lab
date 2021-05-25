#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

#include <QPushButton>
#include <vector>
#include <memory>

#include "tabs/interactivetabwidget.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

    void addInteractiveTab(ui::InteractiveTabWidget* tab);

private:
    QPushButton* appendPushButton(const QString& string);

private:
    Ui::MainWindowClass ui;
    std::vector<ui::InteractiveTabWidget*> tabWidgets;

    std::vector<QPushButton*> pushButtons;
};

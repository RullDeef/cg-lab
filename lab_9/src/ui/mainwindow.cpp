#include "mainwindow.hpp"

#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

MainWindow::~MainWindow()
{
    for (const auto& button : pushButtons)
        delete button;
}

void MainWindow::addInteractiveTab(ui::InteractiveTabWidget* tab)
{
    QStackedWidget& stack = *ui.stackedWidget;
    stack.addWidget(tab);

    QPushButton* button = appendPushButton(tab->getName());

    connect(button, &QPushButton::clicked, this, [&stack, tab]()
    {
        stack.setCurrentWidget(tab);
    });
}

QPushButton* MainWindow::appendPushButton(const QString& buttonText)
{
    QFont font;
    font.setPointSize(14);

    QPushButton* button = new QPushButton(this);
    button->setFont(font);
    button->setAutoDefault(false);
    button->setFlat(true);
    button->setText(buttonText);

    pushButtons.push_back(button);
    ui.buttonsList->insertWidget(ui.buttonsList->count() - 1, button);

    return button;
}

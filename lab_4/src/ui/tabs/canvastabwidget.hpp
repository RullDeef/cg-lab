#pragma once

#include <vector>
#include <functional>
#include <QSpinBox>
#include <QGraphicsScene>
#include "interactivetabwidget.hpp"
#include "ui_canvastabwidget.h"

#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QColorDialog>
#include <QMouseEvent>

namespace ui
{
    class CanvasTabOption
    {
    public:
        CanvasTabOption(const char* text, QGroupBox* box) : label(new QLabel(box))
        {
            label->setText(text);
        }
        
        virtual ~CanvasTabOption() { delete label; }

        QLabel* getLabel() const { return label; }
        virtual QWidget* getInput() const = 0;

    private:
        QLabel* label;
    };

    class IntCanvasTabOption : public CanvasTabOption
    {
    public:
        IntCanvasTabOption(const char* text, QGroupBox* box)
            : CanvasTabOption(text, box), input(new QSpinBox(box))
        {}

        ~IntCanvasTabOption() { delete input; }

        QWidget* getInput() const override { return input; }

    private:
        QSpinBox* input;
    };

    class MouseAwareGraphicsView : public QGraphicsView
    {
    public:
        MouseAwareGraphicsView(QWidget* parent = Q_NULLPTR)
            : QGraphicsView(parent), callback([]() {})
        {
            setScene(&scene);
        }

        void setCallback(std::function<void()> callback)
        {
            this->callback = callback;
        }

        void setColor(QColor color)
        {
            scene.setBackgroundBrush(color);
        }

        void mousePressEvent(QMouseEvent* event) override
        {
            QPoint remapped = mapFromGlobal(event->globalPos());
            if (rect().contains(remapped)) callback();
        }

    private:
        std::function<void()> callback;
        QGraphicsScene scene;
    };

    class ColorCanvasTabOption : public CanvasTabOption
    {
    public:
        ColorCanvasTabOption(const char* text, QGroupBox* box)
            : CanvasTabOption(text, box), colorView(new MouseAwareGraphicsView(box))
        {
            QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            sizePolicy.setHorizontalStretch(0);
            sizePolicy.setVerticalStretch(0);
            sizePolicy.setHeightForWidth(colorView->sizePolicy().hasHeightForWidth());
            colorView->setSizePolicy(sizePolicy);
            colorView->setMaximumSize(QSize(16777215, 32));
            colorView->setFocusPolicy(Qt::NoFocus);
            colorView->setAcceptDrops(false);
            colorView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            colorView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            colorView->setInteractive(true);
        }

        ~ColorCanvasTabOption() { delete colorView; }

        QWidget* getInput() const override { return colorView; }

        void setCallback(std::function<void()> callback)
        {
            colorView->setCallback(callback);
        }

    private:
        MouseAwareGraphicsView* colorView;
    };

    class VariantCanvasTabOption : public CanvasTabOption
    {
    public:
        VariantCanvasTabOption(const char* text, QGroupBox* box, const QStringList& options)
            : CanvasTabOption(text, box), comboBox(new QComboBox(box))
        {
            comboBox->addItems(options);
        }

        ~VariantCanvasTabOption() { delete comboBox; }

        QWidget* getInput() const override { return comboBox; }

    private:
        QComboBox* comboBox;
    };

    class OptionsStrategy : public virtual QObject
    {
        Q_OBJECT

    public:
        ~OptionsStrategy()
        {
            for (const auto& option : options) delete option;
            for (const auto& action : actions) delete action;
        }

        void addIntOption(const char* text, int& value)
        {
            IntCanvasTabOption* option = new IntCanvasTabOption(text, group);
            addOption(option);

            connect((QSpinBox*)option->getInput(), (void(QSpinBox::*)(int))&QSpinBox::valueChanged,
                this, [&value](int newValue) { value = newValue; });
        }

        void addColorOption(const char* text, QColor& color)
        {
            ColorCanvasTabOption* option = new ColorCanvasTabOption(text, group);
            addOption(option);

            option->setCallback([&color, option]()
            {
                color = QColorDialog().getColor();
                ((MouseAwareGraphicsView*)option->getInput())->setColor(color);
            });
        }

        void addVariantOption(const char* text, int& index, const QStringList& options)
        {
            VariantCanvasTabOption* option = new VariantCanvasTabOption(text, group, options);
            addOption(option);

            connect((QComboBox*)option->getInput(), (void(QComboBox::*)(int))&QComboBox::currentIndexChanged,
                this, [&index](int newIndex) { index = newIndex; });
        }

        void addCustomAction(const char* text, std::function<void()> callback)
        {
            QPushButton* button = new QPushButton(group);
            button->setText(text);
            addActionButton(button);

            connect(button, &QPushButton::clicked, this, callback);
        }

    protected:
        void initOptionStrategy(QGroupBox* group, QFormLayout* form)
        {
            this->group = group;
            this->form = form;
        }

    private:
        void addOption(CanvasTabOption* option)
        {
            int row = form->rowCount();
            options.push_back(option);
            form->setWidget(row, QFormLayout::LabelRole, option->getLabel());
            form->setWidget(row, QFormLayout::FieldRole, option->getInput());
        }

        void addActionButton(QPushButton* button)
        {
            actions.push_back(button);
            group->layout()->addWidget(button);
        }

    private:
        QGroupBox* group;
        QFormLayout* form;

        std::vector<CanvasTabOption*> options;
        std::vector<QPushButton*> actions;
    };

    class CanvasTabWidget : public InteractiveTabWidget, public OptionsStrategy
    {
        Q_OBJECT

    public:
        CanvasTabWidget(const char* name, QWidget* parent = Q_NULLPTR)
            : InteractiveTabWidget(name, parent)
        {
            ui.setupUi(this);
            initOptionStrategy(ui.optionsGroup, ui.inputForm);

            canvasScene.setSceneRect(0, 0, ui.canvas->width(), ui.canvas->height());
            ui.canvas->setScene(&canvasScene);

            pixmap = QPixmap(ui.canvas->width(), ui.canvas->height());
            pixmap.fill(Qt::red);
            pixmapItem = canvasScene.addPixmap(pixmap);
        }

        void resizeEvent(QResizeEvent* event) override
        {
            pixmap = QPixmap(ui.canvas->width(), ui.canvas->height());
            pixmap.fill(Qt::red);
            canvasScene.setSceneRect(0, 0, ui.canvas->width(), ui.canvas->height());
        }

        void updateCanvas()
        {
            canvasScene.setSceneRect(0, 0, ui.canvas->width(), ui.canvas->height());
            canvasScene.update();
            ui.canvas->repaint();
        }

        void clearCanvas(QColor color = Qt::white)
        {
            pixmap.fill(color);
            updateCanvas();
        }

    protected:

        QPixmap pixmap;

    private:
        Ui::canvasTabWidget ui;

        QGraphicsScene canvasScene;
        QGraphicsPixmapItem* pixmapItem;
    };
}

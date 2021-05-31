#pragma once

#include <vector>
#include <QGroupBox>
#include <QLabel>
#include <QSpinBox>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QComboBox>
#include <QColorDialog>
#include <QPushButton>
#include <QFormLayout>

namespace ui
{
    class WidgetOption
    {
    public:
        WidgetOption(const char* text, QGroupBox* box) : label(new QLabel(box))
        {
            label->setText(text);
        }

        virtual ~WidgetOption() { delete label; }

        QLabel* getLabel() const { return label; }
        virtual QWidget* getInput() const = 0;

    private:
        QLabel* label;
    };

    class IntWidgetOption : public WidgetOption
    {
    public:
        IntWidgetOption(const char* text, QGroupBox* box, int minValue, int maxValue)
            : WidgetOption(text, box), input(new QSpinBox(box))
        {
            input->setMinimum(minValue);
            input->setMaximum(maxValue);
        }

        ~IntWidgetOption() {}

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

    class ColorWidgetOption : public WidgetOption
    {
    public:
        ColorWidgetOption(const char* text, QGroupBox* box)
            : WidgetOption(text, box), colorView(new MouseAwareGraphicsView(box))
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

        ~ColorWidgetOption() { delete colorView; }

        QWidget* getInput() const override { return colorView; }

        void setCallback(std::function<void()> callback)
        {
            colorView->setCallback(callback);
        }

    private:
        MouseAwareGraphicsView* colorView;
    };

    class VariantWidgetOption : public WidgetOption
    {
    public:
        VariantWidgetOption(const char* text, QGroupBox* box, const QStringList& options)
            : WidgetOption(text, box), comboBox(new QComboBox(box))
        {
            comboBox->addItems(options);
        }

        ~VariantWidgetOption() { delete comboBox; }

        QWidget* getInput() const override { return comboBox; }

    private:
        QComboBox* comboBox;
    };

    class WidgetOptionsStrategy : public virtual QObject
    {
        Q_OBJECT

    public:
        ~WidgetOptionsStrategy()
        {
            for (const auto& option : options) delete option;
            for (const auto& action : actions) delete action;
        }

        void addIntOption(const char* text, int& value, int minValue, int maxValue)
        {
            IntWidgetOption* option = new IntWidgetOption(text, group, minValue, maxValue);
            addOption(option);

            value = minValue;
            connect((QSpinBox*)option->getInput(), (void(QSpinBox::*)(int)) & QSpinBox::valueChanged,
                this, [&value](int newValue) { value = newValue; });
        }

        void addColorOption(const char* text, QColor& color)
        {
            ColorWidgetOption* option = new ColorWidgetOption(text, group);
            addOption(option);

            color = Qt::white;
            option->setCallback([&color, option]()
                {
                    color = QColorDialog().getColor();
                    ((MouseAwareGraphicsView*)option->getInput())->setColor(color);
                });
        }

        void addVariantOption(const char* text, int& index, const QStringList& options)
        {
            VariantWidgetOption* option = new VariantWidgetOption(text, group, options);
            addOption(option);

            index = 0;
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
        void addOption(WidgetOption* option)
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

        std::vector<WidgetOption*> options;
        std::vector<QPushButton*> actions;
    };
}

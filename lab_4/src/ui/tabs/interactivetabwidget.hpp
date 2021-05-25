#pragma once

#include <QWidget>
#include <QString>

namespace ui
{
    class InteractiveTabWidget : public virtual QWidget
    {
    public:
        InteractiveTabWidget(const char* name, QWidget* parent = Q_NULLPTR)
            : QWidget(parent), name(name)
        {}

        QString getName() const { return name; }

    private:
        const char* name;
    };
}

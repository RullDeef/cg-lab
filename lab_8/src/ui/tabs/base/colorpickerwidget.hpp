#pragma once

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QColorDialog>

namespace ui
{
    class ColorPickerWidget : public QGraphicsView
    {
        Q_OBJECT

    public:
        ColorPickerWidget(QWidget* parent = Q_NULLPTR) : QGraphicsView(parent)
        {
            setScene(&scene);

            QSizePolicy sizePol(QSizePolicy::Expanding, QSizePolicy::Expanding);
            sizePol.setHorizontalStretch(0);
            sizePol.setVerticalStretch(0);
            sizePol.setHeightForWidth(sizePolicy().hasHeightForWidth());
            setSizePolicy(sizePol);
            setMaximumSize(QSize(16777215, 32));
            setFocusPolicy(Qt::NoFocus);
            setAcceptDrops(false);
            setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            setInteractive(true);
        }

        virtual ~ColorPickerWidget() = default;

        void setColor(const QColor& color) { scene.setBackgroundBrush(color); }
        QColor getColor() const { return scene.backgroundBrush().color(); }

    signals:
        void colorChanged(const QColor& color);

    protected slots:
        virtual void mousePressEvent(QMouseEvent* event) override
        {
            QColor color = QColorDialog().getColor();
            setColor(color);
            emit colorChanged(color);
        }

    private:
        QGraphicsScene scene;
    };
}

#include "../../../core/cutter/simplecutter.hpp"
#include "../../../core/cutter/coencutter.hpp"
#include "rectcuttercanvas.hpp"
#include <QMouseEvent>

ui::RectCutterCanvas::RectCutterCanvas()
{
    setMouseTracking(true);
}

void ui::RectCutterCanvas::applyCut()
{
    linesCutted = lines;
    core::CoenCutter cutter;
    cutter.cut(linesCutted, rect);
    repaint();
}

void ui::RectCutterCanvas::selectCutter()
{
    selectingRect = true;
    rect.top = 0;
    rect.left = 0;
    rect.width = 0;
    rect.height = 0;
}

void ui::RectCutterCanvas::beginLine(int x, int y)
{
    tempLine.x1 = tempLine.x2 = x;
    tempLine.y1 = tempLine.y2 = y;
}

void ui::RectCutterCanvas::continueLine(int x, int y)
{
    tempLine.x2 = x;
    tempLine.y2 = y;
}

void ui::RectCutterCanvas::endLine()
{
    lines.push_back(tempLine);

    tempLine.x1 = 0;
    tempLine.y1 = 0;
    tempLine.x2 = 0;
    tempLine.y2 = 0;
}

void ui::RectCutterCanvas::normalizeCutter()
{
    if (rect.width < 0)
    {
        rect.left += rect.width;
        rect.width *= -1;
    }
    if (rect.height < 0)
    {
        rect.top += rect.height;
        rect.height *= -1;
    }
}

void ui::RectCutterCanvas::paintEvent(QPaintEvent* event)
{
    Canvas::paintEvent(event);
    repaintCanvas();
}

void ui::RectCutterCanvas::mousePressEvent(QMouseEvent* event)
{
    if (selectingRect)
    {
        rect.left = event->pos().x();
        rect.top = event->pos().y();
        rect.width = 0;
        rect.height = 0;
    }
    else
    {
        beginLine(event->pos().x(), event->pos().y());
        drawingLine = true;
    }
}

void ui::RectCutterCanvas::mouseReleaseEvent(QMouseEvent* event)
{
    if (selectingRect)
    {
        selectingRect = false;
        normalizeCutter();
        emit cutterSelected(rect);
    }
    else if (drawingLine)
    {
        drawingLine = false;
        lines.push_back(tempLine);
    }
}

void ui::RectCutterCanvas::mouseMoveEvent(QMouseEvent* event)
{
    mousePos = event->pos();

    if (selectingRect && rect.left > 0 && rect.top > 0)
    {
        rect.width = event->pos().x() - rect.left;
        rect.height = event->pos().y() - rect.top;
    }
    else if (drawingLine)
    {
        tempLine.x2 = event->pos().x();
        tempLine.y2 = event->pos().y();
    }

    repaint();
}

void ui::RectCutterCanvas::enterEvent(QEvent* event)
{
    mouseEntered = true;
}

void ui::RectCutterCanvas::leaveEvent(QEvent* event)
{
    mouseEntered = false;
}

void ui::RectCutterCanvas::repaintCanvas()
{
    QImage& image = getImage();
    image.fill(Qt::white);

    QPainter painter(&image);
    painter.drawImage(0, 0, overlay);

    drawAxes(painter);
    drawCutter(painter);
    drawLines(painter);

    if (mouseEntered)
    {
        //pointConstrainter.drawConstraints(painter, mousePos);
        drawCursor(painter);
    }

    repaint();
}

void ui::RectCutterCanvas::drawCutter(QPainter& painter)
{
    painter.setPen(QPen(Qt::green, 1));
    painter.drawRect(rect.left, rect.top, rect.width, rect.height);
}

void ui::RectCutterCanvas::drawLines(QPainter& painter)
{
    painter.setPen(QPen(Qt::cyan, 1));
    for (const auto& line : lines)
        painter.drawLine(line.x1, line.y1, line.x2, line.y2);
    painter.drawLine(tempLine.x1, tempLine.y1, tempLine.x2, tempLine.y2);

    painter.setPen(QPen(Qt::red, 3));
    for (const auto& line : linesCutted)
        painter.drawLine(line.x1, line.y1, line.x2, line.y2);
}

void ui::RectCutterCanvas::drawCursor(QPainter& painter)
{
    int x = mousePos.x();
    int y = mousePos.y();

    int padding = 3;
    int radius = 6;
    int margin = 5;

    painter.setPen(Qt::red);

    painter.drawEllipse(mousePos, radius, radius);
    painter.drawLine(x, y - padding, x, y - radius - margin);
    painter.drawLine(x, y + padding, x, y + radius + margin);
    painter.drawLine(x - padding, y, x - radius - margin, y);
    painter.drawLine(x + padding, y, x + radius + margin, y);
}

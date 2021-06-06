#include "core/cutter/cyrusbeckcutter.hpp"
#include "rectcuttercanvas.hpp"
#include "cutterconstrainter.hpp"
#include <QMouseEvent>


ui::RectCutterCanvas::RectCutterCanvas()
{
    setMouseTracking(true);
}

void ui::RectCutterCanvas::applyCut()
{
    linesCutted = lines;
    core::CyrusBeckCutter convexCutter;
    convexCutter.cut(linesCutted, cutter);
    repaint();
}

bool ui::RectCutterCanvas::cutterIsValid() const
{
    return cutter.isConvex();
}

void ui::RectCutterCanvas::removeSegments()
{
    lines.clear();
    linesCutted.clear();

    tempLine.x1 = 0;
    tempLine.y1 = 0;
    tempLine.x2 = 0;
    tempLine.y2 = 0;
}

void ui::RectCutterCanvas::clearCut()
{
    linesCutted.clear();

    update();
}

void ui::RectCutterCanvas::selectCutter()
{
    selectingCutter = true;
    cutter.clear();
}

void ui::RectCutterCanvas::beginLine(int x, int y)
{
    tempLine.x1 = tempLine.x2 = x;
    tempLine.y1 = tempLine.y2 = y;

    startPoint.x = x;
    startPoint.y = y;
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

    startPoint.x = 0;
    startPoint.y = 0;
}

void ui::RectCutterCanvas::normalizeCutter()
{
    auto contour = cutter.getContours().front();

    double sum = 0.0;
    for (auto prev = contour.begin(), curr = contour.begin(); ++curr != contour.end(); prev++)
        sum += prev->cross(*curr);
    
    if (sum < 0.0)
        for (auto& line : contour)
            line.reverse();

    cutter.getLines() = contour;
}

void ui::RectCutterCanvas::paintEvent(QPaintEvent* event)
{
    Canvas::paintEvent(event);
    repaintCanvas();
}

void ui::RectCutterCanvas::mousePressEvent(QMouseEvent* event)
{
    if (selectingCutter)
    {
        if (event->button() == Qt::MouseButton::LeftButton)
        {
            if (cutter.getPoints().size() > 0)
            {
                if (cutter.getPoints().front().distance(event->pos().x(), event->pos().y()) < 10.0)
                {
                    auto& line = cutter.getLines().back();
                    line.p2 = &cutter.getPoints().front();
                    cutter.getPoints().pop_back();

                    selectingCutter = false;
                    lastPoint = nullptr;

                    normalizeCutter();
                    emit cutterSelected(cutter);
                    return;
                }
            }

            auto prevLastPoint = lastPoint;
            lastPoint = &cutter.appendPoint(event->pos().x(), event->pos().y());

            if (!prevLastPoint)
            {
                prevLastPoint = lastPoint;
                lastPoint = &cutter.appendPoint(event->pos().x(), event->pos().y());
            }

            cutter.connectPoints(*prevLastPoint, *lastPoint);
        }
        else
        {
            // cancel input
            selectingCutter = false;
            lastPoint = nullptr;
            cutter.clear();
        }
    }
    else
    {
        beginLine(event->pos().x(), event->pos().y());
        drawingLine = true;
    }
}

void ui::RectCutterCanvas::mouseReleaseEvent(QMouseEvent* event)
{
    if (!selectingCutter && drawingLine)
    {
        drawingLine = false;
        lines.push_back(tempLine);
    }
}

void ui::RectCutterCanvas::mouseMoveEvent(QMouseEvent* event)
{
    mousePos = event->pos();

    if (selectingCutter)
    {
        if (lastPoint)
        {
            lastPoint->x = mousePos.x();
            lastPoint->y = mousePos.y();
        }
    }
    else if (drawingLine)
    {
        core::Point p(event->pos().x(), event->pos().y());
        if (event->modifiers() & Qt::ShiftModifier)
            p = CutterConstrainter(startPoint).constraint(p, cutter);

        tempLine.x2 = p.x;
        tempLine.y2 = p.y;
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

    if (mouseEntered)
    {
        drawCursor(painter);

        if (drawingLine)
            drawConstraints(painter);
    }

    drawLines(painter);

    repaint();
}

void ui::RectCutterCanvas::drawCutter(QPainter& painter)
{
    painter.setPen(QPen(Qt::darkGreen, 1));

    for (const auto& line : cutter.getLines())
        painter.drawLine(line.p1->x, line.p1->y, line.p2->x, line.p2->y);
}

void ui::RectCutterCanvas::drawLines(QPainter& painter)
{
    painter.setPen(QPen(Qt::blue, 1));
    for (const auto& line : lines)
        painter.drawLine(line.x1, line.y1, line.x2, line.y2);
    painter.drawLine(tempLine.x1, tempLine.y1, tempLine.x2, tempLine.y2);

    painter.setPen(QPen(Qt::darkRed, 3));
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

void ui::RectCutterCanvas::drawConstraints(QPainter& painter)
{
    core::Point p(tempLine.x2, tempLine.y2);
    CutterConstrainter(startPoint).drawConstraints(painter, p, cutter);
}

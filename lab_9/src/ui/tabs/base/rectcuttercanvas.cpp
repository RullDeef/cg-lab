#include "core/cutter/weileratherton.hpp"
#include "rectcuttercanvas.hpp"
#include "cutterconstrainter.hpp"
#include <QMouseEvent>


ui::RectCutterCanvas::RectCutterCanvas()
{
    setMouseTracking(true);
}

void ui::RectCutterCanvas::applyCut()
{
    regionCutted = region.clone();
    core::WeilerAtherton weilerAthertonCutter;
    weilerAthertonCutter.cut(regionCutted, cutter);
    repaint();
}

bool ui::RectCutterCanvas::cutterIsValid() const
{
    return !cutter.selfIntersecting();
}

void ui::RectCutterCanvas::removeRegion()
{
    regionBuilder.reset();
    region.clear();
    regionCutted.clear();
}

void ui::RectCutterCanvas::clearCut()
{
    regionCutted.clear();

    update();
}

void ui::RectCutterCanvas::selectCutter()
{
    selectingCutter = true;
}

void ui::RectCutterCanvas::addRegion()
{
    selectingContour = true;
    selectingCutter = false;
}

void ui::RectCutterCanvas::normalizeCutter()
{
    std::list<core::Line> lines;

    for (auto contour : cutter.getContours()) // TODO : in region method
    {
        double sum = 0.0;
        for (auto prev = contour.begin(), curr = contour.begin(); ++curr != contour.end(); prev++)
            sum += prev->cross(*curr);
    
        if (sum < 0.0)
            for (auto& line : contour)
                line.reverse();
    }

    cutter.getLines() = lines;
}

void ui::RectCutterCanvas::paintEvent(QPaintEvent* event)
{
    Canvas::paintEvent(event);
    repaintCanvas();
}

void ui::RectCutterCanvas::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MouseButton::LeftButton)
    {
        if (selectingCutter)
        {
            cutterBuilder.confirmLastPoint();
            cutter = cutterBuilder.getResult();
        }
        else if (selectingContour)
        {
            regionBuilder.confirmLastPoint();
            region = regionBuilder.getResult();
        }
    }
}

void ui::RectCutterCanvas::mouseReleaseEvent(QMouseEvent* event)
{
}

void ui::RectCutterCanvas::mouseMoveEvent(QMouseEvent* event)
{
    mousePos = event->pos();

    if (selectingCutter)
    {
        cutterBuilder.updateLastPoint(mousePos.x(), mousePos.y());
        cutter = cutterBuilder.getResult();
    }
    else if (selectingContour)
    {
        regionBuilder.updateLastPoint(mousePos.x(), mousePos.y());
        region = regionBuilder.getResult();
    }

    repaint();
}

void ui::RectCutterCanvas::enterEvent(QEnterEvent* event)
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

        //if (drawingLine)
        //    drawConstraints(painter);
    }

    drawRegion(painter);

    repaint();
}

void ui::RectCutterCanvas::drawCutter(QPainter& painter)
{
    painter.setPen(QPen(Qt::darkGreen, 1));

    for (const auto& line : cutter.getLines())
        painter.drawLine(line.p1->x, line.p1->y, line.p2->x, line.p2->y);
}

void ui::RectCutterCanvas::drawRegion(QPainter& painter)
{
    painter.setPen(QPen(Qt::blue, 1));
    for (const auto& line : region.getLines())
        painter.drawLine(line.p1->x, line.p1->y, line.p2->x, line.p2->y);
    // painter.drawLine(tempLine.x1, tempLine.y1, tempLine.x2, tempLine.y2);

    painter.setPen(QPen(Qt::darkRed, 3));
    for (const auto& line : regionCutted.getLines())
        painter.drawLine(line.p1->x, line.p1->y, line.p2->x, line.p2->y);
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
    // core::Point p(tempLine.x2, tempLine.y2);
    // CutterConstrainter(startPoint).drawConstraints(painter, p, cutter);
}

#include <cmath>
#include <stack>
#include "bucfillregionrenderer.hpp"


void core::AsyncBucFillRegionRenderer::fill(QImage& image, const QPoint& startPoint, QColor color)
{
    int width = image.width();
    int height = image.height();

    std::stack<QPoint> pointStack;
    
    // занесение в стек затравочного пиксела
    pointStack.push(startPoint);

    while (!pointStack.empty())
    {
        // извлекаем очередной затравочный пиксел из стека
        QPoint point = pointStack.top();
        pointStack.pop();

        int y = point.y();
        int yBelow = y + 1;
        int yAbove = y - 1;

        // заполнение влево от затравочного пиксела
        int xLeft = point.x(), xRight = point.x() + 1;
        for (; xLeft >= 0 && image.pixel(xLeft, y) != color.rgba(); xLeft--)
            image.setPixel(xLeft, y, color.rgba());
        xLeft++;

        // заполнение вправо
        for (; xRight < width && image.pixel(xRight, y) != color.rgba(); xRight++)
            image.setPixel(xRight, y, color.rgba());
        xRight--;

        // Поиск новых затравочных пикселей на двух соседних сторонах
        bool prevBound = true;
        if (yAbove >= 0)
        {
            for (int x = xLeft; x <= xRight; x++)
            {
                bool currBound = image.pixel(x, yAbove) == color.rgba();
                // если прерыдущий пиксел был крайним правым - помечаем как затравочный
                if (currBound && !prevBound)
                    pointStack.push(QPoint(x - 1, yAbove));
                prevBound = currBound;
            }

            if (!prevBound) // помещаем последний пиксел в стек
                pointStack.push(QPoint(xRight, yAbove));
        }

        if (yBelow < height)
        {
            prevBound = true;
            for (int x = xLeft; x <= xRight; x++)
            {
                bool currBound = image.pixel(x, yBelow) == color.rgba();
                if (currBound && !prevBound)
                    pointStack.push(QPoint(x - 1, yBelow));
                prevBound = currBound;
            }

            if (!prevBound)
                pointStack.push(QPoint(xRight, yBelow));
        }
    }
}

void core::AsyncBucFillRegionRenderer::asyncFill(QImage& image, const QPoint& startPoint, QColor color)
{
    int width = image.width();
    int height = image.height();

    if (!initialized)
    {
        pointStack = {};
        pointStack.push(startPoint);
        this->color = color;

        initialized = true;
    }
    else if (!pointStack.empty())
    {
        // извлекаем очередной затравочный пиксел из стека
        QPoint point = pointStack.top();
        pointStack.pop();

        int y = point.y();
        int yBelow = y + 1;
        int yAbove = y - 1;

        // заполнение влево от затравочного пиксела
        int xLeft = point.x(), xRight = point.x() + 1;
        for (; xLeft >= 0 && image.pixel(xLeft, y) != this->color.rgba(); xLeft--)
            image.setPixel(xLeft, y, this->color.rgba());
        xLeft++;

        // заполнение вправо
        for (; xRight < width && image.pixel(xRight, y) != this->color.rgba(); xRight++)
            image.setPixel(xRight, y, this->color.rgba());
        xRight--;

        // Поиск новых затравочных пикселей на двух соседних сторонах
        bool prevBound = true;
        if (yAbove >= 0)
        {
            for (int x = xLeft; x <= xRight; x++)
            {
                bool currBound = image.pixel(x, yAbove) == this->color.rgba();
                // если прерыдущий пиксел был крайним правым - помечаем как затравочный
                if (currBound && !prevBound)
                    pointStack.push(QPoint(x - 1, yAbove));
                prevBound = currBound;
            }

            if (!prevBound) // помещаем последний пиксел в стек
                pointStack.push(QPoint(xRight, yAbove));
        }

        if (yBelow < height)
        {
            prevBound = true;
            for (int x = xLeft; x <= xRight; x++)
            {
                bool currBound = image.pixel(x, yBelow) == this->color.rgba();
                if (currBound && !prevBound)
                    pointStack.push(QPoint(x - 1, yBelow));
                prevBound = currBound;
            }

            if (!prevBound)
                pointStack.push(QPoint(xRight, yBelow));
        }
    }
    else
    {
        finish();
    }
}

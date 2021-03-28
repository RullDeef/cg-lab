#pragma once

#include "ellipserenderer.hpp"

class LibEllRen : public EllipseRenderer
{
public:
    LibEllRen(size_t queueSize = defaultQueueSize) :
        EllipseRenderer(u8"Библиотечный", queueSize) {}

    void drawEllipse(QImage& image, const Ellipse& ellipse) override;
};


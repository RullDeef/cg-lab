#pragma once

#include "circlerenderer.hpp"

class LibCirRen : public CircleRenderer
{
public:
    LibCirRen(size_t queueSize = defaultQueueSize) :
        CircleRenderer(u8"������������", queueSize) {}

    void drawCircle(QImage& image, const Circle& circle) override;
};

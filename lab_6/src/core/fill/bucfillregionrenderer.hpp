#pragma once

#include <stack>
#include "../regionrenderer.hpp"

namespace core
{
    class AsyncBucFillRegionRenderer : public AsyncBucFillRenderer
    {
    public:
        AsyncBucFillRegionRenderer() : AsyncBucFillRenderer(u8"Простой затравочный алгоритм") {}

        void fill(QImage& image, const QPoint& startPoint, QColor color) override;
        void asyncFill(QImage& image, const QPoint& startPoint, QColor color) override;

    private:
        bool initialized = false;

        std::stack<QPoint> pointStack;
        QColor color;
    };

    using BucFillRegionRenderer = AsyncBucFillRegionRenderer;
}

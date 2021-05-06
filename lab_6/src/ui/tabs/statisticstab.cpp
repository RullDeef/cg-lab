#include "statisticstab.hpp"

using namespace core;

ui::StatisticsTab::StatisticsTab(RenderersContainer<Circle> renderers, QWidget* parent)
    : LineChartTabWidget(u8"временные хр-ки (окружности)", u8"График зависимости времени построения окружности от её радиуса", parent)
{
    QImage image(1200, 1200, QImage::Format_ARGB32);
    image.fill(Qt::white);

    for (const auto& iter : renderers)
    {
        auto& name = iter.first;
        auto& renderer = iter.second;

        for (int r = 10; r < 200; r += 10)
        {
            Circle circle;
            circle.x = 600;
            circle.y = 600;
            circle.r = r;

            long long time = 0, iterations = 200;
            for (int i = 0; i < iterations; i++)
            {
                renderer->draw(image, circle, Qt::black);
                time += renderer->getDuration().count();
            }
            addPoint(name, r, double(time) / iterations);
        }
    }
}

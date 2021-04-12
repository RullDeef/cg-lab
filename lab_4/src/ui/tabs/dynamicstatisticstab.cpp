#include "dynamicstatisticstab.hpp"

using namespace core;

ui::DynamicStatisticsTab::DynamicStatisticsTab(RenderersContainer<core::Ellipse> renderers, QWidget* parent)
    : LineChartTabWidget(u8"временные хр-ки (эллипсы)", u8"График зависимости времени построения эллипса от его второй полуоси", parent),
    renderers(renderers)
{
    // setup custom inputbox
    QGroupBox* groupBox = new QGroupBox;
    groupBox->setTitle(u8"Параметры");

    QFont font = groupBox->font();
    font.setPointSize(14);
    groupBox->setFont(font);

    QVBoxLayout* groupLayout = new QVBoxLayout;
    QFormLayout* form = new QFormLayout;
    groupBox->setLayout(groupLayout);
    groupLayout->addLayout(form);
    groupLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Minimum));

    getLayout()->addWidget(groupBox);

    WidgetOptionsStrategy::initOptionStrategy(groupBox, form);

    // setup options

    addIntOption(u8"Полуось a (OX)", ellipseA, 1, 500);
    addCustomAction(u8"перерасчёт", [this]() {
        updateGraph(ellipseA);
    });

    updateGraph(100);
}

void ui::DynamicStatisticsTab::updateGraph(double ellipseA)
{
    QImage image(1200, 1200, QImage::Format_ARGB32);
    image.fill(Qt::white);

    core::Ellipse ellipse;
    ellipse.x = 600;
    ellipse.y = 600;
    ellipse.a = ellipseA;

    clearSeries();
    for (const auto& iter : renderers)
    {
        auto& name = iter.first;
        auto& renderer = iter.second;
        for (int b = 10; b < 500; b += 10)
        {
            ellipse.b = b;

            long long time = 0, iterations = 200;
            for (int i = 0; i < iterations; i++)
            {
                renderer->draw(image, ellipse, Qt::black);
                time += renderer->getDuration().count();
            }

            addPoint(name, b, double(time) / iterations);
        }
    }
}

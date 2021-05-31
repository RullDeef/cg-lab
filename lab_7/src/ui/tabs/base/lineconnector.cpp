#include "lineconnector.hpp"

using namespace core;
using namespace ui;

bool LineConnector::removeLineAt(int x, int y)
{
    constexpr auto offset = 12.0;

    auto& lines = (*region).getLines();

    auto iter = std::find_if(lines.begin(), lines.end(),
        [x, y, offset](const BasicRegion::Line& line)
        { return line.closeTo(x, y, offset); });

    if (iter != lines.end())
    {
        lines.erase(iter);
        return true;
    }

    return false;
}

void LineConnector::selectionChanged(BasicRegion::Point* oldSelectedPoint, BasicRegion::Point* newSelectedPoint)
{
    if (!enabled)
        return;

    if (oldSelectedPoint == nullptr || newSelectedPoint == nullptr)
        return;

    (*region).connectPoints(*oldSelectedPoint, *newSelectedPoint);
}

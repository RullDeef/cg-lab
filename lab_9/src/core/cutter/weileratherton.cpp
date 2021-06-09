#include <list>
#include "weileratherton.hpp"

using namespace core;

struct WeilerVertex
{
    double x = 0.0;
    double y = 0.0;
    bool inside = false;
    bool intersectionPoint = false;

    WeilerVertex* prev = nullptr;
    WeilerVertex* next = nullptr;
    WeilerVertex* alter = nullptr;
};


static WeilerVertex* createVertexList(const std::list<Point>& contour)
{
    if (contour.size() == 0)
        __debugbreak();

    WeilerVertex* headVertex = nullptr;
    WeilerVertex* tailVertex = nullptr;

    for (const auto& point : contour)
    {
        WeilerVertex* vertex = new WeilerVertex();
        vertex->x = point.x;
        vertex->y = point.y;
        vertex->prev = tailVertex;
        tailVertex = vertex;

        if (!headVertex)
            headVertex = vertex;
    }

    headVertex->prev = tailVertex;
    tailVertex->next = headVertex;

    for (WeilerVertex* node = tailVertex; node && !node->prev->next; node = node->prev)
        node->prev->next = node;

    return headVertex;
}

static void deleteVertexList(WeilerVertex* list)
{
    while (list)
    {
        WeilerVertex* next = list->next;
        list->prev->next = nullptr;
        delete list;
        list = next;
    }
}

static std::list<Point> expandContour(const std::list<Line>& contour)
{
    std::list<Point> points;
    for (const auto& line : contour)
        points.push_back(*line.p1);
    return points;
}

static bool checkForIntersection(WeilerVertex* node1, WeilerVertex* node2)
{
    double n2x = -(node2->next->y - node2->y);
    double n2y = node2->next->x - node2->x;

    double n1x = -(node1->next->y - node1->y);
    double n1y = node1->next->x - node1->x;

    double D1x = node1->next->x - node1->x;
    double D1y = node1->next->y - node1->y;

    double D2x = node2->next->x - node2->x;
    double D2y = node2->next->y - node2->y;

    double Wx = node2->x - node1->x;
    double Wy = node2->y - node1->y;

    double n2W = n2x * Wx + n2y * Wy;
    double n2D1 = n2x * D1x + n2y * D1y;

    double n1W = n1x * (-Wx) + n1y * (-Wy);
    double n1D2 = n1x * D2x + n1y * D2y;

    double t = n2W / n2D1;
    double s = n1W / n1D2;

    if (0.001 < t && t < 0.998 && 0.001 < s && s < 0.998)
    {
        WeilerVertex* vert1 = new WeilerVertex();
        WeilerVertex* vert2 = new WeilerVertex();

        vert1->x = vert2->x = node1->x + t * D1x;
        vert1->y = vert2->y = node1->y + t * D1y;
        vert1->inside = vert2->inside = n2D1 > 0;
        vert1->intersectionPoint = true;
        vert2->intersectionPoint = true;

        vert1->alter = vert2;
        vert2->alter = vert1;

        vert1->next = node1->next;
        vert1->prev = node1;
        node1->next = vert1->next->prev = vert1;

        vert2->next = node2->next;
        vert2->prev = node2;
        node2->next = vert2->next->prev = vert2;

        return true;
    }

    return false;
}

static void intersectVertexLists(WeilerVertex* list1, WeilerVertex* list2)
{
    size_t i = 0;
    for (WeilerVertex* node1 = list1; i == 0 || node1 != list1; i++, node1 = node1->next)
    {
        size_t j = 0;
        for (WeilerVertex* node2 = list2; j == 0 || node2 != list2; j++, node2 = node2->next)
            checkForIntersection(node1, node2);
    }
}

static std::list<WeilerVertex*> getInputPoints(WeilerVertex* contour)
{
    std::list<WeilerVertex*> result;

    size_t i = 0;
    for (WeilerVertex* node = contour; i == 0 || node != contour; node = node->next, i++)
        if (node->inside)
            result.push_back(node);

    return result;
}

static bool isInside(const std::list<Point>& inner, const std::list<Point>& outer)
{
    double sum = 0.0;
    int count = 0;
    Point target = inner.front();

    for (auto prev = outer.begin(), curr = ++outer.begin(); prev != outer.end(); prev++, curr++)
    {
        if (curr == outer.end()) curr = outer.begin();

        sum += prev->x * curr->y - curr->x * prev->y;

        if ((prev->y - target.y) * (curr->y - target.y) < 0)
        {
            double dx = curr->x - prev->x;
            double dy = curr->y - prev->y;

            double x = prev->x + dx / dy * (target.y - prev->y);

            if (target.x <= x)
                count++;
        }
    }

    return (sum < 0.0) ^ (count % 2 == 1);
}

std::list<std::list<Point>> cutContour(const std::list<Point>& contour, const std::list<Point>& cutter)
{
    WeilerVertex* contourList = createVertexList(contour);
    WeilerVertex* cutterList = createVertexList(cutter);

    intersectVertexLists(contourList, cutterList);
    std::list<WeilerVertex*> inputPoints = getInputPoints(contourList);

    std::list<std::list<Point>> result;

    if (inputPoints.size() != 0)
    {
        for (const auto& inputPoint : inputPoints)
        {
            std::list<Point> tempContour;
            tempContour.push_back(Point(inputPoint->x, inputPoint->y));

            for (WeilerVertex* node = inputPoint->next; node != inputPoint && node->alter != inputPoint; node = node->next)
            {
                if (node->intersectionPoint)
                    node = node->alter;

                tempContour.push_back(Point(node->x, node->y));
            }

            result.push_back(tempContour);
        }
    }
    else
    {
        // 2 points test
        if (isInside(contour, cutter))
            result.push_back(contour);
        else if (isInside(cutter, contour))
            result.push_back(cutter);
    }

    deleteVertexList(contourList);
    deleteVertexList(cutterList);

    return result;
}

BasicRegion transformToRegion(const std::list<std::list<Point>>& contours)
{
    BasicRegion region;

    for (const auto& contour : contours)
    {
        for (const auto& point : contour)
            region.appendPoint(point);

        for (auto prev_point = contour.begin(), curr_point = ++contour.begin();
                curr_point != contour.end(); curr_point++, prev_point++)
            region.connectPoints(*prev_point, *curr_point);
        region.connectPoints(contour.back(), contour.front());
    }

    return region;
}

void core::WeilerAtherton::cut(BasicRegion& region, const BasicRegion& cutter)
{
    auto contours = region.getContours();
    auto cutterContours = cutter.getContours();

    std::list<std::list<Point>> resultContours;
    std::list<std::list<Point>> tempContours;
    for (const auto& contour : contours)
        resultContours.push_back(expandContour(contour));

    for (const auto& cutterContour : cutterContours)
    {
        auto expandedCutter = expandContour(cutterContour);
        tempContours = resultContours;
        resultContours = {};

        for (const auto& cuttee : tempContours)
        {
            auto cutResults = cutContour(cuttee, expandedCutter);
            resultContours.insert(resultContours.end(), cutResults.begin(), cutResults.end());
        }
    }

    region = transformToRegion(resultContours);
}

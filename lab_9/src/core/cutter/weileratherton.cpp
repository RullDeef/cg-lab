#include <list>
#include "weileratherton.hpp"

using namespace core;

struct WeilerVertex
{
    double x = 0.0;
    double y = 0.0;
    bool inside = false;
    bool intersectionPoint = false;
    bool visited = false;

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

static std::list<Point> contourFromVertexList(WeilerVertex* contour)
{
    std::list<Point> result;

    size_t i = 0;
    for (auto node = contour; i == 0 || node != contour; i++, node = node->next)
        result.push_back(Point(node->x, node->y));

    return result;
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

static bool hasIntersectionPoints(WeilerVertex* contour)
{
    size_t i = 0;
    for (WeilerVertex* node = contour; i == 0 || node != contour; i++, node = node->next)
        if (node->intersectionPoint)
            return true;
    return false;
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

    return /* (sum < 0.0) ^ */ (count % 2 == 1);
}

static bool isOuterContour(const std::list<Point>& contour)
{
    double sum = contour.back().x * contour.front().y - contour.front().x * contour.back().y;
    for (auto p1 = contour.begin(), p2 = ++contour.begin(); p2 != contour.end(); p1++, p2++)
        sum += p1->x * p2->y - p2->x * p1->y;
    return sum > 0.0;
}

static std::list<Point> normalized(const std::list<Point>& contour)
{
    if (isOuterContour(contour))
        return contour;

    return std::list(contour.rbegin(), contour.rend());
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
        bool contourOuter = isOuterContour(contour);
        bool cutterOuter = isOuterContour(cutter);

        // 2 points test
        if (isInside(contour, cutter))
        {
            if (contourOuter && cutterOuter)
                result.push_back(contour);
            else if (!contourOuter && !cutterOuter)
                result.push_back(cutter);
            else if (!contourOuter && cutterOuter)
            {
                result.push_back(contour);
                //result.push_back(cutter);
            }
        }
        else if (isInside(cutter, contour))
        {
            if (contourOuter && cutterOuter)
                result.push_back(cutter);
            else if (!contourOuter && !cutterOuter)
                result.push_back(contour);
            else if (contourOuter && !cutterOuter)
            {
                result.push_back(contour);
                result.push_back(cutter);
            }
        }
    }

    deleteVertexList(contourList);
    deleteVertexList(cutterList);

    return result;
}

static void filterInvertContours(std::list<std::list<Point>>& unstable, const std::list<Point>& region)
{
    unstable = std::list(unstable.begin(), std::remove_if(unstable.begin(), unstable.end(),
        [&region](const std::list<Point>& c) -> bool {
            return !isInside(c, region);
        }));
    // if (stable.size() == 0)
    // {
    // }
    // else
    // {
    //     std::list<std::list<Point>> result;
    // 
    //     for (auto c1 = stable.begin(); c1 != stable.end(); c1++)
    //     {
    //         int count = 0;
    //         for (auto c2 = stable.begin(); c2 != stable.end(); c2++)
    //         {
    //             if (c1 == c2)
    //                 continue;
    //             if ((isOuterContour(*c1) ^ isOuterContour(*c2)) && isInside(*c1, *c2))
    //                 count++;
    //         }
    //         if ((count % 2 == 1) ^ isOuterContour(*c1))
    //             result.push_back(*c1);
    //     }
    //     stable = result;
    // }
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
    std::list<WeilerVertex*> regionContours;
    std::list<WeilerVertex*> cutterContours;

    for (const auto& contour : region.getContours())
        regionContours.push_back(createVertexList(expandContour(contour)));

    for (const auto& contour : cutter.getContours())
        cutterContours.push_back(createVertexList(expandContour(contour)));

    for (auto& c1 : regionContours)
        for (auto& c2 : cutterContours)
            intersectVertexLists(c1, c2);

    std::list<WeilerVertex*> inputPoints;
    for (auto& contour : regionContours)
    {
        auto points = getInputPoints(contour);
        inputPoints.insert(inputPoints.end(), points.begin(), points.end());
    }

    std::list<std::list<Point>> stableContours;

    if (inputPoints.size() != 0)
    {
        for (const auto& inputPoint : inputPoints)
        {
            if (inputPoint->visited)
                continue;
            inputPoint->visited = true;

            std::list<Point> tempContour;
            tempContour.push_back(Point(inputPoint->x, inputPoint->y));

            for (WeilerVertex* node = inputPoint->next; node != inputPoint && node->alter != inputPoint; node = node->next)
            {
                if (node->intersectionPoint)
                {
                    node->visited = true;
                    node = node->alter;
                    node->visited = true;
                }

                tempContour.push_back(Point(node->x, node->y));
            }

            stableContours.push_back(tempContour);
        }
    }

    {
        std::list<std::list<Point>> unstableContours;
        for (auto& contour : regionContours)
            if (!hasIntersectionPoints(contour))
                unstableContours.push_back(contourFromVertexList(contour));
        filterInvertContours(unstableContours, contourFromVertexList(cutterContours.front()));
        stableContours.insert(stableContours.end(), unstableContours.begin(), unstableContours.end());
    }

    {
        std::list<std::list<Point>> unstableContours;
        for (auto& contour : cutterContours)
            if (!hasIntersectionPoints(contour))
                unstableContours.push_back(contourFromVertexList(contour));
        filterInvertContours(unstableContours, contourFromVertexList(regionContours.front()));
        stableContours.insert(stableContours.end(), unstableContours.begin(), unstableContours.end());
    }

    for (auto& contour : regionContours)
        deleteVertexList(contour);
    for (auto& contour : cutterContours)
        deleteVertexList(contour);

    region = transformToRegion(stableContours);
}

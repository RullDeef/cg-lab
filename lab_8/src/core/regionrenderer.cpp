#pragma once

#include <algorithm>
#include "regionrenderer.hpp"

using namespace core;

BasicRegion BasicRegion::clone() const
{
    BasicRegion copy;
    copy.points = getPoints();
    copy.lines = getLines();

    for (auto& line : copy.lines)
    {
        for (const auto& point : copy.points)
        {
            if (*(line.p1) == point) line.p1 = &point;
            if (*(line.p2) == point) line.p2 = &point;
        }
    }

    return copy;
}

Point& core::BasicRegion::appendPoint(int x, int y)
{
    points.push_back(Point(x, y));
    return points.back();
}

void core::BasicRegion::removePoint(const Point& point)
{
    auto iter = std::find(points.begin(), points.end(), point);
    if (iter != points.end())
    {
        lines.erase(std::remove_if(lines.begin(), lines.end(),
            [&point](const Line& line)
            { return *(line.p1) == point || *(line.p2) == point; }),
            lines.end());
        points.erase(iter);
    }
}

bool core::BasicRegion::pointsConnected(const Point& p1, const Point& p2) const
{
    Line connection(p1, p2);
    for (const auto& line : lines)
        if (line == connection)
            return true;
    return false;
}

void core::BasicRegion::connectPoints(const Point& p1, const Point& p2)
{
    if (!pointsConnected(p1, p2))
        lines.push_back(Line(p1, p2));
}

void core::BasicRegion::removeConnection(const Line& connection)
{
    auto iter = std::find(lines.begin(), lines.end(), connection);
    lines.erase(iter);
}

void core::BasicRegion::clear()
{
    lines.clear();
    points.clear();
}

QRectF BasicRegion::getBoundingBox() const
{
    QRectF rect(points.front().x, points.front().y, 0, 0);

    for (const auto& point : points)
    {
        rect.setLeft(std::min(rect.left(), qreal(point.x)));
        rect.setRight(std::max(rect.right(), qreal(point.x)));
        rect.setTop(std::min(rect.top(), qreal(point.y)));
        rect.setBottom(std::max(rect.bottom(), qreal(point.y)));
    }

    return rect;
}

bool core::BasicRegion::isConvex() const
{
    if (lines.size() < 3)
        return false;

    int cross = 0;

    for (auto prev = lines.begin(), curr = lines.begin();
        ++curr != lines.end(); prev++)
    {
        if (cross == 0)
            cross = prev->cross(*curr) > 0 ? 1 : -1;
        else if (prev->cross(*curr) * cross < 0)
            return false;
    }

    return true;
}

std::list<std::list<Line>> core::BasicRegion::getContours() const
{
    std::list<std::list<Line>> contours;
    std::list<Line> contour;
    std::list<Point> pointsVisited;

    const Point* currPoint = &points.front();

    while (true)
    {
        bool added = false;

        for (const auto& line : getLinesConnected(*currPoint))
        {
            if (std::find(pointsVisited.begin(), pointsVisited.end(), *line.p2) == pointsVisited.end())
            {
                contour.push_back(line);
                pointsVisited.push_back(*currPoint);
                currPoint = line.p2;
                added = true;
                break;
            }
            else if (contour.size() > 1 && *contour.front().p1 == *line.p2)
            {
                contour.push_back(line);
                pointsVisited.push_back(*currPoint);
                break;
            }
        }

        if (!added)
            break;
    }

    contours.push_back(contour);
    return contours;
}

std::list<Line> core::BasicRegion::getLinesConnected(const Point& p) const
{
    std::list<Line> result;

    for (const auto& line : lines)
    {
        if (*line.p1 == p)
            result.push_back(line);
        else if (*line.p2 == p)
            result.push_back(line.reversed());
    }

    return result;
}

core::Line::operator Segment() const
{
    Segment segment;
    segment.x1 = p1->x; segment.y1 = p1->y;
    segment.x2 = p2->x; segment.y2 = p2->y;
    return segment;
}

bool core::Line::operator==(const Line& line) const
{
    return *p1 == *(line.p1) && *p2 == *(line.p2) || *p1 == *(line.p2) && *p2 == *(line.p1);
}

bool core::Line::operator!=(const Line& line) const
{
    return (*p1 != *(line.p1) || *p2 != *(line.p2)) && (*p1 != *(line.p2) || *p2 != *(line.p1));
}

bool core::Line::closeTo(double x, double y, double offset) const
{
    return getDistance(x, y) < offset;
}

double core::Line::getDistance(double x, double y) const
{
    double v_x = p2->x - p1->x;
    double v_y = p2->y - p1->y;

    double w1_x = p1->x - x;
    double w1_y = p1->y - y;

    double w2_x = p2->x - x;
    double w2_y = p2->y - y;

    if (w1_x * v_x + w1_y * v_y >= 0)
        return p1->distance(x, y);

    else if (w2_x * v_x + w2_y * v_y <= 0)
        return p2->distance(x, y);

    else
        return abs(w1_x * w2_y - w2_x * w1_y) / p1->distance(*p2);
}

double core::Line::cross(const Line& line) const
{
    double dx1 = p2->x - p1->x;
    double dy1 = p2->y - p1->y;
    double dx2 = line.p2->x - line.p1->x;
    double dy2 = line.p2->y - line.p1->y;

    return dx1 * dy2 - dy1 * dx2;
}

bool core::Point::operator==(const Point& p) const
{
    return x == p.x && y == p.y;
}

bool core::Point::operator!=(const Point& p) const
{
    return x != p.x || y != p.y;
}

double core::Point::distance(int x, int y) const
{
    return hypot(this->x - x, this->y - y);
}

double core::Point::distance(const Point& p) const
{
    return hypot(x - p.x, y - p.y);
}

core::Vector::Vector(double dx, double dy)
    : dx(dx), dy(dy)
{
}

core::Vector::Vector(const Point& p1, const Point& p2)
    : dx(p2.x - p1.x), dy(p2.y - p1.y)
{
}

core::Vector::Vector(const Segment& seg)
    : dx(seg.x2 - seg.x1), dy(seg.y2 - seg.y1)
{
}

double core::Vector::getLength() const
{
    return std::hypot(dx, dy);
}

void core::Vector::setLength(double len)
{
    len /= getLength();
    dx *= len;
    dy *= len;
}

core::Vector::operator Segment() const
{
    Segment seg;

    seg.x1 = 0;
    seg.y1 = 0;
    seg.x2 = dx;
    seg.y2 = dy;

    return seg;
}

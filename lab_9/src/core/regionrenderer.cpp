#pragma once

#include <algorithm>
#include <set>
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

Point& core::BasicRegion::appendPoint(const Point& p)
{
    points.push_back(p);
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
    const Point& rp1 = getPointRef(p1);
    const Point& rp2 = getPointRef(p2);
    Line connection(rp1, rp2);
    for (const auto& line : lines)
        if (line == connection)
            return true;
    return false;
}

void core::BasicRegion::connectPoints(const Point& p1, const Point& p2)
{
    const Point& rp1 = getPointRef(p1);
    const Point& rp2 = getPointRef(p2);
    if (!pointsConnected(rp1, rp2))
        lines.push_back(Line(rp1, rp2));
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

bool core::BasicRegion::selfIntersecting() const
{
    for (auto line1 = lines.begin(); ++line1-- != lines.end(); ++line1)
        for (auto line2 = ++line1--; line2 != lines.end(); ++line2)
            if (line1->intersects(*line2))
                return true;

    return false;
}

static double contourArea(const std::list<Line>& contour)
{
    double area = 0.0;

    for (const auto& line : contour)
        area += line.p1->x * line.p2->y - line.p2->x * line.p1->y;

    return std::abs(area) / 2.0;
}

static bool isInside(const Point& target, const std::list<Line>& contour)
{
    int count = 0;

    for (const auto& line : contour)
    {
        if ((line.p1->y - target.y) * (line.p2->y - target.y) < 0)
        {
            double dx = line.p2->x - line.p1->x;
            double dy = line.p2->y - line.p1->y;

            double x = line.p1->x + dx / dy * (target.y - line.p1->y);

            if (target.x <= x)
                count++;
        }
    }

    return count % 2 == 1;
}

static bool isInside(const std::list<Line>& inner, const std::list<Line>& outer)
{
    return isInside(*inner.front().p1, outer);
}

static bool isOuterContour(const std::list<Line>& contour)
{
    double sum = 0.0;
    for (const auto& line : contour)
        sum += line.p1->x * line.p2->y - line.p2->x * line.p1->y;
    return sum > 0.0;
}

static void invertContour(std::list<Line>& contour)
{
    for (auto& line : contour)
        line.reverse();
    std::reverse(contour.begin(), contour.end());
}

static void normalizeContours(std::list<std::list<Line>>& contours)
{
    for (auto iter = contours.begin(); iter != contours.end(); iter++)
    {
        int count = 0;

        for (auto temp = contours.begin(); temp != contours.end(); temp++)
        {
            if (iter == temp)
                continue;

            if (isInside(*iter, *temp))
                count++;
        }

        if ((count % 2 == 0) ^ isOuterContour(*iter))
            invertContour(*iter);
    }
}

std::list<std::list<Line>> core::BasicRegion::getContours() const
{
    std::list<std::list<Line>> contours;
    std::set<Point> pointsVisited;

    while (pointsVisited.size() < points.size())
    {
        for (const auto& point : points)
        {
            if (pointsVisited.count(point) == 0)
            {
                auto contour = getContourWith(point);
                for (const auto& line : contour)
                    pointsVisited.insert(*line.p1);
                contours.push_back(contour);
                break;
            }
        }
    }

    normalizeContours(contours);
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

std::list<Line> core::BasicRegion::getContourWith(const Point& p) const
{
    std::list<Line> contour;

    std::set<Point> pointsVisited;
    const Point* currPoint = &getPointRef(p);

    bool contourClosed = false;
    bool nextFounded = true;
    while (!contourClosed && nextFounded)
    {
        nextFounded = false;
        for (const auto& line : getLinesConnected(*currPoint))
        {
            const Point* nextPoint = line.p2;

            if (pointsVisited.count(*nextPoint) == 0)
            {
                contour.push_back(line);
                pointsVisited.insert(*currPoint);
                currPoint = nextPoint;
                nextFounded = true;
                break;
            }
            else if (contour.size() > 1 && *contour.front().p1 == *nextPoint)
            {
                contour.push_back(line);
                contourClosed = true;
                break;
            }
        }
        pointsVisited.insert(*currPoint);
    }

    if (!contourClosed)
        __debugbreak(); // contour not closed!!

    return contour;
}

const Point& core::BasicRegion::getPointRef(const Point& p) const
{
    for (const auto& point : points)
        if (point == p)
            return point;
    return p;
}

double core::Line::getA() const
{
    return (p2->y - p1->y) / length();
}

double core::Line::getB() const
{
    return (p1->x - p2->x) / length();
}

double core::Line::getC() const
{
    return ((p2->x - p1->x) * p1->y - (p2->y - p1->y) * p1->x) / length();
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

bool core::Line::intersects(const Line& line) const
{
    double p11 = line.getA() * p1->x + line.getB() * p1->y + line.getC();
    double p12 = line.getA() * p2->x + line.getB() * p2->y + line.getC();

    double p21 = getA() * line.p1->x + getB() * line.p1->y + getC();
    double p22 = getA() * line.p2->x + getB() * line.p2->y + getC();

    return p11 * p12 > 0 || p21 * p22 > 0;
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

bool core::Point::operator<(const Point& p) const
{
    return y == p.y ? x < p.x : y < p.y;
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

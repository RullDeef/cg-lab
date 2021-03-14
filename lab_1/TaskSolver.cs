using System;
using System.Globalization;
using System.Collections.Generic;

namespace lab_1
{
    public struct Point
    {
        public const double EPS = 1e-3;
        public const double EPS_2 = EPS * EPS;

        public int id { get; set; }
        public double x { get; set; }
        public double y { get; set; }

        public Point(double x, double y)
        {
            id = 0;
            this.x = x;
            this.y = y;
        }

        public static bool IsAlmostSame(Point p1, Point p2)
        {
            double dx = p1.x - p2.x;
            double dy = p1.y - p2.y;
            return dx * dx + dy * dy < EPS_2;
        }

        public override string ToString()
        {
            return $"[{id}] ({x.ToString(CultureInfo.InvariantCulture)}; {y.ToString(CultureInfo.InvariantCulture)})";
        }
    }

    public struct Circle
    {
        public Point center;
        public double radius;

        public Circle(Point c, double r)
        {
            center = c;
            radius = r;
        }
    }

    public struct Line
    {
        public const double ANGLE_EPS = 1.0 / 180 * Math.PI;

        public Point p1, p2;
        public double a, b, c;
        public double dFactor;

        public Line(Point p1, Point p2)
        {
            this.p1 = p1;
            this.p2 = p2;

            double dx = p2.x - p1.x;
            double dy = p2.y - p1.y;

            a = dy;
            b = -dx;
            c = p1.y * dx - p1.x * dy;

            dFactor = Math.Sqrt(a * a + b * b);
        }

        public double GetAngleXAxis()
        {
            double dx = -b;
            double dy = a;

            double angle = Math.Abs(Math.Atan2(dy, dx));
            if (angle > Math.PI / 2)
                angle = Math.PI - angle;

            return angle;
        }

        public bool IsPointOnLine(Point p1)
        {
            double dist = Math.Abs(a * p1.x + b * p1.y + c) / dFactor;
            return dist < Point.EPS;
        }

        public static bool IsOnOneLine(Point p1, Point p2, Point p3)
        {
            bool result = Point.IsAlmostSame(p1, p2)
                || Point.IsAlmostSame(p1, p3)
                || Point.IsAlmostSame(p2, p3);

            if (!result)
                result = new Line(p1, p2).IsPointOnLine(p3);

            return result;
        }
    }

    public struct Triangle
    {
        public Point[] verticies;

        public Triangle(Point point1, Point point2, Point point3)
        {
            verticies = new Point[3] { point1, point2, point3 };
        }

        public Triangle(Line line, Point point)
        {
            verticies = new Point[3];
            verticies[0] = point;
            verticies[1] = line.p1;
            verticies[2] = line.p2;
        }
    }

    public class TaskSolution
    {
        public bool founded = false;
        public Line line;
        public Triangle triangle;
    }

    public class TaskSolver
    {
        private readonly List<Point> points = new List<Point>();
        private Circle circle;

        public TaskSolver() { }

        public void SetCircle(double cx, double cy, double r)
        {
            circle = new Circle(new Point(cx, cy), r);
        }

        public void AddPoint(Point point)
        {
            points.Add(point);
        }

        public TaskSolution Solve()
        {
            Triangle? bestTriangle = null;
            Line? bestLine = null;
            double bestAngle = Math.PI;

            foreach ((int i, int j, int k) in Iterate3Indecies(points.Count))
            {
                if (!Line.IsOnOneLine(points[i], points[j], points[k])
                    && CheckoutTargetLine(out Line line, points[i], points[j], points[k]))
                {
                    double angle = line.GetAngleXAxis();

                    if (angle < bestAngle)
                    {
                        bestLine = line;
                        bestAngle = angle;
                        bestTriangle = new Triangle(points[i], points[j], points[k]);
                    }
                }
            }

            TaskSolution solution = new TaskSolution();
            if (bestTriangle.HasValue)
            {
                solution.triangle = bestTriangle.Value;
                solution.line = bestLine.Value;
                solution.founded = true;
            }

            return solution;
        }

        private IEnumerable<(int, int, int)> Iterate3Indecies(int max)
        {
            for (int i = 0; i + 2 < max; i++)
                for (int j = i + 1; j + 1 < max; j++)
                    for (int k = j + 1; k < max; k++)
                        yield return (i, j, k);
        }

        private bool CheckoutTargetLine(out Line line, Point p1, Point p2, Point p3)
        {
            line = new Line(p1, p2);
            if (!line.IsPointOnLine(circle.center))
            {
                line = new Line(p1, p3);
                if (!line.IsPointOnLine(circle.center))
                {
                    line = new Line(p2, p3);
                    if (!line.IsPointOnLine(circle.center))
                        return false;
                }
            }

            return true;
        }
    }
}

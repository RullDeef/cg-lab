using System;
using System.Collections.Generic;

namespace lab_1
{
    class SolutionFormatter
    {
        public static string Format(Circle circle, List<Point> points, TaskSolution solution)
        {
            if (points.Count < 3)
                return SwithOnPointsCount(points);

            else if (!solution.founded)
                return "Ни одна пара точек не образует прямую, проходящую через центр окружности. Как следствие, построить искомый треугольник невозможно.";

            else
                return "Найден треугольник со стороной, которая лежит на прямой, проходящей через центр "
                    + "окружности и образующей минимальный угол с осью абсцисс: "
                    + $"{Math.Round(solution.line.GetAngleXAxis() * 180 / Math.PI)} градусов.\n"
                    + "Точки, образующие искомый треугольник:\n"
                    + $"{solution.triangle.verticies[0]}\n"
                    + $"{solution.triangle.verticies[1]}\n"
                    + $"{solution.triangle.verticies[2]}";
        }

        private static string SwithOnPointsCount(List<Point> points)
        {
            if (points.Count == 0)
                return "Множество точек пусто.";
            else if (points.Count == 1)
                return $"Множество точек состоит всего из одной точки.";

            return $"Множество точек состоит всего из двух точек.";
        }
    }
}

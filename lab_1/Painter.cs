using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace lab_1
{
    public class Painter
    {
        private PictureBox picture;
        private Graphics graphics;

        // rendering boundings
        private RectangleF boundingRect;
        private const float OFFSET = 16; // px

        // drawing pens
        private readonly Pen axisPen = new Pen(Color.DarkSlateGray, 1)
        {
            DashStyle = DashStyle.Dash
        };

        private readonly Pen linePen = new Pen(Color.BlueViolet, 1);
        private readonly SolidBrush pointBrush = new SolidBrush(Color.DarkBlue);
        private readonly SolidBrush textBrush = new SolidBrush(Color.Crimson);

        private readonly Pen circlePen = new Pen(Color.DarkOrange, 1);
        private readonly SolidBrush circleBrush = new SolidBrush(Color.DarkOrange);

        public Painter(PictureBox picture, Graphics graphics)
        {
            this.picture = picture;
            this.graphics = graphics;
        }

        public void PaintSolution(Circle circle, List<Point> points, TaskSolution solution)
        {
            graphics.Clear(Color.White);
            SetupBoundingBox(circle, points);

            DrawAxis();

            DrawCircle(circle);

            if (solution.founded)
            {
                DrawLine(solution.line);
                DrawTriangle(solution.triangle);
            }

            foreach (Point point in points)
                DrawPoint(point);
        }

        private void DrawAxis()
        {
            Point center = TransformPoint(new Point(0, 0));

            graphics.DrawLine(axisPen, (float)center.x, OFFSET, (float)center.x, graphics.ClipBounds.Height - OFFSET);
            graphics.DrawLine(axisPen, OFFSET, (float)center.y, graphics.ClipBounds.Width - OFFSET, (float)center.y);
        }

        private void DrawPoint(Point point)
        {
            Point dest = TransformPoint(point);

            Font font = new Font(FontFamily.GenericSansSerif, 8);
            string text = point.ToString();
            SizeF size = graphics.MeasureString(text, font);

            graphics.FillRectangle(new SolidBrush(Color.Bisque), (float)dest.x, (float)dest.y, size.Width, size.Height);
            graphics.FillEllipse(pointBrush, (float)dest.x - 2.5f, (float)dest.y - 2.5f, 5, 5);
            graphics.DrawString(text, font, textBrush, (float)dest.x, (float)dest.y);
        }

        private void DrawCircle(Circle circle)
        {
            circle = TransformCircle(circle);

            graphics.DrawEllipse(circlePen,
                (float)(circle.center.x - circle.radius),
                (float)(circle.center.y - circle.radius),
                (float)circle.radius * 2, (float)circle.radius * 2);

            graphics.FillEllipse(circleBrush, (float)circle.center.x - 2.5f, (float)circle.center.y - 2.5f, 5, 5);
        }

        private void DrawLine(Line line)
        {
            line.p1 = TransformPoint(line.p1);
            line.p2 = TransformPoint(line.p2);

            double dx = line.p2.x - line.p1.x;
            double dy = line.p2.y - line.p1.y;
            double dLen = Math.Sqrt(dx * dx + dy * dy);

            dx /= dLen;
            dy /= dLen;

            double largeDx = 2000 * dx;
            double largeDy = 2000 * dy;

            graphics.DrawLine(linePen,
                (float)(line.p1.x - largeDx), (float)(line.p1.y - largeDy),
                (float)(line.p2.x + largeDx), (float)(line.p2.y + largeDy));
        }

        private void DrawTriangle(Triangle triangle)
        {
            DrawLine(triangle.verticies[0], triangle.verticies[1]);
            DrawLine(triangle.verticies[0], triangle.verticies[2]);
            DrawLine(triangle.verticies[1], triangle.verticies[2]);
        }

        private void DrawLine(Point p1, Point p2)
        {
            p1 = TransformPoint(p1);
            p2 = TransformPoint(p2);

            graphics.DrawLine(linePen, (float)p1.x, (float)p1.y, (float)p2.x, (float)p2.y);
            graphics.FillEllipse(pointBrush, (float)p1.x - 2.5f, (float)p1.y - 2.5f, 5, 5);
            graphics.FillEllipse(pointBrush, (float)p2.x - 2.5f, (float)p2.y - 2.5f, 5, 5);
        }

        private void SetupBoundingBox(Circle circle, List<Point> points)
        {
            double left = circle.center.x - circle.radius;
            double top = circle.center.y - circle.radius;
            double right = circle.center.x + circle.radius;
            double bottom = circle.center.y + circle.radius;

            for (int i = 0; i < points.Count; i++)
            {
                Point point = points[i];

                left = Math.Min(left, point.x);
                top = Math.Min(top, point.y);
                right = Math.Max(right, point.x);
                bottom = Math.Max(bottom, point.y);
            }

            double width = right - left;
            double height = bottom - top;
            boundingRect = new RectangleF((float)left, (float)top, (float)width, (float)height);
        }

        private Circle TransformCircle(Circle circle)
        {
            circle.radius *= ComputeScalingFactor();
            circle.center = TransformPoint(circle.center);

            return circle;
        }

        // fit the point inside the actual image
        private Point TransformPoint(Point p)
        {
            double xm = graphics.VisibleClipBounds.Width / 2;
            double ym = graphics.VisibleClipBounds.Height / 2;

            double xc = boundingRect.X + boundingRect.Width / 2;
            double yc = boundingRect.Y + boundingRect.Height / 2;

            double k = ComputeScalingFactor();

            p.x = xm + (p.x - xc) * k;
            p.y = ym - (p.y - yc) * k;

            return p;
        }

        private double ComputeScalingFactor()
        {
            double kx = (graphics.VisibleClipBounds.Width - 2 * OFFSET) / boundingRect.Width;
            double ky = (graphics.VisibleClipBounds.Height - 2 * OFFSET) / boundingRect.Height;

            return Math.Min(kx, ky);
        }
    }
}

using System;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace lab_2
{
    public class FigurePainter
    {
        // drawing params
        private static readonly float BOX_SIDE = 1.0f;
        private static readonly float ARC_HEIGHT = 0.25f;

        private static readonly float UNIT_PX = 48.0f;

        // default transform matrix
        private static Matrix defaultMatrix = new Matrix(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);

        public static void SetupViewport(float width, float height)
        {
            defaultMatrix = new Matrix(UNIT_PX, 0.0f, 0.0f, -UNIT_PX, width / 2, height / 2);
        }

        public static void PaintAxes(Graphics graphics)
        {
            GraphicsState state = graphics.Save();
            graphics.MultiplyTransform(defaultMatrix);

            RectangleF bounds = graphics.ClipBounds;

            Pen pen = new Pen(Color.DimGray, TransformPenWidth(graphics, 2));
            Brush brush = new SolidBrush(Color.DimGray);

            float width = 14.0f / UNIT_PX;
            float height = 20.0f / UNIT_PX;

            graphics.DrawLine(pen, bounds.Left, 0.0f,  bounds.Right, 0.0f);
            graphics.DrawLine(pen, 0.0f, bounds.Top, 0.0f, bounds.Bottom);

            PointF[] arrowY = new PointF[]
            {
                new PointF(0.0f, bounds.Bottom),
                new PointF(width / 2, bounds.Bottom - height),
                new PointF(-width / 2, bounds.Bottom - height)
            };

            PointF[] arrowX = new PointF[]
            {
                new PointF(bounds.Right, 0.0f),
                new PointF(bounds.Right - height,  width / 2),
                new PointF(bounds.Right - height, -width / 2)
            };

            graphics.FillPolygon(brush, arrowX);
            graphics.DrawPolygon(pen, arrowX);

            graphics.FillPolygon(brush, arrowY);
            graphics.DrawPolygon(pen, arrowY);

            graphics.Restore(state);
        }

        public static void PaintGrid(Graphics graphics)
        {
            GraphicsState state = graphics.Save();
            graphics.MultiplyTransform(defaultMatrix);

            RectangleF bounds = graphics.ClipBounds;

            Pen pen = new Pen(Color.LightSlateGray, TransformPenWidth(graphics, 1));

            const float size = 1.0f; // 40.0f;

            for (float x = 0.0f; x > bounds.Left; x -= size)
            {
                for (float y = 0.0f; y > bounds.Top; y -= size)
                {
                    graphics.DrawLine(pen, x, bounds.Top, x, bounds.Bottom);
                    graphics.DrawLine(pen, -x, bounds.Top, -x, bounds.Bottom);

                    graphics.DrawLine(pen, bounds.Left, y, bounds.Right, y);
                    graphics.DrawLine(pen, bounds.Left, -y, bounds.Right, -y);
                }
            }

            graphics.Restore(state);
        }

        public static void PaintTransform(Graphics graphics, Matrix transform, Color color)
        {
            GraphicsState state = graphics.Save();
            graphics.MultiplyTransform(defaultMatrix, MatrixOrder.Prepend);
            graphics.MultiplyTransform(transform, MatrixOrder.Prepend);

            DefaultPaint(graphics, new Pen(Color.White, TransformPenWidth(graphics, 4)));
            DefaultPaint(graphics, new Pen(color, TransformPenWidth(graphics, 2)));

            graphics.Restore(state);
        }

        // paint in default configuration
        private static void DefaultPaint(Graphics graphics, Pen pen)
        {
            // draw boxes
            graphics.TranslateTransform(-BOX_SIDE / 2, 0.0f);
            graphics.RotateTransform(60.0f);

            graphics.DrawRectangle(pen, 0.0f, 0.0f, BOX_SIDE, BOX_SIDE);

            graphics.RotateTransform(-60.0f);
            graphics.TranslateTransform(BOX_SIDE, 0.0f);
            graphics.RotateTransform(30.0f);

            graphics.DrawRectangle(pen, 0.0f, 0.0f, BOX_SIDE, BOX_SIDE);

            graphics.RotateTransform(-30.0f);
            graphics.TranslateTransform(-BOX_SIDE / 2, 0.0f);

            // draw connection line
            graphics.DrawLine(pen, -BOX_SIDE / 2, 0.0f, BOX_SIDE / 2, 0.0f);

            // draw circle
            float height = BOX_SIDE * (float)Math.Sin(Math.PI / 3);
            graphics.DrawEllipse(pen, -height / 3, 0.0f, 2 * height / 3, 2 * height / 3);
            graphics.DrawArc(pen, -BOX_SIDE / 2, -ARC_HEIGHT, BOX_SIDE, 2 * ARC_HEIGHT, 180.0f, 180.0f);
        }

        private static float TransformPenWidth(Graphics graphics, float width)
        {
            float[] data = graphics.Transform.Elements;

            float scaleX = (float)Math.Sqrt(data[0] * data[0] + data[2] * data[2]);
            float scaleY = (float)Math.Sqrt(data[1] * data[1] + data[3] * data[3]);

            float widthX = Math.Abs(width / scaleX);
            float widthY = Math.Abs(width / scaleY);

            return Math.Max(widthX, widthY);
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing.Drawing2D;
using System.Drawing;

namespace lab_2
{
    public class TransformRecord
    {
        public Matrix matrix { get; private set; }
        public string text { get; private set; }
        public Color color { get; private set; }
        public bool enabled { get; set; }

        public TransformRecord()
        {
            matrix = new Matrix();
            text = "Начальное положение";
            color = HueToRgb(0.0);
            enabled = true;
        }

        public static TransformRecord Translation(TransformRecord prevRecord, float dx, float dy)
        {
            TransformRecord record = new TransformRecord
            {
                text = FormattableString.Invariant($"Перенос ({dx}, {dy})"),
                matrix = prevRecord.matrix.Clone(),
                color = GetNextColor(prevRecord.color)
            };

            record.matrix.Translate(dx, dy, MatrixOrder.Append);
            return record;
        }

        public static TransformRecord Rotation(TransformRecord prevRecord, float x, float y, float angle)
        {
            TransformRecord record = new TransformRecord
            {
                text = FormattableString.Invariant($"Поворот из точки ({x}, {y}) на угол {angle:N0}°"),
                matrix = prevRecord.matrix.Clone(),
                color = GetNextColor(prevRecord.color)
            };

            record.matrix.RotateAt(angle, new PointF(x, y), MatrixOrder.Append);
            return record;
        }

        public static TransformRecord Scale(TransformRecord prevRecord, float x, float y, float kx, float ky)
        {
            TransformRecord record = new TransformRecord
            {
                text = FormattableString.Invariant($"Масштабирование из ({x}, {y}) в {kx} и {ky} раз"),
                matrix = prevRecord.matrix.Clone(),
                color = GetNextColor(prevRecord.color)
            };

            record.matrix.Translate(-x, -y, MatrixOrder.Append);
            record.matrix.Scale(kx, ky, MatrixOrder.Append);
            record.matrix.Translate(x, y, MatrixOrder.Append);
            return record;
        }

        private static Color GetNextColor(Color prev)
        {
            double r = prev.R / 255.0, g = prev.G / 255.0, b = prev.B / 255.0;
            double hue = Math.Atan2(Math.Sqrt(3) * (g - b), 2 * r - g - b);

            hue += 2 * Math.PI * 5 / 31;

            return HueToRgb(hue);
        }

        private static Color HueToRgb(double hue)
        {
            while (hue < 0.0)  hue += 2 * Math.PI;
            while (hue >= 360) hue -= 2 * Math.PI;

            double V = 0.65, S = 0.75;
            double R, G, B;

            double hf = hue / (Math.PI / 3);
            int i = (int)Math.Floor(hf);
            double f = hf - i;

            double pv = V * (1 - S);
            double qv = V * (1 - S * f);
            double tv = V * (1 - S * (1 - f));

            switch (i)
            {
                case 0: R = V; G = tv; B = pv; break;
                case 1: R = qv; G = V; B = pv; break;
                case 2: R = pv; G = V; B = tv; break;
                case 3: R = pv; G = qv; B = V; break;
                case 4: R = tv; G = pv; B = V; break;
                case 5: R = V; G = pv; B = qv; break;
                case 6: R = V; G = tv; B = pv; break;
                default: R = G = B = V; break;
            }

            return Color.FromArgb((int)(R * 255.0), (int)(G * 255.0), (int)(B * 255.0));
        }
    }
}

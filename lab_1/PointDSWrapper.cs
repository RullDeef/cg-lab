namespace lab_1
{
    public class PointDSWrapper
    {
        public int id { get; set; }
        public double x { get; set; }
        public double y { get; set; }

        public Point ToPoint()
        {
            return new Point(x, y) { id = id };
        }
    }
}

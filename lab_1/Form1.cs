using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Windows.Forms;

namespace lab_1
{
    public partial class Form1 : Form
    {
        private Circle circle = new Circle(new Point(0, 0), 1);
        private BindingList<PointDSWrapper> points = new BindingList<PointDSWrapper>();

        // solution object
        private TaskSolution taskSolution = new TaskSolution();

        private const string dataFilename = "points.csv";

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            textBox1.Text = circle.center.x.ToString();
            textBox2.Text = circle.center.y.ToString();
            textBox3.Text = circle.radius.ToString();

            // points input
            dataGridView1.DataSource = points;
            dataGridView1.Columns["id"].Width = dataGridView1.Width / 5 - 6;
            dataGridView1.Columns["id"].ReadOnly = true;
            dataGridView1.Columns["x"].Width = dataGridView1.Width * 2 / 5 - 6;
            dataGridView1.Columns["y"].Width = dataGridView1.Width * 2 / 5 - 6;

            // text output
            // image output
            pictureBox1.Paint += PaintSolution;
        }

        private void CircleX_Validating(object sender, CancelEventArgs e)
        {
            double x;
            if (!double.TryParse(textBox1.Text, out x))
            {
                e.Cancel = true;
                textBox1.Select(0, textBox1.Text.Length);
            }
            else
                circle.center.x = x;
        }

        private void CircleY_Validating(object sender, CancelEventArgs e)
        {
            double y;
            if (!double.TryParse(textBox2.Text, out y))
            {
                e.Cancel = true;
                textBox2.Select(0, textBox2.Text.Length);
            }
            else
                circle.center.y = y;
        }

        private void CircleR_Validating(object sender, CancelEventArgs e)
        {
            double r;
            if (!double.TryParse(textBox3.Text, out r) || r <= Point.EPS)
            {
                e.Cancel = true;
                textBox2.Select(0, textBox3.Text.Length);
            }
            else
                circle.radius = r;
        }

        private void PaintSolution(object sender, PaintEventArgs e)
        {
            Painter painter = new Painter(pictureBox1, e.Graphics);
            List<Point> points = this.points.ToList().ConvertAll(p => p.ToPoint());

            painter.PaintSolution(circle, points, taskSolution);
        }

        private void deletePointCtxOpt_Click(object sender, EventArgs e)
        {
            try
            {
                List<int> indecies = new List<int>();

                foreach (DataGridViewRow row in dataGridView1.SelectedRows)
                    indecies.Add(row.Index);

                indecies.Sort();
                indecies.Reverse();

                indecies.ForEach(points.RemoveAt);
            }
            catch (Exception exception)
            {
                Console.WriteLine(exception);
            }
        }

        private void SolveButtonClick(object sender, EventArgs e)
        {
            TaskSolver solver = new TaskSolver();
            solver.SetCircle(circle.center.x, circle.center.y, circle.radius);

            List<Point> pointList = points.ToList().ConvertAll(p => p.ToPoint());
            foreach (Point point in pointList)
                solver.AddPoint(point);

            taskSolution = solver.Solve();
            pictureBox1.Refresh();

            textOutput.Text = SolutionFormatter.Format(circle, pointList, taskSolution);
        }

        private void loadFileBtn_Click(object sender, EventArgs e)
        {
            if (!File.Exists(dataFilename))
                MessageBox.Show("Не удалось найти файла с данными \"points.csv\".",
                    "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
            else if (!LoadPointsFromFile())
                MessageBox.Show("Некорректный формат файла. Перепроверьте файл.",
                    "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private bool LoadPointsFromFile()
        {
            try
            {
                string fileContents = File.ReadAllText(dataFilename);
                string[] patches = fileContents.Split(',');

                if (patches.Length % 2 != 0)
                    return false;

                double[] numbers = patches.ToList().ConvertAll(p =>
                    double.Parse(p, CultureInfo.InvariantCulture)).ToArray();

                points.Clear();
                for (int i = 0, id = 1; i < numbers.Length; i += 2, id++)
                    points.Add(new PointDSWrapper() { id = id, x = numbers[i], y = numbers[i + 1] });

                return true;
            }
            catch (Exception e)
            {
                return false;
            }
        }

        private void aboutMenuOpt_Click(object sender, EventArgs e)
        {
            MessageBox.Show(TextResourses.About, "О программе", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void taskMenuOpt_Click(object sender, EventArgs e)
        {
            MessageBox.Show(TextResourses.Task, "Задание", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void dataGridView1_CellParsing(object sender, DataGridViewCellParsingEventArgs e)
        {
            if (double.TryParse(e.Value.ToString(), NumberStyles.Float, CultureInfo.InvariantCulture, out double result))
            {
                e.Value = result;
                e.ParsingApplied = true;

                UpdatePointsIndexing();
            }
            else
                e.ParsingApplied = false;
        }

        private void UpdatePointsIndexing()
        {
            for (int i = 0, id = 1; i < points.Count; i++, id++)
                points[i].id = id;
        }

        private void dataGridView1_DataError(object sender, DataGridViewDataErrorEventArgs e)
        {
            MessageBox.Show("Недопустимый ввод координаты.", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
            e.Cancel = true;
        }
    }
}

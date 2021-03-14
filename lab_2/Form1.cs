using System;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;
using System.Globalization;

namespace lab_2
{
    public partial class Form1 : Form
    {
        private readonly BindingList<TransformRecord> records = new BindingList<TransformRecord>();

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            FigurePainter.SetupViewport(pictureBox1.Width, pictureBox1.Height);
            pictureBox1.Paint += RedrawEvent;
            SetupDataGridView();

            AddTransformRecord(new TransformRecord());
            records.ListChanged += (_sender, _e) => { pictureBox1.Invalidate(); };
        }

        private void AddTransformRecord(TransformRecord record)
        {
            if (records.Count > 0)
                records[records.Count - 1].enabled = false;

            records.Add(record);
        }

        private void SetupDataGridView()
        {
            dataGridView1.DataSource = records;
            dataGridView1.Columns["matrix"].Visible = false;

            dataGridView1.Columns["text"].HeaderText = "Описание";
            dataGridView1.Columns["text"].Width = dataGridView1.Width - 4 - 48 - 96;

            dataGridView1.Columns["color"].HeaderText = "Цвет";
            dataGridView1.Columns["color"].Width = 48;
            dataGridView1.Columns["color"].HeaderCell.Style.Alignment = DataGridViewContentAlignment.MiddleCenter;

            dataGridView1.Columns["enabled"].HeaderText = "Показать";
            dataGridView1.Columns["enabled"].Width = 96;
            dataGridView1.Columns["enabled"].HeaderCell.Style.Alignment = DataGridViewContentAlignment.MiddleCenter;
        }

        private void RedrawEvent(object sender, PaintEventArgs e)
        {
            e.Graphics.Clear(Color.White);

            FigurePainter.PaintGrid(e.Graphics);
            FigurePainter.PaintAxes(e.Graphics);

            foreach (TransformRecord record in records)
                if (record.enabled)
                    FigurePainter.PaintTransform(e.Graphics, record.matrix, record.color);
        }

        private void dataGridView1_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
        {
            if (dataGridView1.Columns[e.ColumnIndex].Name == "color")
            {
                e.CellStyle.BackColor = (Color)e.Value;
                e.CellStyle.ForeColor = (Color)e.Value;
            }
        }

        private void dataGridView_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            pictureBox1.Invalidate();
        }

        private void TranslateButtonClick(object sender, EventArgs e)
        {
            if (PromptTranslation(out float dx, out float dy))
            {
                TransformRecord record = TransformRecord.Translation(records[records.Count - 1], dx, dy);
                AddTransformRecord(record);
            }
        }

        private void RotateButtonClick(object sender, EventArgs e)
        {
            if (PromptRotation(out float cx, out float cy, out float angle))
            {
                TransformRecord record = TransformRecord.Rotation(records[records.Count - 1], cx, cy, angle);
                AddTransformRecord(record);
            }
        }

        private void ScaleButtonClick(object sender, EventArgs e)
        {
            if (PromptScale(out float cx, out float cy, out float kx, out float ky))
            {
                TransformRecord record = TransformRecord.Scale(records[records.Count - 1], cx, cy, kx, ky);
                AddTransformRecord(record);
            }
        }

        private bool PromptTranslation(out float dx, out float dy)
        {
            PromptTranslateForm dialog = new PromptTranslateForm();
            float _dx = 0.0f, _dy = 0.0f;

            dialog.confirmButton.Click += (_sender, _e) =>
            {
                if (!float.TryParse(dialog.inputX.Text, NumberStyles.Float, CultureInfo.InvariantCulture, out _dx)
                    || !float.TryParse(dialog.inputY.Text, NumberStyles.Float, CultureInfo.InvariantCulture, out _dy))
                    MessageBox.Show("Смещения выражаются вещественными числами. Проверьте ввод.",
                        "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                else
                {
                    dialog.DialogResult = DialogResult.OK;
                    dialog.Close();
                }
            };

            DialogResult result = dialog.ShowDialog(this);

            dx = _dx;
            dy = _dy;
            return result == DialogResult.OK;
        }

        private bool PromptRotation(out float cx, out float cy, out float angle)
        {
            PromptRotateForm dialog = new PromptRotateForm();
            float _cx = 0.0f, _cy = 0.0f, _angle = 0.0f;

            dialog.confirmButton.Click += (_sender, _e) =>
            {
                if (!float.TryParse(dialog.inputX.Text, NumberStyles.Float, CultureInfo.InvariantCulture, out _cx)
                        || !float.TryParse(dialog.inputY.Text, NumberStyles.Float, CultureInfo.InvariantCulture, out _cy))
                    MessageBox.Show("Координаты цента выражаются вещественными числами. Проверьте ввод.",
                        "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                else if (!float.TryParse(dialog.inputAngle.Text, NumberStyles.Float, CultureInfo.InvariantCulture, out _angle))
                    MessageBox.Show("Угол поворота выражается вещественным числом. Проверьте ввод.",
                        "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                else
                {
                    dialog.DialogResult = DialogResult.OK;
                    dialog.Close();
                }
            };

            DialogResult result = dialog.ShowDialog(this);

            cx = _cx;
            cy = _cy;
            angle = _angle;
            return result == DialogResult.OK;
        }

        private bool PromptScale(out float cx, out float cy, out float kx, out float ky)
        {
            PromptScaleForm dialog = new PromptScaleForm();
            float _cx = 0.0f, _cy = 0.0f, _kx = 1.0f, _ky = 1.0f;

            dialog.confirmButton.Click += (_sender, _e) =>
            {
                if (!float.TryParse(dialog.inputCX.Text, NumberStyles.Float, CultureInfo.InvariantCulture, out _cx)
                        || !float.TryParse(dialog.inputCY.Text, NumberStyles.Float, CultureInfo.InvariantCulture, out _cy))
                    MessageBox.Show("Координаты цента выражаются вещественными числами. Проверьте ввод.",
                        "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                else if (!float.TryParse(dialog.inputKX.Text, NumberStyles.Float, CultureInfo.InvariantCulture, out _kx)
                        || !float.TryParse(dialog.inputKY.Text, NumberStyles.Float, CultureInfo.InvariantCulture, out _ky))
                    MessageBox.Show("Коэффициенты масштабирования выражаются вещественными числами. Проверьте ввод.",
                        "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                else if (_kx == 0.0f || _ky == 0.0f)
                    MessageBox.Show("Коэффициенты масштабирования не могут быть равны нулю.",
                        "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                else
                {
                    dialog.DialogResult = DialogResult.OK;
                    dialog.Close();
                }
            };

            DialogResult result = dialog.ShowDialog(this);

            cx = _cx;
            cy = _cy;
            kx = _kx;
            ky = _ky;
            return result == DialogResult.OK;
        }

        private void undoButton_Click(object sender, EventArgs e)
        {
            if (records.Count > 1)
            {
                records.RemoveAt(records.Count - 1);
                records[records.Count - 1].enabled = true;
            }
            else
            {
                MessageBox.Show("Все трансформации уже отменены.", "Ошибка", MessageBoxButtons.OK);
            }
        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            dataGridView1.CommitEdit(DataGridViewDataErrorContexts.Commit);
        }
    }
}


namespace lab_2
{
    partial class PromptRotateForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.confirmButton = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.inputX = new System.Windows.Forms.TextBox();
            this.inputY = new System.Windows.Forms.TextBox();
            this.inputAngle = new System.Windows.Forms.TextBox();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 56.69014F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 43.30986F));
            this.tableLayoutPanel1.Controls.Add(this.confirmButton, 0, 4);
            this.tableLayoutPanel1.Controls.Add(this.label1, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.label2, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.label3, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.label4, 0, 3);
            this.tableLayoutPanel1.Controls.Add(this.inputX, 1, 1);
            this.tableLayoutPanel1.Controls.Add(this.inputY, 1, 2);
            this.tableLayoutPanel1.Controls.Add(this.inputAngle, 1, 3);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 5;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 27.27273F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 17.44186F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 15.69767F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 16.86047F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 24.4186F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(473, 304);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // confirmButton
            // 
            this.confirmButton.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.tableLayoutPanel1.SetColumnSpan(this.confirmButton, 2);
            this.confirmButton.Location = new System.Drawing.Point(136, 246);
            this.confirmButton.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.confirmButton.Name = "confirmButton";
            this.confirmButton.Size = new System.Drawing.Size(200, 41);
            this.confirmButton.TabIndex = 0;
            this.confirmButton.Text = "Подтвердить";
            this.confirmButton.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label1.AutoSize = true;
            this.tableLayoutPanel1.SetColumnSpan(this.label1, 2);
            this.label1.Location = new System.Drawing.Point(44, 17);
            this.label1.Margin = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(385, 46);
            this.label1.TabIndex = 1;
            this.label1.Text = "Введите координаты центра поворота и угол поворота";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label2
            // 
            this.label2.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(40, 95);
            this.label2.Margin = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(188, 23);
            this.label2.TabIndex = 2;
            this.label2.Text = "Координата X центра";
            // 
            // label3
            // 
            this.label3.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(40, 144);
            this.label3.Margin = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(187, 23);
            this.label3.TabIndex = 2;
            this.label3.Text = "Координата Y центра";
            // 
            // label4
            // 
            this.label4.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(15, 192);
            this.label4.Margin = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(237, 23);
            this.label4.TabIndex = 2;
            this.label4.Text = "Угол поворота (в градусах)";
            // 
            // inputX
            // 
            this.inputX.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.inputX.Location = new System.Drawing.Point(288, 92);
            this.inputX.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.inputX.Name = "inputX";
            this.inputX.Size = new System.Drawing.Size(164, 30);
            this.inputX.TabIndex = 3;
            // 
            // inputY
            // 
            this.inputY.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.inputY.Location = new System.Drawing.Point(288, 141);
            this.inputY.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.inputY.Name = "inputY";
            this.inputY.Size = new System.Drawing.Size(164, 30);
            this.inputY.TabIndex = 3;
            // 
            // inputAngle
            // 
            this.inputAngle.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.inputAngle.Location = new System.Drawing.Point(288, 189);
            this.inputAngle.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.inputAngle.Name = "inputAngle";
            this.inputAngle.Size = new System.Drawing.Size(164, 30);
            this.inputAngle.TabIndex = 3;
            // 
            // PromptRotateForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(10F, 23F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(473, 304);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Font = new System.Drawing.Font("Candara", 14F);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.Name = "PromptRotateForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Поворот";
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        public System.Windows.Forms.TextBox inputX;
        public System.Windows.Forms.TextBox inputY;
        public System.Windows.Forms.TextBox inputAngle;
        private System.Windows.Forms.Label label1;
        public System.Windows.Forms.Button confirmButton;
    }
}
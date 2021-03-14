
namespace lab_2
{
    partial class PromptScaleForm
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
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.confirmButton = new System.Windows.Forms.Button();
            this.inputCX = new System.Windows.Forms.TextBox();
            this.inputCY = new System.Windows.Forms.TextBox();
            this.inputKX = new System.Windows.Forms.TextBox();
            this.inputKY = new System.Windows.Forms.TextBox();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 52.46479F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 47.53521F));
            this.tableLayoutPanel1.Controls.Add(this.label1, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.label2, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.label3, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.label4, 0, 3);
            this.tableLayoutPanel1.Controls.Add(this.label5, 0, 4);
            this.tableLayoutPanel1.Controls.Add(this.confirmButton, 0, 5);
            this.tableLayoutPanel1.Controls.Add(this.inputCX, 1, 1);
            this.tableLayoutPanel1.Controls.Add(this.inputCY, 1, 2);
            this.tableLayoutPanel1.Controls.Add(this.inputKX, 1, 3);
            this.tableLayoutPanel1.Controls.Add(this.inputKY, 1, 4);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 6;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 16.66667F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 16.66667F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 16.66667F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 16.66667F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 16.66667F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 16.66667F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(473, 419);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label1.AutoSize = true;
            this.tableLayoutPanel1.SetColumnSpan(this.label1, 2);
            this.label1.Location = new System.Drawing.Point(70, 23);
            this.label1.Margin = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(333, 23);
            this.label1.TabIndex = 0;
            this.label1.Text = "Введите параметры масштабирования";
            // 
            // label2
            // 
            this.label2.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(28, 80);
            this.label2.Margin = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(192, 46);
            this.label2.TabIndex = 0;
            this.label2.Text = "Координата Х центра масштабирования";
            // 
            // label3
            // 
            this.label3.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(28, 149);
            this.label3.Margin = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(191, 46);
            this.label3.TabIndex = 0;
            this.label3.Text = "Координата Y центра масштабирования";
            // 
            // label4
            // 
            this.label4.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(42, 218);
            this.label4.Margin = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(163, 46);
            this.label4.TabIndex = 0;
            this.label4.Text = "Коэффициент KX масштабирования";
            // 
            // label5
            // 
            this.label5.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(42, 287);
            this.label5.Margin = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(163, 46);
            this.label5.TabIndex = 0;
            this.label5.Text = "Коэффициент KY масштабирования";
            // 
            // confirmButton
            // 
            this.confirmButton.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.tableLayoutPanel1.SetColumnSpan(this.confirmButton, 2);
            this.confirmButton.Location = new System.Drawing.Point(136, 361);
            this.confirmButton.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.confirmButton.Name = "confirmButton";
            this.confirmButton.Size = new System.Drawing.Size(200, 41);
            this.confirmButton.TabIndex = 1;
            this.confirmButton.Text = "Подтвердить";
            this.confirmButton.UseVisualStyleBackColor = true;
            // 
            // inputCX
            // 
            this.inputCX.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.inputCX.Location = new System.Drawing.Point(278, 88);
            this.inputCX.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.inputCX.Name = "inputCX";
            this.inputCX.Size = new System.Drawing.Size(164, 30);
            this.inputCX.TabIndex = 2;
            // 
            // inputCY
            // 
            this.inputCY.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.inputCY.Location = new System.Drawing.Point(278, 157);
            this.inputCY.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.inputCY.Name = "inputCY";
            this.inputCY.Size = new System.Drawing.Size(164, 30);
            this.inputCY.TabIndex = 2;
            // 
            // inputKX
            // 
            this.inputKX.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.inputKX.Location = new System.Drawing.Point(278, 226);
            this.inputKX.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.inputKX.Name = "inputKX";
            this.inputKX.Size = new System.Drawing.Size(164, 30);
            this.inputKX.TabIndex = 2;
            // 
            // inputKY
            // 
            this.inputKY.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.inputKY.Location = new System.Drawing.Point(278, 295);
            this.inputKY.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.inputKY.Name = "inputKY";
            this.inputKY.Size = new System.Drawing.Size(164, 30);
            this.inputKY.TabIndex = 2;
            // 
            // PromptScaleForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(10F, 23F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(473, 419);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Font = new System.Drawing.Font("Candara", 14F);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.Name = "PromptScaleForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Масштабирование";
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        public System.Windows.Forms.TextBox inputCX;
        public System.Windows.Forms.TextBox inputCY;
        public System.Windows.Forms.TextBox inputKX;
        public System.Windows.Forms.TextBox inputKY;
        public System.Windows.Forms.Button confirmButton;
    }
}
namespace hello1
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.txt_msg = new System.Windows.Forms.TextBox();
            this.btn_recv = new System.Windows.Forms.Button();
            this.btn_send = new System.Windows.Forms.Button();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.SuspendLayout();
            // 
            // txt_msg
            // 
            this.txt_msg.Location = new System.Drawing.Point(41, 64);
            this.txt_msg.Name = "txt_msg";
            this.txt_msg.Size = new System.Drawing.Size(245, 21);
            this.txt_msg.TabIndex = 0;
            // 
            // btn_recv
            // 
            this.btn_recv.Location = new System.Drawing.Point(37, 151);
            this.btn_recv.Name = "btn_recv";
            this.btn_recv.Size = new System.Drawing.Size(61, 23);
            this.btn_recv.TabIndex = 1;
            this.btn_recv.Text = "recv";
            this.btn_recv.UseVisualStyleBackColor = true;
            this.btn_recv.Click += new System.EventHandler(this.btn_recv_Click);
            // 
            // btn_send
            // 
            this.btn_send.Location = new System.Drawing.Point(194, 150);
            this.btn_send.Name = "btn_send";
            this.btn_send.Size = new System.Drawing.Size(91, 23);
            this.btn_send.TabIndex = 2;
            this.btn_send.Text = "send";
            this.btn_send.UseVisualStyleBackColor = true;
            this.btn_send.Click += new System.EventHandler(this.btn_send_Click);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(333, 225);
            this.Controls.Add(this.btn_send);
            this.Controls.Add(this.btn_recv);
            this.Controls.Add(this.txt_msg);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox txt_msg;
        private System.Windows.Forms.Button btn_recv;
        private System.Windows.Forms.Button btn_send;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
    }
}


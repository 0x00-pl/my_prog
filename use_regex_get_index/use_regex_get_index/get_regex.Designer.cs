namespace use_regex_get_index
{
    partial class get_redex
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
            this.regex_out = new System.Windows.Forms.TextBox();
            this.url_list_src = new System.Windows.Forms.TextBox();
            this.def_regex = new System.Windows.Forms.ListBox();
            this.prefix = new System.Windows.Forms.Button();
            this.expland_range = new System.Windows.Forms.Button();
            this.set_url_pattern = new System.Windows.Forms.Button();
            this.add_url_from_txt = new System.Windows.Forms.Button();
            this.checkedList_url = new System.Windows.Forms.ListView();
            this.url = new System.Windows.Forms.ColumnHeader();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.update_urls = new System.Windows.Forms.Button();
            this.cut_regex = new System.Windows.Forms.Button();
            this.regex_show = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // regex_out
            // 
            this.regex_out.Location = new System.Drawing.Point(14, 23);
            this.regex_out.Name = "regex_out";
            this.regex_out.Size = new System.Drawing.Size(536, 21);
            this.regex_out.TabIndex = 0;
            this.regex_out.TextChanged += new System.EventHandler(this.regex_out_TextChanged);
            // 
            // url_list_src
            // 
            this.url_list_src.Location = new System.Drawing.Point(71, 175);
            this.url_list_src.Name = "url_list_src";
            this.url_list_src.Size = new System.Drawing.Size(750, 21);
            this.url_list_src.TabIndex = 1;
            this.url_list_src.TextChanged += new System.EventHandler(this.url_list_src_TextChanged);
            // 
            // def_regex
            // 
            this.def_regex.FormattingEnabled = true;
            this.def_regex.ItemHeight = 12;
            this.def_regex.Items.AddRange(new object[] {
            "/",
            "\\.",
            "\\d+",
            "\\w+",
            "[^\\.]+",
            ".+"});
            this.def_regex.Location = new System.Drawing.Point(318, 50);
            this.def_regex.Name = "def_regex";
            this.def_regex.Size = new System.Drawing.Size(232, 112);
            this.def_regex.TabIndex = 2;
            this.def_regex.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.def_regex_MouseDoubleClick);
            // 
            // prefix
            // 
            this.prefix.Location = new System.Drawing.Point(14, 50);
            this.prefix.Name = "prefix";
            this.prefix.Size = new System.Drawing.Size(65, 24);
            this.prefix.TabIndex = 4;
            this.prefix.Text = "生成前缀";
            this.prefix.UseVisualStyleBackColor = true;
            this.prefix.Click += new System.EventHandler(this.prefix_Click);
            // 
            // expland_range
            // 
            this.expland_range.Location = new System.Drawing.Point(85, 50);
            this.expland_range.Name = "expland_range";
            this.expland_range.Size = new System.Drawing.Size(98, 24);
            this.expland_range.TabIndex = 5;
            this.expland_range.Text = "扩大匹配范围";
            this.expland_range.UseVisualStyleBackColor = true;
            this.expland_range.Click += new System.EventHandler(this.expland_range_Click);
            // 
            // set_url_pattern
            // 
            this.set_url_pattern.Location = new System.Drawing.Point(85, 145);
            this.set_url_pattern.Name = "set_url_pattern";
            this.set_url_pattern.Size = new System.Drawing.Size(105, 24);
            this.set_url_pattern.TabIndex = 6;
            this.set_url_pattern.Text = "设置获取url模式";
            this.set_url_pattern.UseVisualStyleBackColor = true;
            this.set_url_pattern.Click += new System.EventHandler(this.set_url_pattern_Click);
            // 
            // add_url_from_txt
            // 
            this.add_url_from_txt.Location = new System.Drawing.Point(14, 145);
            this.add_url_from_txt.Name = "add_url_from_txt";
            this.add_url_from_txt.Size = new System.Drawing.Size(65, 24);
            this.add_url_from_txt.TabIndex = 7;
            this.add_url_from_txt.Text = "编辑列表";
            this.add_url_from_txt.UseVisualStyleBackColor = true;
            this.add_url_from_txt.Click += new System.EventHandler(this.add_url_from_txt_Click);
            // 
            // checkedList_url
            // 
            this.checkedList_url.CheckBoxes = true;
            this.checkedList_url.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.url});
            this.checkedList_url.Location = new System.Drawing.Point(14, 202);
            this.checkedList_url.Name = "checkedList_url";
            this.checkedList_url.Size = new System.Drawing.Size(807, 548);
            this.checkedList_url.TabIndex = 8;
            this.checkedList_url.UseCompatibleStateImageBehavior = false;
            this.checkedList_url.View = System.Windows.Forms.View.Details;
            // 
            // url
            // 
            this.url.Text = "url";
            this.url.Width = 1800;
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(556, 12);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(232, 157);
            this.textBox1.TabIndex = 9;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 178);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(59, 12);
            this.label1.TabIndex = 10;
            this.label1.Text = "目录网页:";
            // 
            // update_urls
            // 
            this.update_urls.Location = new System.Drawing.Point(196, 146);
            this.update_urls.Name = "update_urls";
            this.update_urls.Size = new System.Drawing.Size(66, 23);
            this.update_urls.TabIndex = 11;
            this.update_urls.Text = "刷新";
            this.update_urls.UseVisualStyleBackColor = true;
            this.update_urls.Click += new System.EventHandler(this.update_urls_Click);
            // 
            // cut_regex
            // 
            this.cut_regex.Location = new System.Drawing.Point(189, 50);
            this.cut_regex.Name = "cut_regex";
            this.cut_regex.Size = new System.Drawing.Size(90, 23);
            this.cut_regex.TabIndex = 12;
            this.cut_regex.Text = "去除最后一层";
            this.cut_regex.UseVisualStyleBackColor = true;
            this.cut_regex.Click += new System.EventHandler(this.cut_regex_Click);
            // 
            // regex_show
            // 
            this.regex_show.AutoSize = true;
            this.regex_show.Location = new System.Drawing.Point(17, 80);
            this.regex_show.Name = "regex_show";
            this.regex_show.Size = new System.Drawing.Size(65, 12);
            this.regex_show.TabIndex = 13;
            this.regex_show.Text = "regex_show";
            // 
            // get_redex
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(833, 768);
            this.Controls.Add(this.regex_show);
            this.Controls.Add(this.cut_regex);
            this.Controls.Add(this.update_urls);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.checkedList_url);
            this.Controls.Add(this.add_url_from_txt);
            this.Controls.Add(this.set_url_pattern);
            this.Controls.Add(this.expland_range);
            this.Controls.Add(this.prefix);
            this.Controls.Add(this.def_regex);
            this.Controls.Add(this.url_list_src);
            this.Controls.Add(this.regex_out);
            this.Name = "get_redex";
            this.Text = "get_regex";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox regex_out;
        private System.Windows.Forms.Button prefix;
        private System.Windows.Forms.Button expland_range;
        private System.Windows.Forms.Button set_url_pattern;
        private System.Windows.Forms.Button add_url_from_txt;
        private System.Windows.Forms.ListView checkedList_url;
        public System.Windows.Forms.TextBox textBox1;
        public System.Windows.Forms.TextBox url_list_src;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ColumnHeader url;
        private System.Windows.Forms.Button update_urls;
        private System.Windows.Forms.Button cut_regex;
        private System.Windows.Forms.ListBox def_regex;
        private System.Windows.Forms.Label regex_show;

    }
}


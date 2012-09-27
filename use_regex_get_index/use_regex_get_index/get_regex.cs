using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.Threading;
using System.Text.RegularExpressions;

namespace use_regex_get_index
{
    public partial class get_redex : Form
    {
        public get_redex()
        {
            InitializeComponent();
            thread_load_page = new thread_class(textBox1,checkedList_url);
        }

        private void add_url_from_txt_Click(object sender, EventArgs e)
        {
            string default_text = url_list_to_string(checkedList_url);

            input_text in_url = new input_text();
            in_url.textBox1.Text = default_text;
            in_url.ShowDialog();

            checkedList_url.Items.Clear();
            add_url_list(checkedList_url, in_url.textBox1.Text);
        }
        public string url_list_to_string(ListView urls_list) {
            string ret="";
            foreach (ListViewItem iter in urls_list.Items)
            {
                ret += iter.Text + "\r\n";
            }
            return ret;
        }
        public void add_url_list(ListView dest, string val) {
            string[] urls= val.Split('\n');
            ListViewItem[] to_addrange = new ListViewItem[urls.Length];
            for (int i = 0; i < urls.Length; i++)
            {
                string temp= urls[i].Trim();
                if (!string.IsNullOrEmpty(temp))
                    to_addrange[i] = new ListViewItem(temp);
                else
                    to_addrange[i] = new ListViewItem("");
            }
            checkedList_url.Items.AddRange(to_addrange);
        }
        private void prefix_Click(object sender, EventArgs e)
        {
            string[] urls= get_checked_list(checkedList_url);
            regex_out.Text = prefix_update(regex_out.Text,urls);
        }

        public string[] get_checked_list(ListView lv) {
            ArrayList urls = new ArrayList();
            for (int i = 0; i < lv.Items.Count; ++i)
            {
                if (lv.Items[i].Checked)
                    urls.Add(lv.Items[i].Text);
            }
            return (string[])urls.ToArray(typeof(string));
        }
        public string prefix_update(string from, string[] datas) {
            return base_func.find_same_prefix(datas, from);
        }

        public class thread_class: html_get_single_thread {
            public TextBox dest_txt;
            public ListView dest_list;
            public thread_class(TextBox _dest_txt, ListView _dest_list)
            {
                dest_txt = _dest_txt;
                dest_list = _dest_list;
            }
            public override void begin_update()
            {
                dest_txt.Text = "loading...";
            }
            public override void exception_update(Exception ex)
            {
                dest_txt.Text = ex.Message;
            }
            public class regex_pub: Regex{
                public regex_pub(string pattern):base(pattern) {  }
                public string get_puttern(){
                return pattern;
               }
            }
           // Regex url_pattern = new Regex("(http://)?([\\w-]+\\.)+[\\w-]+(/[\\w- ./?%&=]*)?");
            public regex_pub url_pattern = new regex_pub("(?<=href=\")[^\"]*(?=\")");
            public override void end_update(string html)
            {
                dest_txt.Text = html;
                MatchCollection urls = url_pattern.Matches(html);
                if (urls.Count > 0)
                {
                    dest_list.Items.Clear();
                    //foreach (Match math in urls)
                    //{
                    //    dest_list.Items.Add("url",math.Value,0);
                    //}
                    ListViewItem[] to_addrange=new ListViewItem[urls.Count];
                    for (int i = 0; i < urls.Count; i++) {
                        to_addrange[i] = new ListViewItem(urls[i].Value);
                    }
                    dest_list.Items.AddRange(to_addrange);
                }
            }
        }
        public thread_class thread_load_page = null;

        public void recolor_url_list() {
            try
            {
                Regex reg = new Regex("^" + regex_out.Text + "$");
                ListViewItem first_red = null;
                foreach (ListViewItem iter in checkedList_url.Items)
                {
                    int flag = 1;
                    if (reg.IsMatch(iter.Text))
                        flag *= -1;
                    if (iter.Checked) flag *= -1;

                    if (flag == -1)
                    {
                        iter.ForeColor = Color.Red;
                        if (first_red == null) first_red = iter;
                    }
                    else
                        iter.ForeColor = Color.Black;
                }
                if (first_red != null)
                {
                    checkedList_url.EnsureVisible(first_red.Index);


                    Regex get_tail = new Regex("^" + regex_out.Text + "(.*?)$");

                    Match tail_match = get_tail.Match(first_red.Text);
                    regex_show.Text = tail_match.Groups[tail_match.Groups.Count - 1].Value;

                }
            }
            catch (ArgumentException e)
            {
            }
        }


        //events
        private void url_list_src_TextChanged(object sender, EventArgs e)
        {
            thread_load_page.update_url(url_list_src.Text);
        }

        private void def_regex_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            int index = this.def_regex.IndexFromPoint(e.Location);
            if (index != System.Windows.Forms.ListBox.NoMatches)
            {
                regex_out.Text += this.def_regex.Items[index].ToString();
            }
        }

        private void set_url_pattern_Click(object sender, EventArgs e)
        {
            input_text input = new input_text();
            //TODO 
            input.textBox1.Text = thread_load_page.url_pattern.get_puttern();
            input.ShowDialog();

            thread_load_page.url_pattern = new thread_class.regex_pub(input.textBox1.Text);
        }

        private void expland_range_Click(object sender, EventArgs e)
        {
            regex_out.Text=base_func.expland_range(regex_out.Text);
        }

        private void update_urls_Click(object sender, EventArgs e)
        {
            recolor_url_list();
        }

        private void cut_regex_Click(object sender, EventArgs e)
        {
            if (regex_out.Text.LastIndexOf("/") == -1) { 
                regex_out.Text="";
                return; 
            }

            if (regex_out.Text.EndsWith("/"))
                regex_out.Text = regex_out.Text.Substring(0, regex_out.Text.Length - 1);
            else
                regex_out.Text = regex_out.Text.Substring(0,regex_out.Text.LastIndexOf("/")+1);
        }

        private void regex_out_TextChanged(object sender, EventArgs e)
        {
            recolor_url_list();
        }

    }
}

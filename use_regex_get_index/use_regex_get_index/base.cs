

using System.Net;
using System.IO;
using System.Text;
using System.Windows.Forms;
using System;
using System.Threading;
using System.Text.RegularExpressions;
using System.IO.Compression;
namespace use_regex_get_index
{
    static class base_func{
        public static string find_same_prefix(string[] vals, string prefix)
        {
            //input empty:  do nothing
            if (vals.Length == 0) return prefix;

            string ret = prefix;
            bool char_diff=false;
            while (true) {
                //get next char
                Regex next_char_rule;
                try
                {
                    next_char_rule = new Regex("^" + prefix + "(.)");
                }
                catch (Exception) {
                    return prefix;
                }
                Match next_char=next_char_rule.Match(vals[0]);
                if (next_char.Groups.Count == 0) {
                    //no more prefix, return
                    return ret;
                }
                string s_next_char = next_char.Groups[next_char.Groups.Count - 1].Value;
                if (string.IsNullOrEmpty(s_next_char))
                {
                    //no more prefix, return
                    return ret;
                }
                prefix += Regex.Escape(s_next_char);
                Regex rule= new Regex("^"+prefix);
                foreach(string iter in vals){
                    if (!starts_with_regex(iter,rule))
                        char_diff = true;
                }
                if (char_diff)
                {
                    //no more prefix, return
                    return ret;
                }
                ret = prefix;
            }
        }
        private static bool starts_with_regex(string str, Regex rule) {
            return rule.IsMatch(str);
        }
        public static string get_page(string url)
        {
            string ret = "";

            while (true)
            {
                try
                {
                    byte[] buf = new byte[65536];
                    HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
                    //request.Timeout = 50000;
                    request.BeginGetResponse(null,null);
                    while(!request.HaveResponse){
                        Thread.Sleep(100);
                        ret += "sleep(100) ";
                    }
                    HttpWebResponse response = (HttpWebResponse)request.GetResponse();
                    Stream resStream = response.GetResponseStream();

                    int count = resStream.Read(buf, 0, buf.Length);

                    ret = Encoding.Default.GetString(buf, 0, count);
                    resStream.Close();
                    return ret;
                }
                catch (Exception e)
                {
                    ret = "connect fail";
                }
            }
        }

        public static void get_page_to_txtbox(string url, TextBox dest) {
            dest.Text = "loading...";
            dest.Text = get_page(url);
        }

        public static string expland_range(string from)
        {
            if (from.EndsWith("/")) return from + "\\d+";

            string ret;
            //get last term
            int idx=from.LastIndexOfAny("/".ToCharArray());
            if (idx == -1) idx = 0;
            ret = from.Substring(0, idx);
            string last_term = from.Substring(idx);

            // [\d+] --> [\w+] --> [[^/]+]
            if (last_term == "/\\d+") {
                ret += "/\\w+";
            }
            else if (last_term == "/\\w+")
            {
                ret += "/[^/]+";
            }
            else {
                ret += last_term;
            }
            return ret;
        }
    
    }

    public class html_get_single_thread {
        public html_get_single_thread() { }

        public void update_url(string url)
        {
            begin_update();
            abort_last_request();
            try
            {//异步读网页
                request = (HttpWebRequest)WebRequest.Create(url);
                request.BeginGetResponse(new AsyncCallback(update_url_part2), null);
            }
            catch (Exception ex) {
                exception_update(ex);
            }
        }
        private void update_url_part2(IAsyncResult asyncResult)
        {
            string ret;
            try
            {
                {//异步读网页
                    //HttpWebResponse response = (HttpWebResponse)request.EndGetResponse(asyncResult);
                    HttpWebResponse response = (HttpWebResponse)request.GetResponse();
                    string encodeing = "utf-8";
                    string bit_encodeing = response.ContentEncoding;
                    if (response.ContentType.IndexOf("charset=") != -1)
                    {
                        int encodeing_beg = response.ContentType.IndexOf("charset=") + "charset=".Length;
                        int encodeing_end = response.ContentType.Substring(encodeing_beg).IndexOf(" ");
                        if (encodeing_end == -1) encodeing_end = response.ContentType.Substring(encodeing_beg).Length;
                        encodeing = response.ContentType.Substring(encodeing_beg, encodeing_end);
                    }
                    StreamReader resStream;
                    if(bit_encodeing=="gzip"){
                        GZipStream gstream= new GZipStream(response.GetResponseStream(), CompressionMode.Decompress);
                        resStream = new StreamReader(gstream, Encoding.GetEncoding(encodeing));
                    }else{
                    resStream = new StreamReader(response.GetResponseStream(), Encoding.GetEncoding(encodeing));
                    }
                    ret = resStream.ReadToEnd();
                    resStream.Close();
                }

                end_update(ret);
            }
            catch (Exception ex)
            {
                exception_update(ex);
            }
            request.Abort();
            request = null;
        }
        private void abort_last_request() {
            if (request != null)
                request.Abort();
        }


        public virtual void begin_update() { }
        public virtual void exception_update(Exception ex) { }
        public virtual void end_update(string html) { }

        HttpWebRequest request=null;
    }

 
}
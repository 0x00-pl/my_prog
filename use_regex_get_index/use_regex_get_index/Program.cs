﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace use_regex_get_index
{
    static class Program
    {
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main()
        {
            System.Windows.Forms.Control.CheckForIllegalCrossThreadCalls = false;
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new get_redex());
        }
    }
}

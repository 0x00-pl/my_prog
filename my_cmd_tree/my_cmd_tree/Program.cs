using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace my_cmd_tree
{
    class Program
    {
        static void Main(string[] args)
        {
            DirectoryInfo mydir = 
                new DirectoryInfo(
                    System.IO.Directory.GetCurrentDirectory()
                    );

            progress(mydir, ".");
            dest_sw.Close();
            dest_file.Close();
        }
        static void progress(DirectoryInfo curdir, string curpath)
        {
            foreach (FileInfo fi in curdir.GetFiles())
            {
                do_some(fi, curpath);
            }

            foreach (DirectoryInfo dic in curdir.GetDirectories())
            {
                progress(dic, curpath + "\\" + dic.Name);
            } 
        }
        static void do_some(FileInfo fi, string path)
        {
            //if(fi.Name.EndsWith(".java"))
                dest_sw.WriteLine(path+"\\"+fi.Name);
        }
        static FileStream dest_file=new FileStream ("filename.txt",FileMode.Create);
        static StreamWriter dest_sw = new StreamWriter(dest_file);
    }
}

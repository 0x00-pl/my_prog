using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace pack_file1
{
    class Program
    {
        int bloc_size = 0x800;
        static void Main(string[] args)
        {
            Program entry = new Program();
            //entry.pack("out.bin", @".\bin");
            if (args.Length < 3)
            {
                Console.WriteLine(" pack <dest> <from>");
               // Console.WriteLine(" unpack <dest> <from>");
                return;
            }
            if(args[0].Trim().StartsWith("pack"))
                entry.pack(args[1].Trim(), args[2].Trim());
            if (args[0].Trim().StartsWith("unpack"))
                entry.unpack(args[1].Trim(), args[2].Trim());
        }
        long my_ceiling(long num, long step) {
            return (num + step - 1) & ~(step-1);
        }

        void pack(string file_name, string dir_name) {
            dir_name = System.IO.Path.GetFullPath(dir_name);
            if (!dir_name.EndsWith(@"\")) dir_name = dir_name + @"\";
            if(!Directory.Exists(dir_name))
            {
                Console.WriteLine(dir_name);
                Console.WriteLine("源目录不存在");
                return;
            }
            FileStream output_fs = new FileStream(file_name, FileMode.Create);

            //make data and header
            string[] input_files = Directory.GetFiles(dir_name);
            long[] files_offset = new long[input_files.Length];
            long[] files_len = new long[input_files.Length];
            int base_on = input_files.Length * 4 * 2;

            MemoryStream file_data = new MemoryStream();
            byte[] file_buf = new byte[bloc_size];
            //init default mem
            for (int j = 0; j < bloc_size; j++) file_buf[j] = 0;

            for (int i = 0; i < input_files.Length; i++){
                FileInfo finfo = new FileInfo(input_files[i]);
                files_offset[i] = my_ceiling(base_on, bloc_size);
                base_on = (int)files_offset[i];
                files_len[i] = finfo.Length;

                FileStream input_fs = new FileStream(input_files[i], FileMode.Open);
                //copy file
                while (input_fs.Read(file_buf, 0, bloc_size) != 0){
                    file_data.Write(file_buf, 0, bloc_size);
                    for (int j = 0; j < bloc_size; j++) file_buf[j] = 0;
                    base_on += bloc_size;
                }
                input_fs.Close();
            }
            file_data.Flush();
            file_data.Seek(0, SeekOrigin.Begin);

            byte[] header = new byte[input_files.Length * 4 * 2];
            for (int i = 0; i < input_files.Length; i++)
            {
                Array.Copy(
                    BitConverter.GetBytes(files_offset[i]), 0,
                    header, i * 4 * 2,
                    4);
                Array.Copy(
                    BitConverter.GetBytes(files_len[i]), 0,
                    header, i * 4 * 2 + 4,
                    4);
            }

            //write to dest
            ////init default mem
            for (int j = 0; j < bloc_size; j++) file_buf[j] = 0;
            Array.Copy(header, file_buf, header.Length);
            output_fs.Write(file_buf, 0, file_buf.Length);
            byte[] data = new byte[base_on];
            file_data.Read(data, 0, base_on);
            output_fs.Write(data, 0, base_on);
            output_fs.Close();
        }
        void unpack(string dir_name, string file_name) {

            //dir_name = System.IO.Path.GetFullPath(dir_name);
            //if (!dir_name.EndsWith(@"\")) dir_name = dir_name + @"\";

            //if (!File.Exists(file_name))
            //{
            //    Console.WriteLine(System.IO.Path.GetFullPath(file_name));
            //    Console.WriteLine("源文件不存在");
            //    return;
            //}
            //FileStream input_fs = new FileStream(file_name, FileMode.Open);
            
            //byte[] int_buff = new byte[4];
            //input_fs.Read(int_buff, 0, 4);
            //int files_size= BitConverter.ToInt32(int_buff, 0);

            //int[] file_offset = new int[files_size+1];
            //for (int i = 0; i < files_size; i++){
            //    input_fs.Read(int_buff, 0, 4);
            //    file_offset[i]= BitConverter.ToInt32(int_buff, 0);
            //}

            //for(int i=0; i<files_size; i++){
            //    byte[] readbuff= new byte[file_offset[i+1]-file_offset[i]];
            //    input_fs.Read(readbuff, 0, file_offset[i+1]-file_offset[i]);
            //    FileStream out_fs = new FileStream(dir_name + i + ".bin", FileMode.Create);
            //    out_fs.Write(readbuff, 0, file_offset[i + 1] - file_offset[i]);
            //    out_fs.Close();
            //}

            //input_fs.Close();
        }
    }
}

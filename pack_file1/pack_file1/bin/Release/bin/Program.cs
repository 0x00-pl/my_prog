using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace pack_file1
{
    class Program
    {
        int bloc_size = 0x8000;
        static void Main(string[] args)
        {
            if (args.Length < 3)
            {
                Console.WriteLine(" pack <dest> <from>");
                Console.WriteLine(" unpack <dest> <from>");
                return;
            }
            Program entry = new Program();
            if(args[0].Trim().StartsWith("pack"))
                entry.pack(args[1].Trim(), args[2].Trim());
            if (args[0].Trim().StartsWith("unpack"))
                entry.unpack(args[1].Trim(), args[2].Trim());
        }
        long my_ceiling(long num, long step) {
            return (num + step - 1) % step;
        }

        void pack(string file_name, string dir_name) {
            dir_name = System.IO.Path.GetFullPath(dir_name);
            if (Directory.Exists("\"" + dir_name + "\""))
            {
                Console.WriteLine(dir_name);
                Console.WriteLine("源目录不存在");
                return;
            }
            FileStream output_fs = new FileStream(file_name, FileMode.Create);

            //make data and header
            string[] input_files = Directory.GetFiles(dir_name);
            int header_size = (input_files.Length + 1) * 4;
            long[] file_offset = new long[input_files.Length];
            int base_on = header_size;

            MemoryStream file_data = new MemoryStream();
            byte[] file_buf = new byte[bloc_size];

            for (int i = 0; i < input_files.Length; i++){
                file_offset[i] = my_ceiling(base_on, bloc_size);
                FileStream input_fs = new FileStream(input_files[i], FileMode.Open);
                //copy file
                while (input_fs.Read(file_buf, 0, bloc_size) != 0){
                    file_data.Write(file_buf, 0, bloc_size);
                    base_on += bloc_size;
                }
                input_fs.Close();
            }
            file_data.Flush();

            byte[] header = new byte[header_size];
            Array.Copy(BitConverter.GetBytes(input_files.Length), header, 4);
            for (int i = 0; i < input_files.Length; i++)
                Array.Copy(
                    BitConverter.GetBytes(input_files.Length), 0,
                    header, 4+i*4,
                    4);

            //write to dest
            output_fs.Write(header, 0, header.Length);
            byte[] data = file_data.ToArray();
            output_fs.Write(data, 0, data.Length);
            output_fs.Close();
        }
        void unpack(string dir_name, string file_name) {
            if (File.Exists(file_name))
            {
                Console.WriteLine(System.IO.Path.GetFullPath(file_name));
                Console.WriteLine("源文件不存在");
                return;
            }
            FileStream input_fs = new FileStream(file_name, FileMode.Open);
            
            byte[] int_buff = new byte[4];
            input_fs.Read(int_buff, 0, 4);
            int files_size= BitConverter.ToInt32(int_buff, 0);

            int[] file_offset = new int[files_size+1];
            for (int i = 0; i < files_size; i++){
                input_fs.Read(int_buff, 0, 4);
                file_offset[i]= BitConverter.ToInt32(int_buff, 0);
            }
            file_offset[files_size] = (int)new FileInfo(file_name).Length;
            if ((long)file_offset[files_size] != new FileInfo(file_name).Length) Console.WriteLine("file-size over 4G");

            for(int i=0; i<files_size; i++){

            }
            //FileStream out_fs= new FileStream;

        }
    }
}

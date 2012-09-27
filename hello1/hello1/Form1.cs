using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Security.Cryptography;
using System.IO.Compression;

namespace hello1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void btn_send_Click(object sender, EventArgs e)
        {
            saveFileDialog1.ShowDialog();
            using (FileStream sw = new FileStream(saveFileDialog1.FileName, FileMode.Create))
            {
                TripleDESCryptoServiceProvider tdes = new TripleDESCryptoServiceProvider();
                CryptoStream encStream = new CryptoStream(sw,
                    tdes.CreateEncryptor(exportfilepassword.key,
                    exportfilepassword.vec),
                    CryptoStreamMode.Write
                    );

                System.IO.Compression.GZipStream gcp = new System.IO.Compression.GZipStream(encStream, CompressionMode.Compress, false);
                byte[] bt_buff=  System.Text.Encoding.Default.GetBytes ( txt_msg.Text );
                gcp.Write(bt_buff, 0, bt_buff.Length);
                gcp.Close();        
            }
            txt_msg.Text = "";
        }

        private void btn_recv_Click(object sender, EventArgs e)
        {
            openFileDialog1.ShowDialog();
            using (FileStream rw = new FileStream(openFileDialog1.FileName, FileMode.Open))
            {
                TripleDESCryptoServiceProvider tdes = new TripleDESCryptoServiceProvider();
                CryptoStream decStream = new CryptoStream(rw,
                    tdes.CreateDecryptor(exportfilepassword.key,
                    exportfilepassword.vec),
                    CryptoStreamMode.Read
                    );
                GZipStream gcp = new GZipStream(decStream, CompressionMode.Decompress, true);
                byte[] bt_buff = new byte[1024];
                gcp.Read(bt_buff, 0, 1024);
                txt_msg.Text = System.Text.Encoding.Default.GetString(bt_buff);
                gcp.Close();
            }
        }
    }
    public class exportfilepassword
    {
        public static byte[] key = { 0x42, 0x38, 0x34, 0x38, 0x55, 0x41, 0x3e, 0x34, 0x4e, 0x25, 0x3d, 0x3e, 0x35, 0x3a, 0x34, 0x3a };
        public static byte[] vec = { 0x42, 0x33, 0x33, 0x53, 0x82, 0x03, 0xd3, 0xe3, 0xa2, 0x23, 0x73, 0xd3, 0x43, 0xb2, 0x03, 0xd3 };
    }
}

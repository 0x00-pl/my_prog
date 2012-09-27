using System;
using System.Collections.Generic;
using System.Text;
using System.Security.Cryptography;
using System.IO;

namespace Lib
{
    public class StringEncode
    {
        public static string EncodeString(string s)
        {
            using (MemoryStream mStream = new MemoryStream())
            {
                TripleDESCryptoServiceProvider tdes = new TripleDESCryptoServiceProvider();
                using (CryptoStream encStream = new CryptoStream(mStream,
                         tdes.CreateEncryptor(exportfilepassword.key,
                         exportfilepassword.vec),
                         CryptoStreamMode.Write
                         ))
                {
                    byte[] bt_buff = System.Text.Encoding.Default.GetBytes(s);
                    encStream.Write(bt_buff, 0, bt_buff.Length);
                    encStream.FlushFinalBlock();
                    return Convert.ToBase64String(mStream.ToArray());
                }
            }
        }
        public static string DecodeString(string s)
        {
            using (MemoryStream mStream = new MemoryStream())
            {
                TripleDESCryptoServiceProvider tdes = new TripleDESCryptoServiceProvider();
                using (CryptoStream encStream = new CryptoStream(mStream,
                         tdes.CreateDecryptor(exportfilepassword.key,
                         exportfilepassword.vec),
                         CryptoStreamMode.Write
                         ))
                {
                    byte[] bt_buff = Convert.FromBase64String(s);
                    encStream.Write(bt_buff, 0, bt_buff.Length);
                    encStream.FlushFinalBlock();
                    byte[] bt_buff_ret=mStream.ToArray();
                    return System.Text.Encoding.Default.GetString(bt_buff_ret, 0, bt_buff_ret.Length);
                }
            }
        }
    }
}

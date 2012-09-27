using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Kland_link_title
{
    class edit_distance
    {

        public int get_distance(string a, string b) { 
            int[] disarr= new int[a.Length*b.Length];
            //val[x,y]= min( val[x-1,y]+1, val[x,y-1], a[x]==b[y]? val[x-1,y-1]: val[x-1,y-1]+1)
            // ababc
            //a01234
            //b10123
            //c11122 ==>2

            //int[] cons_from_arr;  这个函数只计算编辑距离但是
            //  不关心是如何编辑的 所以不维护这个变量
            // ababc
            //a0----
            //b^\---
            //c^-\-- ==>2

            //计算
            int w= b.Length;
            for (int x = 0; x < a.Length; x++)
            {
                for (int y = 0; y < b.Length; y++)
                {
                    int min_val = int.MaxValue;
                    if (x > 0) min_val = Math.Min(disarr[(x - 1) * w + y]+1, min_val);
                    if (y > 0) min_val = Math.Min(disarr[x * w + (y-1)]+1, min_val);
                    if (x > 0 & y > 0) {
                        int temp= disarr[(x - 1) * w + (y - 1)];
                        if(a[x]!=b[y]) temp+=1;
                        min_val = Math.Min(temp, min_val); 
                    }
                    //start from 0
                    if (x == 0 & y == 0) min_val = 0;
                    disarr[x * w + y] = min_val;
                }
            }

            return disarr[a.Length * b.Length - 1];
        }
    }
}

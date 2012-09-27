using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace Kland_link_title
{
    class Program
    {
        public static header_map step1(doc_reader docs)
        {
            header_map ret = new header_map();
            foreach (doc_reader.doc doc in docs) {
                ret.push(doc.title, new doc_reader.doc.location(doc.id));
                //TODO 同义词
            }
            return ret;
        }
        public static bool step2_flip(doc_reader.doc doc, doc_reader.doc.location match_at, Match _match) {
            //TODO 过滤
            return true;
        }
        public static link_map step2_each(doc_reader.doc doc, KeyValuePair<string, doc_reader.doc.location> link, Regex law_regex)
        {
            link_map ret = new link_map();

            MatchCollection mc= law_regex.Matches(doc.text);
            foreach(Match iter in mc)
            {
                doc_reader.doc.location match_at= new doc_reader.doc.location(doc.id, iter.Index, iter.Length);
                if (step2_flip(doc, match_at, iter))
                    ret.push(match_at, link.Value);

                //TODO push <XXX>XX章
            }
            return ret;
        }
        public static link_map step2(doc_reader docs, header_map links) { 
            link_map ret = new link_map();
            //双循环可以改成多线程做
            foreach(doc_reader.doc doc in docs){
                foreach(KeyValuePair<string, doc_reader.doc.location> link in links){
                    Regex law_regex = new Regex(link.Key);
                    link_map temp= step2_each(doc, link, law_regex);
                    ret.union(temp);
                }
            }
            return ret;
        }


        static void Main(string[] args)
        {
            int temp = new edit_distance().get_distance("中华人民共和国刑法", "中华人民共和国公司法");
        //    header_map temp1= step1(new doc_reader());
        //    link_map temp2 = step2(new doc_reader(), temp1);
            
        }
    }

}

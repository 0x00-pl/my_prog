using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Kland_link_title
{
    class plantform
    {
    }

    public class doc_reader: System.Collections.IEnumerable
    {
        public class doc 
        {
            public doc() { }
            public doc(int _id, string _title, string _text) { id = _id; title = _title; text = _text; }
            public int id;
            public string title;
            public string text;
            public class location
            {
                public location(int _doc_id)
                {
                    doc_id = _doc_id;
                }
                public location(int _doc_id, int _offset, int _length)
                {
                    doc_id = _doc_id;
                    offset = _offset;
                    length = _length;
                }
                public int doc_id = -1;
                public int offset = -1;
                public int length = -1;
            }
        }
        public System.Collections.IEnumerator GetEnumerator()
        {
            //TODO 获取doc
            yield return new doc(0, "法律1", "xxxxxx");
            yield return new doc(1, "T1", "xx法律1xxxx");
            yield return new doc(2, "T2", "xxxxxx");
            yield return new doc(3, "T3", "xxxxT1T2T3T4T5xx");
            yield return new doc(4, "T4", "xxxxxx");
            yield return new doc(5, "T5", "xxxxxx");
            for (int i = 6; i < 1000; i++)
            {
                yield return new doc(i, "T"+i, "xxxxT1T2T3T4T5xx");
            }
        }
        public doc get_doc(int id) {
            return new doc();
        }
    }

    public class header_map : System.Collections.IEnumerable
    {
        public void push(string name, doc_reader.doc.location dest)
        {
            try
            {
                links.Add(name, dest);
            }catch(ArgumentException e){
                Console.WriteLine(e.Message+"  "+name);
            }
        }
        public System.Collections.IEnumerator GetEnumerator()
        {
            foreach (KeyValuePair<string, doc_reader.doc.location> i in links)
                yield return i;
        }

        Dictionary<string, doc_reader.doc.location> links = new Dictionary<string, doc_reader.doc.location>();
    }

    public class link_map : System.Collections.IEnumerable
    {
        public void push(doc_reader.doc.location from, doc_reader.doc.location dest)
        {
            links.Add(from, dest);
        }
        public void union(link_map o) {
            foreach (KeyValuePair<doc_reader.doc.location, doc_reader.doc.location> iter in o) {
                push(iter.Key, iter.Value);
            }
        }
        public System.Collections.IEnumerator GetEnumerator()
        {
            foreach (KeyValuePair<doc_reader.doc.location, doc_reader.doc.location> i in links)
                yield return i;
        }

        Dictionary<doc_reader.doc.location, doc_reader.doc.location> links = new Dictionary<doc_reader.doc.location, doc_reader.doc.location>();
    }
}

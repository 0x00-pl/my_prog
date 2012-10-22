#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<functional>
using namespace std;
#include"raii.h"

namespace pl{
	
	template<typename Tdst, typename Tsrc>
	class convert{
	public:
		Tdst operator()(const Tsrc& src){
			return move(Tdst(src));
		}
	};

	//to_string
	template<>
	class convert<string,bool>{
	public:
		convert(string _t="true", string _f="false"):t(_t),f(_f){}
		string t;
		string f;
		string operator()(bool src){
			return src? t: f;
		}
	};

	template<>
	class convert<string, long double>{
	public:
		string operator()(long double src){
			return to_string(src);//c++11
		}
	};
	template<>
	class convert<string, double>:public convert<string,long double>{};
	template<>
	class convert<string, float>:public convert<string,long double>{};
	template<>
	class convert<string, long long>{
	public:
		string operator()(long long src){
			return to_string(src);//c++11
		}
	};
	template<>
	class convert<string, int>:public convert<string,long long>{};

	template<>
	class convert<string, unsigned long long>{
	public:
		string operator()(unsigned long long src){
			return to_string(src);//c++11
		}
	};
	template<>
	class convert<string, unsigned int>:public convert<string,unsigned long long>{};
	template<>
	class convert<string, short>:public convert<string,unsigned long long>{};
	template<>
	class convert<string, unsigned short>:public convert<string,unsigned long long>{};

	
	template<>
	class convert<string, char>{
	public:
		string operator()(char src){
			string ret;
			ret=src;
			return ret;
		}
	};
	


	template<typename T>
	class convert<string, vector<T> >{
	public:
		string left;
		string comma;
		string right;
		function<string(T)> stringlize_element;

		convert(string _left, string _right, string _comma, function<string(T)> _stringlize_element):
		left(_left), right(_right), comma(_comma), stringlize_element(_stringlize_element){}
		convert():left("["), right("]"), comma(", "), stringlize_element(convert<string,T>()){}

		string operator()(vector<T> src){
			string ret;
			string& _right= right;
			string& _left= left;
			do{
				raii _([&](){ret+=_left;}, [&](){ret+=_right;});
				if(src.empty()) break;
				ret+= stringlize_element(*src.begin());
				for(auto iter=++src.begin(); iter!=src.end(); ++iter){
					ret+= comma;
					ret+= stringlize_element(*iter);
				}
			}while(false);
			return ret;
		}
	};

	template<typename Tk, typename Tv>
	class convert<string, map<Tk,Tv> >{
	public:
		string left;
		string comma;
		string eq;
		string right;
		function<string(Tk)> stringlize_key;
		function<string(Tv)> stringlize_val;

		convert(string _left, string _right, string _comma, string _eq,
			function<string(Tk)> _stringlize_key, 
			function<string(Tv)> _stringlize_val):
		left(_left), right(_right), comma(_comma), eq(_eq),
			stringlize_key(_stringlize_key),
			stringlize_val(_stringlize_val)
		{}
		convert():left("{"), right("}"), comma(", "), eq(":"),
			stringlize_key(convert<string,Tk>()),
			stringlize_val(convert<string,Tv>()){}

		string operator()(map<Tk,Tv> src){
			string ret;
			string& _right= right;
			string& _left= left;
			do{
				raii _([&](){ret+=_left;}, [&](){ret+=_right;});
				if(src.empty()) break;
				ret+= stringlize_key(src.begin()->first);
				ret+= eq;
				ret+= stringlize_val(src.begin()->second);
				for(auto iter=++src.begin(); iter!=src.end(); ++iter){
					ret+= comma;
					ret+= stringlize_key(iter->first);
					ret+= eq;
					ret+= stringlize_val(iter->second);
				}
			}while(false);
			return ret;
		}
	};

}
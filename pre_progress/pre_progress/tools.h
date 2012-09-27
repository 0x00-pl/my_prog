#include<functional>
#include<map>
#include<regex>
using namespace std;

class at_end{
public:
	function<void()> _f_end;
	bool enable;
	at_end(function<void()> func){
		_f_end= func;
		enable= true;
	}
	~at_end(){
		if(!enable)
			_f_end();
	}
};


//---toString---
//builder
class tostring_bulider{
public:
	string _str;
	string _prefix;
	string _comma;
	string _suffix;
	bool first_item;

	tostring_bulider(){
		first_item= true;
		_prefix= "[";
		_suffix= "]";
		_comma= ", ";
	}
	void prefix(){
		_str+= _prefix;
	}
	template<typename T>
	void push(const T& item){
		if(first_item){
			first_item= false;
		}else{
			_str+= _comma;
		}
		_str+= ::toString(item);
	}
	void suffix(){
		_str+= _suffix;
	}
	string toString()const{return _str;}
};
class tostring_map_bulider{
public:
	string _str;
	string _prefix;
	string _comma;
	string _eq;
	string _suffix;
	bool first_item;

	tostring_map_bulider(){
		first_item= true;
		_prefix= "{";
		_suffix= "}";
		_comma= ", ";
		_eq= ":";
	}
	void prefix(){
		_str+= _prefix;
	}
	template<typename T>
	void push(const T& item){
		if(first_item){
			first_item= false;
		}else{
			_str+= _comma;
		}
		_str+= ::toString(item.first)+ _eq+ ::toString(item.second);
	}
	void suffix(){
		_str+= _suffix;
	}
	string toString()const{return _str;}
};
//etc.

//default type
string toString(const string& s){return s;}

//system type
template<typename T>
string toString(const T& v){
	return v.toString();
}
template<>
string toString<char>(const char& v){
	return string()+v;
}
template<>
string toString<bool>(const bool& v){
	return v? "true": "false";
}
//etc.

//Container
template<typename T, typename Tfmt>
string toString(const T& v, Tfmt fmter){
	fmter.prefix();
	for(auto iter = begin(v); iter!=end(v); ++iter)
		fmter.push(*iter); 
	fmter.suffix();
	return move(toString(fmter));
}

//std Container
template<typename T>
string toString(const vector<T>& v){
	return toString(v,tostring_bulider());
}

template<typename Tk, typename Tv>
string toString(const map<Tk,Tv>& v){
	return toString(v, tostring_map_bulider());
}
//etc.






vector<string> split_string(string txt, const regex& comma){
	vector<string> ret;
	smatch m;
	int offset= 0;
	while (regex_search(txt, m, comma, regex_constants::match_not_bol)) {
		string& temp= m.prefix().str()+m.str();
		if(!temp.empty()) ret.push_back(temp);
		txt= m.suffix();
	}
	string temp= txt;
	if(!temp.empty()) ret.push_back(temp);
	return move(ret);
}










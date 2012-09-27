#include<fstream>
#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include<array>
using namespace std;

#include"section.h"

string load_testfile(){
	fstream ifs;
	ifs.open("./testfile.cpp", ios::in);
	if(ifs.fail()) return "read fail";
	stringstream ss;

	while(!ifs.eof()){
		string line;
		getline(ifs, line);
		ss<<line<<'\n';
	}

	return ss.str();
}


map<char, vector<bool> > make_test_val(){
	map<char, vector<bool> > ret;
	vector<bool> tvb;
	tvb.push_back(false);
	tvb.push_back(false);
	tvb.push_back(true);
	tvb.push_back(false);
	ret['a']=tvb;
	tvb.push_back(true);
	tvb.push_back(false);
	tvb.push_back(true);
	tvb.push_back(false);
	ret['b']=tvb;

	return ret;
}
class my_tostring_map_bulider: public tostring_map_bulider{
public:
	string toString_vector_bool(const vector<bool>& v){
		string ret;
		for(auto iter= begin(v); iter!=end(v); ++iter)
			ret+= *iter? "1": "0";
		return ret;
	}
	void push(const pair<char, vector<bool> >& item){
		if(first_item){
			first_item= false;
		}else{
			_str+= _comma;
		}
		_str+= ::toString(item.first)+ _eq+ toString_vector_bool(item.second);
	}
};


void main(){
	map<char, vector<bool> > temp= make_test_val();
	cout<< toString(temp, my_tostring_map_bulider());
	//cout<< toString(temp);


	string testfile= load_testfile();
	get_section(testfile);


}
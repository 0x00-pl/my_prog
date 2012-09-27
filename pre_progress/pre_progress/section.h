#pragma once
#include<string>
#include<regex>
#include<vector>
using namespace std;
#include"tools.h"

class section{
public:
	section(string alltext){
		string header;
		smatch m;
		if(regex_search(alltext, m, regex("/\\*@(.*)@\\*/"),  regex_constants::match_not_bol)){
			header= m[1].str();
			text_after= m.suffix().str();
			smatch mt;
			if(regex_search(header, mt, regex("\\{(.*)\\}"),  regex_constants::match_not_bol)){
				text= mt[1].str();
			}
		}else{
			header= "&all"; 
			text_after= alltext;
			text= "";
		}
		holder= split_string(header, regex(" "));
	}
	vector<string> holder;
	string text;
	string text_after;
};

vector<string> split_section(string file){
	return split_string(file, regex("[.\\n](?=/\\*@)"));
}


vector<section> get_section(string file){
	vector<section> ret;
	vector<string> section_string= split_section(file);
	for_each(begin(section_string), end(section_string), [&](string s){ret.push_back(section(s));});
	return move(ret);
}
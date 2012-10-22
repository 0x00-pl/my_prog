#include<regex>
#include<iostream>
using namespace std;
#include"convert.h"
using namespace pl;


void main(){
	int ipt; cin>>ipt;
	cout<<convert<int,bool>()(ipt)<<endl;
	cout<<convert<string,bool>()(ipt)<<endl;
	cout<<convert<string,bool>("1","0")(ipt)<<endl;
	cout<<convert<string,int>()(ipt)<<endl;
	cout<<convert<string,unsigned int>()(ipt)<<endl;
	cout<<convert<string,char>()(char(ipt))<<endl;
	cout<<convert<string,float>()(0.5*ipt)<<endl;
	vector<int> vec_int; vec_int.push_back(1); vec_int.push_back(2); vec_int.push_back(ipt);
	cout<<convert<string,vector<int>>()(vec_int)<<endl;
	map<string, vector<int>> vec_vec_int; vec_vec_int["vec_name"]= vec_int; vec_vec_int["vec_name_cpy"]= vec_int;
	cout<<convert<string,decltype(vec_vec_int)>()(vec_vec_int)<<endl;
	

	char pse;
	cin>>pse;
}
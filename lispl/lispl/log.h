#pragma once
#include<iostream>
using namespace std;


#define plog(type,message) log::type(message)
//#define plog(type,meddage) /*do nothing*/
class log{
public:
	static void error(string msg){
		cout<<msg<<endl;
	}
};

#pragma once
#include<string>
using namespace std;


void compare(string cl_path, string cppfile, string dest){
	string cmd= cl_path+ " "+ ""+ cppfile;
	system(cmd.c_str());
}
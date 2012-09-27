/*@ &stpcpp @*/
#include<iostream>
#include<string>
using namespace std;
/*@ &last @*/
#include<stdio.h>

/*@ &stpcpp @*/
string make_str(){return "hello world!";}

/*@ &last { char* str= }@*/
/*@ *stpcpp { ret<<make_str(); }@*/
/*@ &ignore @*/
char* str="AUTO GEN";

/*@ &last {
void main(){
	printf("%s", str);
}
}@*/
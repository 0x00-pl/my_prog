#include<iostream>
#include<vector>
#include<functional>
//#include<map>
using namespace std;


//#define arr(exp,x,collection,cond) 	\
//	[&]()->vector<remove_reference<decltype(*begin(collection))>::type>{\
//	  vector<remove_reference<decltype(*begin(collection))>::type>  ret;    /*此处的类型应该为 decltype(exp) 但是此处的exp的环境不全  */\
//	  for(auto px= begin(collection); px!=end(collection); ++px) { \
//	   auto x=*px; \
//	   if(cond){\
//	    ret.push_back(exp);  \
//	   }\
//	  }\
//	  return move(ret);\
//	}()
#define loop(_exp, _for, _in, _if) for(auto iter= begin(_in); iter!=end(_in); ++iter) { \
	auto _for= *iter; \
	if(_if) _exp;  \
	}


void split(std::string& s, std::string& delim,std::vector< std::string >* ret)
{
 size_t last = 0;
 size_t index=s.find_first_of(delim,last);
 while (index!=std::string::npos)
 {
  ret->push_back(s.substr(last,index-last));
  last=index+1;
  index=s.find_first_of(delim,last);
 }
 if (index-last>0)
 {
  ret->push_back(s.substr(last,index-last));
 }
}




void main(){
	function<int(int)> f;
	vector<int> arr;
	arr.push_back(0);
	arr.push_back(1);
	arr.push_back(2);
	arr.push_back(3);

	//[f(x) for x in arr if z(x)]  
	loop(cout<<x, x, arr, x>1)
}
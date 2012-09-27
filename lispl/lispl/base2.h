#pragma once
#include<vector>
#include<map>
#include<sstream>
#include"log.h"
#include"shared_ptr.h"

using namespace std;

class stack_tree;
class tail;

class node{
public:
	virtual~node(){}
	virtual void call(shared_ptr<stack_tree> env, tail& ret){}
	virtual string to_string(){return "node";}
};

shared_ptr<node> nil((node*)0);
bool isnil(shared_ptr<node> n){
	return n==nil;
}


class stack_tree{
public:
	stack_tree(){
		prev_stack= ((stack_tree*)0);
	}
	stack_tree(shared_ptr<stack_tree> prev){
		prev_stack= prev;
	}
	shared_ptr<node> get(int lvl, size_t offset){
		if(lvl==0) 
			return get_from_cur_stack(offset);
		return prev_stack->get(lvl-1,offset);
	}
	shared_ptr<node> get(int lvl, string name){
		if(lvl==0) 
			return get_from_cur_stack(name);
		return prev_stack->get(lvl-1,name);
	}
	shared_ptr<node> get(string name){
		map<string,size_t>::iterator dest= cur_name_mapping.find(name);
		if(dest!=cur_name_mapping.end())
			return get_from_cur_stack(dest->second);
		return prev_stack->get(name);
	}

	shared_ptr<node> get_from_cur_stack(size_t offset){
		if(this==0) return nil;
		if(offset<cur_stack.size())
			return cur_stack[offset];
		return nil;
	}
	shared_ptr<node> get_from_cur_stack(string name){
		if(this==0) return nil;
		map<string,size_t>::iterator dest= cur_name_mapping.find(name);
		if(dest==cur_name_mapping.end()) return nil;
		return get_from_cur_stack(dest->second);
	}

	size_t put(shared_ptr<node> val){
		cur_stack.push_back(val);
		return cur_stack.size()-1;
	}
	void put(size_t offset, shared_ptr<node> val){
		if(offset<=cur_stack.size())
			cur_stack.insert(cur_stack.begin()+offset, val);
	}
	void put(string name, shared_ptr<node> val){
		map<string,size_t>::iterator dest= cur_name_mapping.find(name);
		if(dest!=cur_name_mapping.end()){
			cur_stack[dest->second]= val;
		}else{
			size_t offset= put(val);
			cur_name_mapping[name]= offset;
		}
	}

	size_t size(){return cur_stack.size();}

	shared_ptr<stack_tree> prev_stack;
	vector<shared_ptr<node> > cur_stack;
	map<string, size_t> cur_name_mapping;
};


shared_ptr<stack_tree> empty_stack= new stack_tree();

class tail{
public:
	tail(){
		data=nil;
		env= empty_stack;
	}
	tail(shared_ptr<node> _data, shared_ptr<stack_tree> _env= empty_stack){
		data=_data;
		env=_env;
	}
	shared_ptr<node> get_node(){
		while(tailing()){
			data->call(env, *this);
		}
		return data;
	}

	bool tailing(){
		return env!= empty_stack;
	}
	shared_ptr<node> data;
	shared_ptr<stack_tree> env;
};


//封装宿主类型
template<typename T>
class lispl_type: public node{
public:
	lispl_type(const T& t){val=t;}
	string to_string(){
		string sval("NoData");
		stringstream sconvert;
		sconvert<<val;
		sconvert>>sval;
		return string("#")+typeid(T).name()+"("+sval+")";
	}

	operator T&(){return val;}
	T val;
};

//typedef void (*T_node_call)(shared_ptr<stack_tree>,tail&);
template<void (*Tf)(shared_ptr<stack_tree>,tail&)>
class lispl_func: public node{
public:
	void call(shared_ptr<stack_tree> env, tail& ret){
		Tf(env,ret);
	}
	string to_string(){
		return "lispl_func";
	}
};


class symbol: public node{
public:
	symbol(int _lvl, size_t _offset){lvl=_lvl; offset=_offset;}
	void call(shared_ptr<stack_tree> env, tail& ret){
		ret= tail(env->get(lvl,offset));
	}
	string to_string(){return "#symbol";}
	int lvl;
	size_t offset;
};

class list: public node{
public:
	list(){}
	void call(shared_ptr<stack_tree> prev_env, tail& ret){
		shared_ptr<stack_tree> cur_env= new stack_tree(prev_env);
		for(size_t i=0; i<sub.size(); ++i){
			tail temp;
			sub[i]->call(prev_env, temp);
			cur_env->put(i, temp.get_node());
		}
		ret.env= cur_env;
		ret.data= cur_env->get_from_cur_stack(0);
	}
	string to_string(){return "#list";}

	void add(shared_ptr<node> n){
		sub.push_back(n);
	}
	vector<shared_ptr<node> > sub;
};

class quote: public node{
public:
	quote(shared_ptr<node> n){
		quoted_node=n;
	}
	void call(shared_ptr<stack_tree> prev_env, tail& ret){
		ret= tail(quoted_node);
	}
	string to_string(){return "quote";}
	shared_ptr<node> quoted_node;
};
void make_quote(shared_ptr<stack_tree> prev_env, tail& ret){
	shared_ptr<node>& temp_n= prev_env->get_from_cur_stack(1); 
	ret= tail(new quote(temp_n));
}

class lambda: public node{
public:

	lambda(shared_ptr<node> _exp, shared_ptr<stack_tree> stack_on_create){
		closure= stack_on_create;
		exp=_exp;
	}
	void call(shared_ptr<stack_tree> prev_env, tail& ret){
		shared_ptr<stack_tree> cur_env= new stack_tree(closure);
		
		for(size_t i=0; i<prev_env->size(); ++i){
			cur_env->put(i, prev_env->get_from_cur_stack(i));
		}

		ret.data= exp;
		ret.env= cur_env;
	}

	string to_string(){return "lambda";}
	shared_ptr<node> exp;
	shared_ptr<stack_tree> closure;
};

//(lambda () (exp))
void make_lambda(shared_ptr<stack_tree> prev_env, tail& ret){

	lambda* temp_lmd= new lambda(
		prev_env->get_from_cur_stack(2),
		prev_env->prev_stack
		);

	ret= tail(temp_lmd);
}
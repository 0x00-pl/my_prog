#pragma once
#include<vector>
#include<map>
#include<sstream>
#include"log.h"
#include"shared_ptr.h"

using namespace std;

struct nil_type{virtual~nil_type(){}};


class node;
shared_ptr<node> nil((node*)0);

bool isnil(shared_ptr<node> n){
	return n==nil;
}


class stack_tree{
public:
	stack_tree(shared_ptr<stack_tree> prev){
		prev_stack= prev;
	}
	shared_ptr<node> get(int lvl, size_t offset){
		if(lvl==0) 
			return get_from_cur_stack(offset);
		if(prev_stack!=nil)
			return prev_stack->get(lvl-1,offset);
		return nil;
	}
	shared_ptr<node> get(int lvl, string name){
		if(lvl==0) 
			return get_from_cur_stack(name);
		if(prev_stack!=nil)
			return prev_stack->get(lvl-1,name);
		return nil;
	}
	shared_ptr<node> get(string name){
		map<string,size_t>::iterator dest= cur_name_mapping.find(name);
		if(dest!=cur_name_mapping.end())
			return get_from_cur_stack(dest->second);
		if(prev_stack!= nil)
			return prev_stack->get(name);
		return nil;
	}

	shared_ptr<node> get_from_cur_stack(size_t offset){
		if(offset<cur_stack.size())
			return cur_stack[offset];
		return nil;
	}
	shared_ptr<node> get_from_cur_stack(string name){
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

shared_ptr<stack_tree> empty_stack= new stack_tree(nil);


class tail{
public:
	tail(){
		data=nil;
		is_quote=true;
	}
	tail(shared_ptr<node> _data, bool _is_quote){
		data=_data;
		is_quote=_is_quote;
	}
	//shared_ptr<node> get_node(){
	//	shared_ptr<node> ret= data;
	//	while(tailing()){
	//		ret=ret.data->call(empty_stack);
	//	}
	//}
	shared_ptr<node> data;
	bool is_quote;
};

class node{
public:
	node(){is_macro=false;is_value=true;}
	virtual~node(){}
	virtual tail call(shared_ptr<stack_tree> stack_base){return tail();}
	virtual string to_string(){return "#node";}

	bool is_macro;
	bool is_value;
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


static shared_ptr<node> untail(tail& tl){
	while(!tl.is_quote){
		tl=tl.data->call(empty_stack);
	}
	return tl.data;
}
static shared_ptr<node> eval(shared_ptr<node> n, shared_ptr<stack_tree> _stack){
	if(n->is_value){
		return n;
	}
	tail ret;
	ret=n->call(_stack);
	return untail(ret);
}

class lazy_node: public node{
public:
	lazy_node(shared_ptr<node> _exp, shared_ptr<stack_tree> _stack){
		exp=_exp;
		e_stack=_stack;
	}

	tail call(shared_ptr<stack_tree> un_used_arg){
		return exp->call(e_stack);
	}
	string to_string(){return "#lazy-node";}

	shared_ptr<node> exp;
	shared_ptr<stack_tree> e_stack;
};

class symbol: public node{
public:
	symbol(int _lvl, size_t _offset){is_value=false; lvl=_lvl; offset=_offset;}
	tail call(shared_ptr<stack_tree> stack_base){
		return tail(stack_base->prev_stack->get(lvl,offset), true);
	}
	string to_string(){return "#symbol";}
	int lvl;
	size_t offset;
};



class list: public node{
public:
	list(){is_value=false;}
	tail call(shared_ptr<stack_tree> stack_base){
		if(sub.empty()) return tail();

		shared_ptr<stack_tree> fstack= new stack_tree(empty_stack);
		shared_ptr<node> pf= eval(sub[0],stack_base);
		fstack->put(pf);
		if(pf->is_macro){
			for(unsigned int i=1; i<sub.size(); ++i){
				fstack->put(sub[i]);
			}
		}else{
			for(unsigned int i=1; i<sub.size(); ++i){
				shared_ptr<node> pt=eval(sub[i],stack_base);
				fstack->put(pt);
			}
		}
		fstack->prev_stack= stack_base->prev_stack;
		return tail(new lazy_node(pf, fstack), false);
	}
	string to_string(){return "#list";}

	void add(shared_ptr<node> n){
		sub.push_back(n);
	}
	vector<shared_ptr<node> > sub;
};


class quote: public node{
public:
	quote(){
		is_macro=true;
	}
	tail call(shared_ptr<stack_tree> stack_base){
		return tail(stack_base->get(0,1),true);
	}
	string to_string(){return "quote";}
};



//(lambda '(keyword) '(exp))
//(lambda 
//'(keyword)
//'(collect 
// '(range 0 100)
// '(lambda '((arg n base)) '(+ [2] [1]))))
class lambda: public node{
public:

	lambda(shared_ptr<list> _exp, shared_ptr<stack_tree> stack_on_create){
		is_value=false;
		closure= stack_on_create;
		exp=_exp;
	}
	tail call(shared_ptr<stack_tree> stack_base){
		//if(stack_base->prev_stack!=nil){
		//	plog(error,"??? why? assert!");//- - 也许会进入这个
		//}
		stack_base->prev_stack= closure;
		shared_ptr<stack_tree> temp= new stack_tree(stack_base);
		return tail(new lazy_node(exp,temp), false);
	}

	string to_string(){return "lambda";}
	shared_ptr<list> exp;
	shared_ptr<stack_tree> closure;
};
class make_lambda: public node{
public:
	make_lambda(){is_value=false;}
	tail call(shared_ptr<stack_tree> stack_base){
		shared_ptr<lambda> ret= new lambda(stack_base->get(0,2), stack_base->prev_stack);
		//shared_ptr<lambda> ret= new lambda(stack_base->get(0,2), stack_base);
		return tail(ret, true);
	}
	string to_string(){return "make_lambda";}
};

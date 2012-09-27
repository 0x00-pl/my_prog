#pragma once
#include"base.h"


//(cond if1 exp1 if2 exp2 ... )
class lispl_cond: public node{
public:
	lispl_cond(){
		is_macro=true;
	}
	tail call(shared_ptr<stack_tree> stack_base){
		static int n=0;
		cout<<n++<<endl;
		vector<shared_ptr<node> >& args= stack_base->cur_stack;

		for(size_t i=1; i<args.size(); i+=2){
			shared_ptr<node> _if= stack_base->get_from_cur_stack(i);
			shared_ptr<node> _exp= stack_base->get_from_cur_stack(i+1);
			if(!isnil(eval(_if,stack_base))){
				return tail(_exp,false);
			}
		}

		return tail();
	}
	string to_string(){return "cond";}
};


class lispl_eq: public node{
public:
	tail call(shared_ptr<stack_tree> stack_base){
		shared_ptr<lispl_type<int> > arg1= stack_base->get_from_cur_stack(1);
		shared_ptr<lispl_type<int> > arg2= stack_base->get_from_cur_stack(2);

		if(arg1==nil&arg2==nil) return tail(new quote(),true);//t
		if(arg1==nil|arg2==nil) return tail();//nil
		if(arg1->val==arg2->val) return tail(new quote(),true);//t
		return tail();//nil
	}
	string to_string(){return "eq";}
};

class lispl_add: public node{
public:
	tail call(shared_ptr<stack_tree> stack_base){
		shared_ptr<lispl_type<int> > arg1= stack_base->get_from_cur_stack(1);
		shared_ptr<lispl_type<int> > arg2= stack_base->get_from_cur_stack(2);
		if(arg1==nil||arg2==nil) return tail();
		int sum= arg1->val+arg2->val;
		return tail(new lispl_type<int>(sum),true);
	}
	string to_string(){return "add";}
};
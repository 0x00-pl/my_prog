#pragma once
#include<iostream>
#include<sstream>
#include<limits>
#include"base.h"
using namespace std;

std::string trim(std::string& str)
{
str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
str.erase(str.find_last_not_of(' ')+1);         //surfixing spaces
return str;
}
void pick_space(string& str){
	str.erase(str.find_last_not_of(' ')+1);
}
string first_term(const string& str){
	size_t min_index= str.find_first_of(' ');
	min_index= min(min_index, str.find_first_of(')'));
	min_index= min(min_index, str.find_first_of('}'));
	min_index= min(min_index, str.find_first_of('>'));
	return str.substr(0, min_index);
}
node* make_quote(shared_ptr<node> val){
	list* ret= new list();
	ret->sub.push_back(new quote());
	ret->sub.push_back(val);
	return ret;
}

	//(lambda[add 1 &1]) inc
	//(lambda(list 'lambda (list 'add &1 '&1))) addn
	//(lambda{lambda ,{add &1 ,&1}})     { ==> (list
	//(lambda[lambda[add &1 &&1]]) addn
	//{add &1 1} inc
	//{'{add <0,1> <1,1>}} addn

	//(lambda [n] '{[lambda[a b][+ a b]] &1 &n}) addn
	//symbol自动加quote
	//[]等价于'()
	//{}等价于(lambda ... )


namespace parser_pl_origin{
	class parser{
	public:
		virtual~parser(){}
		virtual bool test(string&){return false;}
		virtual shared_ptr<node>get_node(string&){return nil;}
	};
	class parser_manager{
	public:
		static parser_manager ins;
		shared_ptr<node> parse(string& s){
			trim(s);
			if(s.empty()) return nil;
			for(size_t i=0; i<parser_list.size(); ++i){
				if(parser_list[i]->test(s)){
					return parser_list[i]->get_node(s);
				}
			}
			return nil;
		}

		vector<parser*> parser_list;
	};
	parser_manager parser_manager::ins;

	class parser_list: public parser{
	public:
		virtual bool test(string& s){
			return s[0]=='(';
		}
		virtual shared_ptr<node>get_node(string& s){
			shared_ptr<list> ret= new list();
			s=s.substr(1);//'('
			while(s[0]!=')'){
				ret->add(parser_manager::ins.parse(s));
			}
			s=s.substr(1);//')'
			s=trim(s);
			return ret;
		}
	};
	class parser_quote: public parser{
	public:
		virtual bool test(string& s){
			return s[0]=='\'';
		}
		virtual shared_ptr<node>get_node(string& s){
			shared_ptr<list> ret= new list();
			s=s.substr(1);//'''
			ret->add(new quote());
			ret->add(parser_manager::ins.parse(s));
			return ret;
		}
	};
	template<typename T>
	class parser_func: public parser{
	public:
		parser_func(string _name){name=_name;}
		virtual bool test(string& s){
			return name==first_term(s);
		}
		virtual shared_ptr<node>get_node(string& s){
			size_t tl= first_term(s).length();
			s= s.substr(tl);
			s= trim(s);
			return make_quote(new T());
		}
		string name;
	};

	template<typename T>
	class parser_type: public parser{
	public:
		virtual bool test(string& s){
			stringstream ss;
			ss<<first_term(s);			
			T f;
			ss.clear();
			ss>>f;
			return !ss.fail();
		}
		virtual shared_ptr<node>get_node(string& s){
			string sft=first_term(s);
			stringstream ss;
			ss<<sft;		
			T val;
			ss>>val;
						
			size_t tl= sft.length();
			s= s.substr(tl);
			s= trim(s);
			return make_quote(new lispl_type<T>(val));
		}
	};

	//<lvl,index>
	class parser_symbol: public parser{
	public:
		virtual bool test(string& s){
			return s[0]=='<';
		}
		virtual shared_ptr<node>get_node(string& s){
			string symbol_text= s.substr(0, s.find_first_of('>')+1);
			s= s.substr(symbol_text.length());
			s= trim(s);

			stringstream ss;
			ss<<symbol_text;

			char c;
			int lvl,offset;

			//<int,int>
			ss>>c>>lvl>>c>>offset;

			if(ss.fail()){return nil;}

			return new symbol(lvl,offset);
		}
	};
	
	//{exp}
	//('lambda '() '(exp))
	class parser_lambda_shortcut: public parser{
	public:
		virtual bool test(string& s){
			return s[0]=='{';
		}
		virtual shared_ptr<node>get_node(string& s){
			shared_ptr<list> ret= new list();
			ret->add(make_quote(new make_lambda()));
			ret->add(make_quote(nil));
			
			s=s.substr(1);//'{'

			shared_ptr<list> exp= new list();
			while(s[0]!='}'){
				exp->add(parser_manager::ins.parse(s));
			}
			ret->add(make_quote(exp));

			s=s.substr(1);//'}'
			pick_space(s);
			return ret;
		}
	};
}

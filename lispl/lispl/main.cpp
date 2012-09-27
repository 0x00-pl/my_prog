//#include<iostream>
//#include"base.h"
//#include"func.h"
//#include"parse.h"
//using namespace std;
//using namespace parser_pl_origin;
//
//
//
//
//void parser_unit_test(){
//	parser_manager::ins.parser_list.push_back(new parser_list);
//	parser_manager::ins.parser_list.push_back(new parser_lambda_shortcut);
//	parser_manager::ins.parser_list.push_back(new parser_quote);
//	parser_manager::ins.parser_list.push_back(new parser_symbol);
//	parser_manager::ins.parser_list.push_back(new parser_func<make_lambda>("lambda"));
//	parser_manager::ins.parser_list.push_back(new parser_func<lispl_cond>("cond"));
//	parser_manager::ins.parser_list.push_back(new parser_func<lispl_eq>("eq"));
//	parser_manager::ins.parser_list.push_back(new parser_func<lispl_add>("add"));
//	parser_manager::ins.parser_list.push_back(new parser_type<int>);
//
//	////string text("(((lambda '() '(lambda '() '(add <0,1> <1,1>)))100)100)");
//	//string text("((({'{'{add <2,1> <1,1>}}} 100) 200) 300)");
//	//shared_ptr<node> exp= parser_manager::ins.parse(text);
//	//cout<<eval(exp, empty_stack)->to_string()<<endl;
//}
//
//void main(){
//	//cout<<"hello world"<<endl;
//	system("title pl");
//
//	parser_unit_test();
//
//	string text;
//	while(1){
//		//getline( cin, text, '\n' );
//		text="({cond (eq <0,1> 10) <0,1> 1 (<0,0> (add <0,1> 1))} 0)";
//		shared_ptr<node> exp= parser_manager::ins.parse(text);
//		cout<<"parsed."<<endl;
//		cout<<eval(exp, empty_stack)->to_string()<<endl;
//	char pause;
//	cin>>pause;
//	}
//}


#include"base2.h"

void main(){
	lispl_func<&make_quote>;
}
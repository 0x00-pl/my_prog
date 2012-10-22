#include"parser.h"
#include"bloc_graph.h"
#include"bloc_opt.h"
#include"show_mid_code.h"

char* testprog(){
	return
	"{ Sample program\n\
	in TINY language\n\
	}\n\
	read x; { input an integer }\n\
	if 0 < x then { don't compute if x <= 0 }\n\
	  fact := 1;\n\
	  repeat\n\
	    fact := fact * x; \n\
	    x := x - 1 \n\
	  until x = 0; \n\
	  write fact { output factorial of x } \n\
	end";
}

void main(){
	auto toks= get_tokens(testprog());
	//auto& exp_temp= get_tokens("1+2+3+(4<10)");
	//auto exp= match_exp(exp_temp.begin(), exp_temp.end());
	auto prog= match_stmt_sequence(toks.begin(), toks.end());
	//cout<< prog.to_string();
	//auto exp_code= genoptexp(exp, symbol(gen_name()));
	//for(auto iter=exp_code.begin(); iter!=exp_code.end(); ++iter){
	//	cout<<iter->to_string()<<endl;
	//}

	auto prog_code= genopt(prog);
	map<string,int> global_const;
	string tcd1= show_mid_code(prog_code);
	auto const_opt_ed= const_opt(prog_code, global_const);
	string tcd2= show_mid_code(const_opt_ed);
	
}
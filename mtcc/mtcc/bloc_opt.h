#pragma once
#include<map>
using namespace std;
#include"bloc_graph.h"


vector<mid_code>&  
	const_bloc_opt(
	vector<mid_code>& block,
	vector<mid_block>& block_item,
	map<string,int>& const_val
	);


mid_block const_opt(mid_block bloc, map<string,int>& const_val){
	switch(bloc.type){
	case mid_block::seqence:
		const_bloc_opt(bloc.code_true, bloc.block_item, const_val);
		break;
	case mid_block::if_stmt:
		if(bloc.test_symbol.is_val){
			bloc.test_symbol.val= bloc.test_symbol.val!=0? 1: 0;
		}else if(const_val.find(bloc.test_symbol.name)!=const_val.end()){
			bloc.test_symbol= symbol(const_val[bloc.test_symbol.name]!=0? 1: 0);
		}
		
		if(bloc.test_symbol.is_val){
			const_bloc_opt(
				bloc.test_symbol.val!=0? bloc.code_true:  bloc.code_false,
				bloc.block_item, const_val);
		}else{
			map<string,int> br_true(const_val.begin(), const_val.end());
			map<string,int> br_false(const_val.begin(), const_val.end());
			
			const_bloc_opt(bloc.code_true, bloc.block_item, br_true);
			const_bloc_opt(bloc.code_false,	bloc.block_item, br_false);

			//交集
			const_val.clear();
			const_val.insert(br_true.begin(), br_true.end());
			const_val.insert(br_false.begin(), br_false.end());
			for(auto i=const_val.begin(); i!=const_val.end();){
				auto j=i;
				++i;
				if(br_true.find(j->first)==br_true.end()){
					const_val.erase(j);
					continue;
				}
				if(br_true[j->first]!=j->second){
					const_val.erase(j);
					continue;
				}
			}
		}
		break;
	case mid_block::repeat_stmt:
		map<string,int> temp_const_map= const_val;
		bool temp_const_test= false;
		for(int i=0; i<120; ++i){
			mid_block temp_bloc= bloc;
			const_bloc_opt(temp_bloc.code_true, temp_bloc.block_item, temp_const_map);
			if(temp_bloc.test_symbol.is_val){
			}else if(temp_const_map.find(temp_bloc.test_symbol.name)!=temp_const_map.end()){
				temp_bloc.test_symbol= symbol(temp_const_map[temp_bloc.test_symbol.name]);
			}else{
				temp_const_test= false;
				break;
			}

			//正常退出
			if(temp_bloc.test_symbol.val==1){
				temp_const_test= true;
				break;
			}
		}
		//todo
		if(temp_const_test){
			/*常量化repeat为if-stmt*/
			bloc= mid_block();
			bloc.type= bloc.if_stmt;
			bloc.test_symbol= symbol(1);
			for(auto iter=temp_const_map.begin(); iter!=temp_const_map.end(); ++iter){
				if(const_val.find(iter->first)==const_val.end() || 
					const_val[iter->first]!=iter->second)
				{
					mid_code temp_code;
					temp_code.type= mid_code::set;
					temp_code.op= mid_code::none;
					temp_code.s0= symbol(iter->first);
					temp_code.s1= symbol(iter->second);
					temp_code.s2.bad= true;
					bloc.code_true.push_back(temp_code);
				}
			}
			const_val= temp_const_map;
		}else{
			const_bloc_opt(bloc.code_true, bloc.block_item, map<string,int>());
			//todo 这里的判断不够精确 暂时先全删掉
			const_val.clear();
		}
		break;
	}
	return bloc;
}


vector<mid_code>&  
	const_bloc_opt(
	vector<mid_code>& block,
	vector<mid_block>& block_item,
	map<string,int>& const_val
	){
		for(auto iter=block.begin();
			iter!=block.end();
			++iter
			){
				switch(iter->type){
				case mid_code::bloc:
					block_item[iter->s0.val]= 
						const_opt(block_item[iter->s0.val], const_val);
					break;
				case mid_code::set:
					if(iter->op==mid_code::none){
						if(iter->s1.is_val){
							const_val[iter->s0.name]= iter->s1.val;
						}else if(const_val.find(iter->s1.name)!=const_val.end()){
							iter->s1= symbol(const_val[iter->s1.name]);
							const_val[iter->s0.name]= iter->s1.val;
						}else{
							const_val.erase(iter->s0.name);
							continue;
						}
					}else{
						int cs1,cs2,res;
						if(iter->s1.is_val){
							cs1= iter->s1.val;
						}else if(const_val.find(iter->s1.name)!=const_val.end()){
							cs1= const_val[iter->s1.name];
						}else{
							const_val.erase(iter->s0.name);
							continue;
						}
						if(iter->s2.is_val){
							cs2= iter->s2.val;
						}else if(const_val.find(iter->s2.name)!=const_val.end()){
							cs2= const_val[iter->s2.name];
						}else{
							const_val.erase(iter->s0.name);
							continue;
						}
						switch(iter->op){
						case mid_code::lt:
							res= cs1<cs2? 1: 0;
							break;
						case mid_code::eq:
							res= cs1==cs2? 1: 0;
							break;
						case mid_code::plus:
							res= cs1+cs2;
							break;
						case mid_code::sub:
							res= cs1-cs2;
							break;
						case mid_code::mul:
							res= cs1*cs2;
							break;
						}
						iter->s1= symbol(res);
						const_val[iter->s0.name]= iter->s1.val;
					}
					break;
				case mid_code::read:
					const_val.erase(iter->s0.name);
					break;
				case mid_code::write:
					if(!iter->s0.is_val){
						if(const_val.find(iter->s1.name)!=const_val.end()){
							iter->s0= symbol(const_val[iter->s1.name]);
						}
					}
					break;
				}
		}
		return block;
}

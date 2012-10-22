#pragma once
#include<map>
using namespace std;
#include"bloc_graph.h"

mid_block const_opt(mid_block bloc, map<string,int>& const_val){
	switch(bloc.type){
	case mid_block::seqence:
		for(auto iter=bloc.code_true.begin(); iter!=bloc.code_true.end(); ++iter){
			switch(iter->type){
			case mid_code::bloc:
				bloc.block_item[iter->s0.val]= const_opt(bloc.block_item[iter->s0.val], const_val);
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
				break;
			case mid_code::write:
				break;
			}
		}
		break;
	case mid_block::if_stmt:
		break;
	case mid_block::repeat_stmt:
		break;
	}
}

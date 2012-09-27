#pragma once

template<typename T>
class plstm{
public:
	virtual~plstm(){}
	plstm& operator<<(plstm<T>&(*pf)(plstm<T>&)){
		return pf(*this);
	}
	plstm& operator>>(plstm<T>&(*pf)(plstm<T>&)){
		return pf(*this);
	}
	virtual void operator<<(T&){}
	virtual void operator>>(T&){}
};
template<typename T>
class plitm{
	T& ref;
public:
	bool alive(){return true;}
	plitm& operator=(T& v){ref=v;}
	operator const T&(){return ref;}
};

template<typename T>
class pllst{
public:
	plitm<T> operator*();

};

template<typename T>
class pllst_random_access: public pllist<T>{
public:
	
};

void unit(){
	pool p;
	for{
	auto key= p.unlock(0,cur,autoinc);
	if(p.isend(key)) break;
	int x= key;
	key=123;
	}
}
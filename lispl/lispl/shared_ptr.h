#pragma once
#if __cplusplus != 201103L



template<typename T>
class shared_ptr{
public:
	shared_ptr(){
		num=0;
		ptr=0;
	}
	template<typename T2>
	shared_ptr(T2* p){
		num=0;
		ptr=cast_ptr(p);
		inc();
	}
	template<typename T2>
	shared_ptr(const shared_ptr<T2>& other){
		num=other.num;
		ptr=cast_ptr(other.ptr);
		inc();
	}

	template<typename T2>
	void swap_to(shared_ptr<T2>& o){
		swap(num,o->num);
		swap(ptr,cast_ptr(o->ptr));
	}

	template<typename T2>
	shared_ptr<T>& operator=(shared_ptr<T2> o){
		swap_to(o);
		return *this;
	}
	template<typename T2>
	bool operator==(const shared_ptr<T2>& o){
		return ptr==cast_ptr(o.ptr);
	}
	template<typename T2>
	bool operator!=(const shared_ptr<T2>& o){
		return ptr!=cast_ptr(o.ptr);
	}

	T* operator->(){return ptr;}


	template<typename T2>
	T* cast_ptr(T2* p){
		T* ret=dynamic_cast<T*>(p);
		if(ret==0&&p!=0){
			string msg("error convert from ");
			msg+= typeid(T2).name();
			msg+= " to ";
			msg+= typeid(T).name();
			plog(error, msg);
			cout<<msg<<endl;
		}
		return ret;
	}
	void inc(){
		if(num==0){
			num= new size_t(0);
		}
		++*num;
	}
	void dec(){
		if(num==0) return;
		--*num;
		if(*num==0){
			delete num;
			delete ptr;
		}
	}
	T* ptr;
	size_t* num;
};


//
////shared_ptr的简单实现版本
////基于引用记数的智能指针
////它可以和stl容器完美的配合
//
//template<class T>
//class shared_ptr
//{
//public:
//	typedef T element_type;
//	typedef T value_type;
//	typedef T * pointer;
//	typedef T& reference;
//	typedef unsigned long size_type;
//
//
//
//	explicit shared_ptr(T* p=0) : px(p){
//		try { pn = new size_type(1); }
//		catch (...) { delete(p); throw; }
//	}
//	shared_ptr& operator= (T* p){
//		if(this->px == p) return *this;
//		dispose();
//		try { pn = new size_type(1); }
//		catch (...) { delete(p); throw; }
//		px=p;
//		return *this;
//	}
//
//
//
//	shared_ptr(const shared_ptr& r) throw(): px(r.px){
//		pn = r.pn;
//		addref();
//	}
//	shared_ptr& operator= (const shared_ptr& r) throw(){
//		if(this == &r) return *this;
//		dispose();
//		px = r.px;
//		pn = r.pn;
//		addref();
//		return *this;
//	}
//	operator T*(){
//		return px;
//	}
//
//
//	template<typename Y> friend class shared_ptr;
//	//为了让有继续关系的shared_ptr类型赋值或构造
//	template<typename Y>
//	shared_ptr(const shared_ptr<Y>& r){
//		px = dynamic_cast<T*>(r.px);
//		if(px!=0){
//			pn = r.pn; // shared_count::op= doesn't throw
//			addref();
//		}else{
//			pn=0;
//		}
//	}
//	template<typename Y>
//	shared_ptr& operator= (const shared_ptr<Y>& r){
//		if(r.px==px) return *this;
//		dispose();
//		px = dynamic_cast<T*>(r.px);
//		if(px!=0){
//			pn = r.pn; // shared_count::op= doesn't throw
//			addref();
//		}else{
//			pn=0;
//		}
//		return *this;
//	}
//
//
//
//	template<typename Y>
//	shared_ptr(Y* py){
//		try { pn = new size_type(1); }
//		catch (...) { delete(py); throw; }
//		px=py;
//	}
//	template<typename Y>
//	shared_ptr& operator= (Y* py){
//		if(this->px == py) return *this;
//		dispose();
//		try { pn = new size_type(1); }
//		catch (...) { delete(py); throw; }
//		px=py;
//		return *this;
//	}
//
//	~shared_ptr() { dispose(); }
//
//
//
//
//
//
//
//
//
//	void reset(T* p=0){
//		if ( px == p ) return;
//		if (--*pn == 0){ delete(px); }
//		else
//		{ // allocate new reference
//			// counter
//			// fix: divvent leak if new throws
//			try { pn = new size_type; }
//			catch (...) {
//				// undo effect of —*pn above to
//				// meet effects guarantee
//				++*pn;
//				delete(p);
//				throw;
//			} // catch
//		} // allocate new reference counter
//		*pn = 1;
//		px = p;
//	} // reset
//
//
//	reference operator*() const throw(){ return *px; }
//	pointer operator->() const throw(){ return px; }
//	pointer get() const throw(){ return px; }
//	size_type use_count() const throw()//
//	{ return *pn; }
//	bool unique() const throw()//
//	{ return *pn == 1; }
//
//private:
//	void dispose() throw(){
//		cout<<"disref()"<<endl;
//		if (--*pn == 0){ delete px; delete pn; }
//	}
//	void addref()throw(){
//		cout<<"addref()"<<endl;
//		if(pn==0){
//			pn=new size_type(1);
//		}else{
//			++*pn;
//		}
//	}
//public:
//	T * px;    // contained pointer
//	size_type* pn;  // reference counter
//
//};  // shared_ptr
//
//
//
//template<typename A,typename B>
//inline bool operator==(shared_ptr<A> const & l, shared_ptr<B> const & r){
//	return l.get() == r.get();
//}
//
//template<typename A,typename B>
//inline bool operator!=(shared_ptr<A> const & l, shared_ptr<B> const & r){
//	return l.get() != r.get();
//}
#endif
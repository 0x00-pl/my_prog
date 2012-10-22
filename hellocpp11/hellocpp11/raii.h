#include<functional>
using namespace std;

class raii{
public:
	function<void()> _enter;
	function<void()> _exit;

	raii(function<void()> enter, function<void()> leave): _enter(enter), _exit(leave){
		_enter();
	}
	~raii(){
		_exit();
	}
};

//��֧�ִ����͵�ģ�����
//template<function<void()> Tenter, function<void()> Texit>
//class raiiT{
//public:
//	raiiT(){
//		Tenter();
//	}
//	~raiiT(){
//		Texit();
//	}
//};

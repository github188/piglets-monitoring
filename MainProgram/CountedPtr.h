#ifndef COUNTER_PRT_H
#define COUNTER_PTR_H
/*
stl 容器提供的是value 语义 而不是 reference 语义，他在内部共建了元素的副本，有时候元素复制的代价过大，就要采用
智能型指针。这是个解决办法，采用的是 reference counting(参考计数)智能型指针。
*/

template <typename T>
class CountedPtr
{
private:
	T* ptr;         //ptr
	long* count; //counting
public:
	explicit CountedPtr(T* p = 0) // 避免 = 的类型转换
		:ptr(p),count(new long(1))
	{}

	CountedPtr(const CountedPtr<T>& p) throw() // 拷贝构造函数，++counter
		:ptr(p.ptr),count(p.count)
	{
		++*count;
	}
	~CountedPtr() throw()
	{
		dispose();
	}
	CountedPtr<T>& operator=(const CountedPtr<T>& p) throw() // =
	{
		if(this != &p)
		{
			dispose();
			ptr = p.ptr;
			count = p.count;
			++*count;
		}
		return *this;
	}

	T& operator*()const throw() // *
	{
		return *ptr;
	}

	T* operator ->()const throw() // ->
	{
		return ptr;
	}

private:
	void dispose()    //dispose
	{
		if(--*count == 0)
		{
			delete count;
			delete ptr;
		}
	}
};
#endif

/*
===================================================================
如何使用countedPtr<T>
===================================================================
*/
//#include <iostream>
//#include <list>
//#include <deque>
//#include <algorithm>
//#include "countptr.h"
//using namespace std;
//
//void PrintCountedPtr(CountedPtr<int> elem)
//{
//	cout <<*elem <<' ';
//}
//
//int main()
//{
//	static int values[] = {3,5,9,1,6,4};
//	typedef CountedPtr<int> IntPtr;
//	deque<IntPtr> coll1;
//	list<IntPtr> coll2;
//
//	for(int i = 0; i <sizeof(values)/sizeof(values[0]);++i)
//	{
//		IntPtr ptr(new int(values[i]));
//		coll1.push_back(ptr);
//		coll2.push_front(ptr);
//	}
//
//	for_each(coll1.begin(),coll1.end(),PrintCountedPtr);
//	cout <<endl;
//	for_each(coll2.begin(),coll2.end(),PrintCountedPtr);
//	cout <<endl <<endl;
//
//	*coll1[2] *= *coll1[2];
//	(**coll1.begin()) *= -1;
//	(**coll2.begin()) = 0;
//
//	for_each(coll1.begin(),coll1.end(),PrintCountedPtr);
//	cout <<endl;
//	for_each(coll2.begin(),coll2.end(),PrintCountedPtr);
//	cout <<endl;
//	return 0;
//}

/*
result
===========================
3 5 9 1 6 4
4 6 1 9 5 3

-3 5 81 1 6 0
0 6 1 81 5 -3
*/
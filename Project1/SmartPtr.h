#pragma once
#include<functional>
#include<thread>
#include<mutex>

using namespace std;
template<class T>
class SmartPtr
{
public:
	// RAII
	SmartPtr(T* ptr)
		:_ptr(ptr)
	{}

	~SmartPtr()
	{
		cout << "~SmartPtr()->" << _ptr << endl;

		delete _ptr;
	}

	// 像指针一样
	T& operator*()
	{
		return *_ptr;
	}

	T* operator->()
	{
		return _ptr;
	}
private:
	T* _ptr;
};

namespace bit
{
	// C++98
	// 管理权转移，最后一个拷贝对象管理资源，被拷贝对象都被置空
	// 很多公司都明确规定了，不要用这个
	template<class T>
	class auto_ptr
	{
	public:
		// RAII
		auto_ptr(T* ptr)
			:_ptr(ptr)
		{}

		~auto_ptr()
		{
			if (_ptr)
			{
				cout << "delete->" << _ptr << endl;
				delete _ptr;
				_ptr = nullptr;
			}
		}

		// ap2(ap1)
		auto_ptr(auto_ptr<T>& ap)
			:_ptr(ap._ptr)
		{
			ap._ptr = nullptr;
		}

		// 像指针一样
		T& operator*()
		{
			return *_ptr;
		}

		T* operator->()
		{
			return _ptr;
		}

	private:
		T* _ptr;
	};


	// C++11 
	template<class T>
	class unique_ptr
	{
	public:
		// RAII
		unique_ptr(T* ptr)
			:_ptr(ptr)
		{}

		~unique_ptr()
		{
			cout << "delete->" << _ptr << endl;

			delete _ptr;
		}

		// 像指针一样
		T& operator*()
		{
			return *_ptr;
		}

		T* operator->()
		{
			return _ptr;
		}

		// C++11
		unique_ptr(const unique_ptr<T>& up) = delete;
		unique_ptr<T>& operator=(const unique_ptr<T>& up) = delete;

	private:
		// C++98
		// 1、只声明不实现
		// 2、限定为私有
		//unique_ptr(const unique_ptr<T>& up);
		//unique_ptr<T>& operator=(const unique_ptr<T>& up);
	private:
		T* _ptr;
	};

	template<class T>
	class shared_ptr
	{
	public:
		// RAII
		shared_ptr(T* ptr = nullptr)
			:_ptr(ptr)
			, _pcount(new atomic<int>(1))
		{}

		template<class D>
		shared_ptr(T* ptr, D del)
			: _ptr(ptr)
			, _pcount(new atomic<int>(1))
			, _del(del)
		{}

		// function<void(T*)> _del;

		void release()
		{
			if (--(*_pcount) == 0)
			{
				//cout << "delete->" << _ptr << endl;
				//delete _ptr;
				_del(_ptr);

				delete _pcount;
			}
		}

		~shared_ptr()
		{
			release();
		}

		shared_ptr(const shared_ptr<T>& sp)
			:_ptr(sp._ptr)
			, _pcount(sp._pcount)
		{
			++(*_pcount);
		}

		// sp1 = sp3
		shared_ptr<T>& operator=(const shared_ptr<T>& sp)
		{
			if (_ptr != sp._ptr)
			{
				release();

				_ptr = sp._ptr;
				_pcount = sp._pcount;

				++(*_pcount);
			}

			return *this;
		}

		// 像指针一样
		T& operator*()
		{
			return *_ptr;
		}

		T* operator->()
		{
			return _ptr;
		}

		int use_count() const
		{
			return *_pcount;
		}

		T* get() const
		{
			return _ptr;
		}

	private:
		T* _ptr;
		//int* _pcount;
		atomic<int>* _pcount;

		function<void(T*)> _del = [](T* ptr) {delete ptr; };
	};

	// shared_ptr本身是线程安全的
	void test_shared_ptr()
	{
		size_t n1 = 10000;
		size_t n2 = 10000;
		mutex mtx;
		shared_ptr<double> sp(new double(1.1));

		atomic<size_t> x = 0;
		// 17:20继续
		thread t1([&]() {
			for (size_t i = 0; i < n1; i++)
			{
				shared_ptr<double> copy1(sp);

				{
					unique_lock<mutex> lock(mtx);
					++(*copy1);
				}

				shared_ptr<double> copy2(sp);

			}
			});

		thread t2([&]() {
			for (size_t i = 0; i < n2; i++)
			{
				shared_ptr<double> copy1(sp);

				{
					unique_lock<mutex> lock(mtx);
					++(*copy1);
				}

				shared_ptr<double> copy2(sp);

			}
			});

		t1.join();
		t2.join();

		cout << sp.use_count() << endl;
		cout << *sp << endl;
	}

	template<class T>
	class weak_ptr
	{
	public:
		weak_ptr()
			:_ptr(nullptr)
		{}

		weak_ptr(const shared_ptr<T>& sp)
			:_ptr(sp.get())
		{}

		weak_ptr<T>& operator=(const shared_ptr<T>& sp)
		{
			_ptr = sp.get();
			return *this;
		}

		// 像指针一样
		T& operator*()
		{
			return *_ptr;
		}

		T* operator->()
		{
			return _ptr;
		}
	private:
		T* _ptr;
	};

}

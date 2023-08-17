#pragma once

namespace csq
{
    template<class T>
    class SmartPtr
    {
    public:
        SmartPtr(T* ptr)
            :_ptr(ptr)
        {}

        ~SmartPtr()
        {
            cout << "delete:" << _ptr << endl;
            delete[] _ptr;
        }

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


//unique指针
    template<class T>
    class unique_ptr
    {
    public:
        unique_ptr(T* ptr)
            :_ptr(ptr)
        {}

        ~unique_ptr()
        {
            if(_ptr)
            {
                cout << "delete" << _ptr << endl;
                delete _ptr;
                _ptr = nullptr;
            }
        }
        
        T* operator*()
        {
            return *_ptr;
        }

        T* operator->()
        {
            return _ptr;
        }

        unique_ptr(const unique_ptr<T>& sp) = delete;
        unique_ptr<T>& operator=(const unique_ptr<T>& sp) = delete;
    private:
        T* _ptr;
    };


    template<class T>
    class shared_ptr
    {
    public:

        void Release()
        {
            if(--(*_pCount) == 0 && _ptr)
            {
                cout << "delete" << _ptr << endl;
                delete _ptr;
                _ptr = nullptr;


                delete _pCount;
                _pCount = nullptr;
            }
        }
        //RAII思想
        shared_ptr(T* ptr)
            :_ptr(ptr)
            ,_pCount(new int(1))
        {}
        
        ~shared_ptr()
        {
            Release();
        }

        shared_ptr(const shared_ptr<T>& sp)
            :_ptr(sp._ptr)
            ,_pCount(sp._pCount)
        {
            (*_pCount)++;
        }

        //sp1 = sp3
        shared_ptr<T>& operator=(const shared_ptr<T>& sp)
        {
            if(_ptr != sp._ptr)
            {
                Release();
                
                _ptr = sp._ptr;
                _pCount = sp._pCount;
                ++(*_pCount);
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

		T* get()
		{
			return _ptr;
		}

    private:
        T* _ptr;
        int* _pCount;

    };
} 
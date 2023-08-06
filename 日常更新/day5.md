#  友元类

比如我的Date类里面有个函数想访问Time类里面的私有变量。这时候我Time类里面可以把Date类设置为他的朋友。那么作为Time类的朋友Date类里面就可以访问Time了。

```
#include <iostream>
using namespace std;
class Time
{
     friend class Date;
private:
    int _hour;
    int _minute;
    int _second;
};
class Date
{
   
    friend ostream& operator<<(ostream& out, const Date& d);
public:
    Date(int year = 0, int month = 0, int day = 0)
    :_year(year)
    ,_month(month)
    ,_day(day)
    {}

    void SetTime(int hour, int minute, int second)
    {
        _t._hour = hour;
        _t._minute = minute;
        _t._second = second;
    }
private:
    int _year;
    int _month;
    int _day;

     Time _t;
};

ostream& operator<<(ostream& out, const Date& d)
{
    out << d._year << d._month << d._day << endl;
}
int main()
{
    Date d1;
   
    cout << d1;
    return 0;
}
```

友元是一个单向关系。总的来说： **谁想访问我，谁就要成为我的朋友**

假如有一个全局函数要用到这两个类的私有变量。那就把这个函数变为这两个类的朋友就行。

#   内部类

内部类，假如一个类是内部类，那这个类天生就是友元类。

![image-20230806131600004](E:\markdown\图片\image-20230806131600004.png)

C++几乎不用内部类。

#  匿名对象

声明周期只在那一行。



#  虚拟地址进程空间

栈 堆 静态区 常量区(代码段)

函数内部定义的变量都是在栈上开辟的。

![image-20230806140509959](E:\markdown\图片\image-20230806140509959.png)

但是* char2在栈上，原因是 * char2是开辟的数组，是把abcd拷贝到栈上了。所以* char2是在栈上。

但是* pChar3是在常量区，因为pChar3是指针，指向常量区。

#  malloc  calloc  realloc的区别

malloc就是开空间

calloc就是开空间+初始化

realloc是扩容，分为原地扩和异地扩。

为什么C++要用new？

malloc只开空间，new开空间+调用构造函数进行初始化。

#  struct

struct里面的成员默认都是public的，一般节点什么的需要大家公用的都用struct来定义。

 

#   new

new失败会抛异常

![image-20230806152524285](E:\markdown\图片\image-20230806152524285.png)

抛异常用try catch捕获后就不会报错了，会把错误打印出来。遇到异常，就会调到catch语句里。

#  operator new

就是封装了malloc，然后实现了抛异常。以前是malloc失败直接报错，现在抛异常。

用的方法和malloc没有区别。只是不用检查是否失败了。

new会先去调用operator new, 然后operator new再去调用malloc。



#   定位new

![image-20230806160434912](E:\markdown\图片\image-20230806160434912.png)

#  new和malloc的区别

new竟然是操作符。

![image-20230806161038537](E:\markdown\图片\image-20230806161038537.png)

#  内存泄露的危害

![image-20230806161343501](E:\markdown\图片\image-20230806161343501.png)

#  模板的原理

##   函数模板原理 



在C++中，函数模板的参数类型推断通常是自动的，你不需要显式指定模板参数类型。

![image-20230806162829559](E:\markdown\图片\image-20230806162829559.png)

##  类模板

函数模板可以自己推到，类模板不能自己推到，需要自己指定类型。

```
template<class T>
class stack
{
private:
	T* _a;
};
int  main()
{
	Stack<int> st1;
}
```

#   模板和定义不能进行分离

因为模板的定义里面没有调用。他并不知道自己的类型。所以没法编译。 

放在一起的话就没有链接错误，因为放在一起后，就算展开了，也会在调用的地方展开。这时候都在一个文件中，根本不需要在链接的时候找函数的地址了。直接就能call地址了，所以不需要分离。 

#  string模板

string看起来不是模板，其实他也就是模板，只是被typedef过的。

![image-20230806205654277](E:\markdown\图片\image-20230806205654277.png)
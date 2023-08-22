day21C++IO流

##  while(cin >> str) operator bool

cin >> str实质是调用了operator>>(cin, str);

这个函数返回值是istream，也就是cin，那么cin怎么做条件判断？

cin本身不能，但是C++支持隐士类型的转换。

所以有了对bool的重载。这点比较奇怪。operator bool支持把自定义类型转化为内置类型。所以也可以把cin也转换为bool，这样就可以进行条件判断了。



![image-20230822210127721](E:\markdown\图片\image-20230822210127721.png)

![image-20230822205707118](E:\markdown\图片\image-20230822205707118.png)

好多oj题是为了多组数据才用这个代码。

scanf如果读取成功返回的是字节数。

![image-20230822205030458](E:\markdown\图片\image-20230822205030458.png)

##  C++文件IO流

##  ifstream

进行读文件，用法需要先构造对象。

![image-20230822212505619](E:\markdown\图片\image-20230822212505619.png)

  ifs也可以这样像cin用，因为是继承自cin。>>读不了空格和换行。

 
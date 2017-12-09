/*
 *  RiOS/kernel/maincpp.cpp
 *
 *  Copyright (C) 2017  Curie （邱日）
 */
/*为了能使main.c能够用上C++*/
//为了使RiOS支持C++编程，我颇费了一些工夫
//Linux 中C文件调用C++代码的方法
//reference:http://blog.csdn.net/wutong_login/article/details/6600780

class MyClass
{
public:
    int add(int a, int b)
       {
              return (a + b);
       }
};

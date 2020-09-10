# QT开发过程问题解决方法汇总

## int转QString

```cpp
int i = 5;
QString s = QString::number(i);
```

## QString 转 int

```c++
QString str = “100”;
int tmp = str.toInt();
```

## QString 转换为 char *

将 QString 转 char *，需要用到 QByteArray 类，QByteArray 类的说明详见 Qt 帮助文档。 

==[法一]==

因为 char * 最后都有一个`'\0'`作为结束符，而采用 QString::toLatin1() 时会在字符串后面加上`'\0'`。

```cpp
QString  str;
char*  ch;
QByteArray ba = str.toLatin1(); // must
ch=ba.data();
```

这样就完成了 QString 向 char * 的转化。经测试程序运行时不会出现 bug。注意第3行，一定要加上，不可以 str.toLatin1().data() 这样一步完成，否则可能会出错。 

==[法二]==

```c++
//QString -> char* 先转成QByteArray 再转 char*
QString str;
char * ch;
ch = str.toUtf8().data();
```



## 中文乱码

在头文件前加入下面的代码

```c++
#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif
```

## 4k对齐问题

解决方法：重新定义原来的结构体，使其大小为4的倍数即可。


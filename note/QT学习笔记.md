# QT学习笔记

## 一、文档结构介绍

默认创建有窗口类，myWidget，基类有三种选择：QWidget, QMainWindow. QDialog，这三个基类的继承关系如下

<img src="https://s1.ax1x.com/2020/09/03/wiQhRg.png" style="zoom: 50%;" />

其中QWidget仅包含一个窗口，QMainWindow在QWidget基础上增加了菜单栏和状态栏，QDialog是对话框，没有最大、最小化按钮。

在创建好的项目中，有一个main.cpp的文件，其中包含一个main函数，这是应用程序的入口，具体代码详解如下：

```c++
#include "mywidget.h"//自己的头文件
#include <QApplication>//包含一个应用程序类的头文件

//main应用程序入口， argc为命令行变量的数量， argv为命令行变量的数组
int main(int argc, char *argv[])
{
    //a 为应用程序对象，在Qt中，应用程序对象有且仅有一个
    QApplication a(argc, argv);
    //窗口对象，myWidget父类为QWidget
    myWidget w;
    //窗口对象，默认不会显示，必须要调用show方法显示窗口
    w.show();
    
    //让应用程序对象进入消息循环
    //让代码阻塞到这行
    return a.exec();
}
```

## 二、创建按钮与窗口控制常用API

在`mywidget.cpp`中的`myWidget`类的构造函数中：

```c++
myWidget::myWidget(QWidget *parent):QWidget(parent)
{
    //创建一个按钮(法一)
    QPushButton *btn = new QPushButton;
    //让btn对象依赖在myWidget窗口中，如果使用btn->show()，那么将以顶层的方式弹出窗口控件
    btn->setParent(this);
    
    //显示文本
    btn->setText("第一个按钮");
    
    //创建第二个按钮 ， 按照控件的大小创建窗口
    QPushButton *btn2 = new QPushButton("第二个按钮", this);
    //移动btn2按钮到x=100,y=100处
    btn->move(100,100);
    
    //重置窗口大小，这个仅是设置默认窗口大小，用户仍然可以通过拖拽的方式改变窗口大小
    resize(600, 400);
   	//设置固定窗口大小，用户不可自己改变窗口大小
    setFixedSize(600,400);
    
    //设置窗口标题
    setWindowTitle("这是窗口标题");
    
}
```

## 三、对象树

3.2对象模型(对象树)
在QT中创建对象的时侯会提供一个 Parent对象指针，下面来解释这个 parent到底是干什么的。

* QObject是以对象树的形式组织起来的。

  * 当你创建一个QObject对象时，会看到QObject的构造函数接收一个QObject指针作为参数，这个参数款是 perent，也就是父对象指针。
    这相当于，在创建 QObject对象时，可以提供一个其父对象，我们创建的这个QObject对象会自动添加到其父对象的 children()列表

  * 当父对象析构的时候，这个列表中的所有对象也会被析构。(注意，这里的父对象并不是继承意义上的父类！)
    这种制在GUI程序设计中相当有用。例如，一个按钮有一个QShortcut(快捷键)对象作为其子对象。当我们删除按钮的时候，这个快捷键理应被删除。这是合理的。

* QWidget是能够在屏幕上显示的一切组件的父类。
  * QWidget继承自QObject,因此也维承了这种对象树关系。一个孩子自动地成为父组件的一个子组件。区此,它会显示在父组件的坐标系统中，被父组件的边界剪裁。例如，当用户关闭一个对话框的时候,应用程序将其是除,那么,我们希望属于这个对话框的按钮、图标等应该一起被删除。事实就是如此,国为这些都是对话框的子组件。
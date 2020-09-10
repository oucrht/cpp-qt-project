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



## 四、信号和槽

#### signals: 

1. 自定义信号， 写到头文件的signals下
2. 返回值是`void`，只需要声明，不需要实现
3. 可以有参数，可以重载。

#### 槽函数：

1. 高版本可写到public或全局下
2. 返回值`void`，需要声明，需要实现
3. 可以有参数，可以发生重载

#### 触发自定义的信号：

`emit` 自定义信号

#### 当自定义信号和槽出现重载：

需要利用函数指针 明确指向函数的地址

```c++
void (Teacher::*tSignal)(QString) = &Teacher::hungry;
```

#### 拓展：

1. 信号可以连接信号
2. 一个信号可以连接多个槽函数
3. 多个信号可以连接一个槽函数
4. 信号和槽函数的参数 类型必须一一对应
5. 信号和槽的参数个数 可以不一致，但信号的参数个数必须大于等于槽的参数个数

## 补充知识点: C++11 lambda表达式

[](./C++lambda表达式.md)

## 五、定时器

#### 步骤

利用定时器类`QTimer`

创建定时器对象

```c++
QTimer *timer = new QTimer(this);
```

启动定时器`timer->start(毫秒数)`

每隔一定毫秒，发送信号，信号为：`timeout`,

暂停是`timer->stop()`

## 六、事件分发

![](D:\data\github\cpp-qt-project\note\QT学习笔记.assets\未命名绘图.svg)

#### 通过Event事件，拦截鼠标点击事件

```c++
bool myLabel::event(QEvent *e)
{
    //如果是鼠标按下，在event事件分发中做拦截操作
    if(e->type()==QEvent::MouseButtonPress)
    {
        QMouseEvent *ev = static_cast<QMouseEvent *>(e);//静态类型转换
        QString str = QString("Event函数中, 鼠标按下了...  ");
        qDebug() << str;
        return true;//true代表自己处理这个事件，不向下分发
    }
    //其他事件 交给父类处理 ，默认处理
    return QLabel::event(e);
}
```

当鼠标点击相应区域时，会打印`"Event函数中, 鼠标按下了... "`

## 七、绘图事件

```c++
void Widget::paintEvent(QPaintEvent *)
{
    //实例化画家对象，this指定的是绘图设备
    QPainter painter(this);//表示再Widget窗口中画
    
    //画线
    painter.drawLine(QPoint(0,0), QPoint(100,100));
    //其他函数可查手册
    
    
}
```

#### 绘图时使用的坐标示意

![](D:\data\github\cpp-qt-project\note\QT学习笔记.assets\未命名绘图2.svg)

#### 设置抗锯齿：

```c++
//在上面的函数中添加
painter.setRenderHint(QPainter::Antialiasing);//其他参数可查手册
//改设置对之后的语句有效，对之前的无效
```

#### 移动画家位置：

```c++
painter.translate(100,0;)//注意移动的是画家的位置
```

![](D:\data\github\cpp-qt-project\note\QT学习笔记.assets\2020-09-10_194335.png)

<img src="D:\data\github\cpp-qt-project\note\QT学习笔记.assets\2020-09-10_194116.png" style="zoom:70%;" />

#### 用painter画图片：

```c++
painter.drawPixmap(x坐标,y坐标,QPixmap("图片路径"));//具体参数查手册
```

#### 如果要手动调用绘图事件，用`update()`更新：

一个小栗子：

通过按钮来控制画家位置。其中x是当前类的一个成员变量，每当按钮被点击时，x增加40,并更新绘图事件。具体效果见动图.

<img src="D:\data\github\cpp-qt-project\note\QT学习笔记.assets\2020-09-10_200139.png" style="zoom:60%;" />

<img src="D:\data\github\cpp-qt-project\note\QT学习笔记.assets\1.gif" style="zoom:67%;" />

## 八、绘图设备

绘图设备是指继承`QPainterDevice`的子类. Qt一共提供了四个这样的类，分别为：`QPixmap`, `QBitmap`, `QImage`, `QPicture`。其中：

* QPixmap专门为图像在屏幕上的显示做了优化
* QBitmap是QPixmap的一个子类，它的色深限定为1
* QImage专门为图像的像素级访问做了优化
* QPicture则可以记录和重现QPainter的各条命令

#### 利用QPixmap往磁盘上画图：

```c++
//以下代码在Widget的构造函数中
QPixmap pix(300,300);
//填充颜色，默认是黑色
pix.fill(Qt::white);//填充为白色
//声明画家
QPainter painter(&pix);
painter.setPen(Qpen(Qt::green));//绿色画笔
painter.drawEllipse(QPoint(150,150),100,100);

pix.save("D:\\test.png");
```

#### QImage绘图设备

```c++
//以下代码在Widget的构造函数中
QImage img(300,300,QImage::Format_RGB32)；//参数查手册
img.fill(Qt::white);

QPainter painter(&img);
painter.setPen(QPen(Qt::blue));
painter.drawEllipse(QPoint(150,150),100,100);

//保存在磁盘上
img.save("D:\\img.png");
```

对图像的像素点进行修改：

```c++
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QImage img;
    img.load("D:\\test.jpg");

    for(int i=100;i<150;i++)
    {
        for(int j=100;j<150;j++)
        {
            QRgb rgb = qRgb(255,0,0);//红色
            img.setPixel(i,j,rgb);
        }
    }
    painter.drawImage(0,0,img);

}
```

<img src="D:\data\github\cpp-qt-project\note\QT学习笔记.assets\2020-09-10_204023.png" style="zoom:50%;" />
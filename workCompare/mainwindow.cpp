#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QTextCodec>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置窗口固定大小
    setFixedSize(830,650);
    //设置窗口标题
    setWindowTitle("作业未交情况查询_by小帅1.0");
    //设置图标资源
    setWindowIcon(QIcon(":/res/app.ico"));
    ui->pushButton->setStyleSheet("QPushButton{border:none;color:rgb(0, 0, 0);background-color: rgb(230, 230, 230);border-radius:20px;}"
                                  "QPushButton:hover{background-color: rgb(251, 139, 200);border:none;color:rgb(255, 255, 255);}"
                                  "QPushButton:checked{background-color: rgb(20, 62, 134);border:none;color:rgb(255, 255, 255);}");

    ui->pushButton_2->setStyleSheet("QPushButton{border:none;color:rgb(0, 0, 0);background-color: rgb(240, 240, 240);border-radius:20px;}"
                                  "QPushButton:hover{background-color: rgb(251, 139, 200);border:none;color:rgb(255, 255, 255);}"
                                  "QPushButton:checked{background-color: rgb(20, 62, 134);border:none;color:rgb(255, 255, 255);}");

    ui->textEdit->setStyleSheet("background:transparent;border-width:2;border-style:outset;border-radius:15px;");
    ui->textEdit_2->setStyleSheet("border-width:3;border-style:outset;border-radius:5px;");

    ui->label->setStyleSheet("background-color: rgb(230, 230, 230);");
    ui->label_2->setStyleSheet("background-color: rgb(230, 230, 230);");

    //程序开始

    //存取数据变量
    static QString listAlready = ""; //已交文件 ：listAlready
    static QStringList listAllName; //存取班级姓名 ：listAllName


    //点击按钮pushButton_2：打开存取班级姓名的文件
    connect(ui->pushButton_2,&QPushButton::clicked,[=](){
        QString filePath = QFileDialog::getOpenFileName(this,"打开文件","","*.txt");
        qDebug()<<filePath;


        if(filePath.isEmpty())
        {
            QMessageBox::warning(this,"警告","没有打开文件，无法检测交作业的情况");
            return;
        }
        else
        {
            //清空文件数组
            listAllName.clear();
            //QMessageBox::information(this, "成功打开" ,"姓名文件已保存，请进行第二步");
            setWindowTitle("成功打开，姓名文件已保存，请进行第二步");

            //将内容读取到文本框中
            //1、实例化文件对象
            QFile file(filePath);
            //2、指定打开方式
            file.open(QIODevice::ReadOnly);
            //3、将内容读入变量中
            //array = file.readAll(); //一次读完

            //4、读取文件到数组，【需要先读到QString，再分割到数组内】
            QString str = file.readAll();
            listAllName = str.split("\r\n"); // 注意，如果str是空字符串，list1会增加一个空字符串到列表里，其size=1，我为此吃过苦头～

            //5、关闭文件
            file.close();
        }

    });


    //点击按钮pushButton：打开已交作业文件的绝对路径
    connect(ui->pushButton, &QPushButton::clicked, [=](){
        //清空文本框
        ui->textEdit->setText("");
        //清空数组
        listAlready.clear();


        if(listAllName.isEmpty())
        {
            ui->textEdit->setText(""); //清空文本框
            QMessageBox::warning(this,"警告","请先打开存取班级姓名的文件，否则将无法进行检测");
            return;
        }

        //将目录读取到链表中
        QStringList file_names = QFileDialog::getOpenFileNames(this,QStringLiteral("选择文件"),"");
        if(file_names.isEmpty())
        {
            QMessageBox::warning(this,"警告","没有打开文件，无法检测交作业的情况");
            return;
        }
        if(!file_names.isEmpty())
        {
           for(auto file_name : file_names) //for(auto 变量:数组)：对于数组中的每个值都放到变量中，C++11新特性
           {
                listAlready.append(file_name).append('\n'); //将变量添加到数组中
           }

           //所有已交文件的路径显示到文本框内
           ui->textEdit->setText(listAlready);
        }

        //判断：已交文件的路径名 是否包含 每个同学的名字
        QString s = "";  //保存未交的名字
        for(int i = 0; i<listAllName.size(); ++i)
        {

            if(!listAlready.contains(listAllName.at(i)))
            {
                //qDebug()<<"没交作业的人：";
                //qDebug()<<listAllName.at(i);
                s = s + listAllName.at(i) + "\t";
            }
            //---------contains有小bug，通过如下代码解决-----------
            if(listAlready.contains(listAllName.at(i)))
            {
                //移除已交名字的路径：最终的小bug---------周星驰 名字必须要在 周星 前先进行比较----------
                listAlready.remove(listAllName.at(i));
            }
        }
        //没交作业的显示到文本框中
        ui->textEdit_2->setText(s);

        setWindowTitle("作业未交情况查询_by小帅1.0");

        if(s == "")
        {
            //qDebug()<<"作业全交";
            ui->textEdit_2->setText("恭喜，作业已全交");
        }

    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

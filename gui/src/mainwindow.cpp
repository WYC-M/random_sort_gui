#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QTextBrowser>
#include <QFile>
#include <QString>
#include <QMessageBox>
#include <QTextStream>
#include <QIODevice>
#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include <QTextCharFormat>
#include <QSpinBox>

#include <windows.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置 textbroser 中的文本大小
    QTextCharFormat initializeFormat;
    initializeFormat.setFontPointSize(14);
    ui->textBrowser->setCurrentCharFormat(initializeFormat);

    setWindowTitle("Random Sort GUI"); // 设置标题

    // 初始化 spinBox
    ui->spinBox->setPrefix("字号: ");
    ui->spinBox->setRange(5, 50);
    ui->spinBox->setValue(14);

    connect(ui->btnSort,&QPushButton::clicked,[=](){
        ui->textBrowser->clear(); // 清除文本框内文本
        // ui->btnSort->setText("请稍候");

        int errNum=0; // 报错数量
        if(QFileInfo::exists("core.exe")==false) // core.exe 不存在时
        {
            QTextCharFormat errCoreFormat;
            errCoreFormat.setFontPointSize(9);
            ui->textBrowser->setCurrentCharFormat(errCoreFormat);

            QMessageBox::critical(this,"无法启动主程序","无法启动 core.exe"); // 判断 core.exe 是否存在
            ui->textBrowser->insertPlainText("错误：无法启动 core.exe ，请重新安装程序\n");
            errNum++;

            errCoreFormat.setFontPointSize(14);
            ui->textBrowser->setCurrentCharFormat(errCoreFormat);
        }
        if(QFileInfo::exists("list.txt")==false) // list.txt 不存在时
        {
            QTextCharFormat errListFormat;
            errListFormat.setFontPointSize(9);
            ui->textBrowser->setCurrentCharFormat(errListFormat);

            QMessageBox::critical(this,"无法打开文本文件","无法打开 list.txt"); // 判断 list.txt 是否存在
            ui->textBrowser->insertPlainText("错误：无法打开 list.txt ，请重新安装程序\n");
            errNum++;

            errListFormat.setFontPointSize(14);
            ui->textBrowser->setCurrentCharFormat(errListFormat);
        }
        if(errNum>=1)
        {
            return; // 当报错数大于等于1时，return
        }


        if(QFileInfo::exists("data.txt")==true) QFile::remove("data.txt"); // 若 data.txt 存在，则删除

        // 打开 core.exe （生成 data.txt）
        // system("start core.exe");
        WinExec("core.exe",SW_SHOW);

        // 检测 core.exe 是否生成文件完毕
        while(1)
        {
            if(QFileInfo::exists("data.txt")==true) break;
            Sleep(200);
        }
        // Sleep(1000);

        // 读取 data.txt
        QString content=""; // data.txt 内文本
        QString path=QDir::currentPath()+"/data.txt"; // 生成 data.txt 绝对路径
        QFile file(path); // 实例化对象
        if (file.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QTextStream in(&file);  // 使用 QTextStream 读取文本文件
            content = in.readAll();
            file.close();
        }
        else // 无法正常打开时
        {
            QTextCharFormat errDataFormat;
            errDataFormat.setFontPointSize(9);
            ui->textBrowser->setCurrentCharFormat(errDataFormat);

            QMessageBox::critical(this,"无法打开文本文件","无法打开 data.txt"); // 判断 list.txt 是否存在
            ui->textBrowser->insertPlainText("错误：无法打开 data.txt ，请重新安装程序\n");

            errDataFormat.setFontPointSize(14);
            ui->textBrowser->setCurrentCharFormat(errDataFormat);

            return;
        }

        ui->textBrowser->insertPlainText(content); // 显示文本
        ui->btnSort->setText("重新排序");
    });

    connect(ui->btnEdit,&QPushButton::clicked,[=](){
        if(QFileInfo::exists("list.txt")==false) // list.txt 不存在时
        {
            QTextCharFormat errListFormat;
            errListFormat.setFontPointSize(9);
            ui->textBrowser->setCurrentCharFormat(errListFormat);

            QMessageBox::critical(this,"无法打开文本文件","无法打开 list.txt");
            ui->textBrowser->insertPlainText("错误：无法打开 list.txt ，请重新安装程序\n");

            errListFormat.setFontPointSize(14);
            ui->textBrowser->setCurrentCharFormat(errListFormat);

            return;
        }
        else QDesktopServices::openUrl(QUrl("list.txt")); // 打开 list.txt
    });

    connect(ui->spinBox,&QSpinBox::valueChanged,[=](){
        int currentValue = ui->spinBox->value(); // 获取 spinBox 当前值

        // 调节字号
        QTextCharFormat format;
        format.setFontPointSize(currentValue);
        ui->textBrowser->setCurrentCharFormat(format);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QPushButton>
#include <QSplitter>
#include <qevent.h>
#include <QMimeData>
class QSplitter;
class QTreeWidget;
class QTableView;
class QStandardItemModel;
class QPushbutton;
class QMenu;
class QToolBar;
class DomParser;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
   QTreeWidget *treeWidget;             //树形区
   QTableView *tableView;				//表格区
   //QPushButton * btn;
   QSplitter *splitter;                 //分割窗口
   DomParser *parser;					//XML解析
   QStandardItemModel *model;			//数据模型

private:
   void setLayoutOfMainWindow();
   void createQtreewidget();
   void createButton();
   void createAction();
   void createMenu();
   void createToolBar();
   void deleteAll();
   void readSettings();
   void writeSettings();

private slots:
   bool loadFile(const QString &fileName);
   void updateRecentFileActions();
   void setCurrentFile(const QString &fileName);
   void tableWidgetInitial(QTreeWidgetItem *item,int column);	//窗口布局配置
   void slotAbout();
   void slotAboutQt();
   void slotOpenFile();
protected:
    void closeEvent(QCloseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
private:
    QAction *openAction;
    QAction *saveAction;
    QAction *AboutAction;
    QAction *AboutQtAction;
    QAction *exitAction;
    //QAction *testAction;
    enum { MaxRecentFiles = 5 };
    QAction *recentFileActions[MaxRecentFiles];
    QStringList recentFiles;
    QString curFile;

    QMenu* fileMenu;
    QMenu* helpMenu;
    QToolBar* toolBar;
    QAction *separatorAction;
    QString strippedName(const QString &fullFileName);
    QMap<QString, QMap< QString, QString > > _NodeMap;//存储节点和其全部属性
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

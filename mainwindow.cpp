#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QFile>
#include <QFileInfo>
#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>
#include <domparse.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //ui->setupUi(this);
    setLayoutOfMainWindow();
    //createButton();
    //createQtreewidget();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setLayoutOfMainWindow()
{
        setWindowTitle(tr("XMLReader"));
        setWindowIcon(QIcon(":/images/XmlIcon.png"));
        resize(680,480);

        createAction();
        createMenu();
        createToolBar();
        readSettings();

        tableView = new QTableView(this);
        tableView->setAcceptDrops(true);
        model = new QStandardItemModel(0,2);								//设置表格列数
//        for (int row = 0; row < 4; ++row) {
//            for (int column = 0; column < 4; ++column) {
//                QStandardItem *item = new QStandardItem(QString("row %0, column %1").arg(row).arg(column));
//                model->setItem(row, column, item);
//            }
//        }
        tableView->setSortingEnabled(true);									//开启排序
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);		//只选中整行
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);	//单行选中
        tableView->setShowGrid(true);										//显示网格
        tableView->verticalHeader()->hide();								//隐藏垂直表头编号
        //tableView->horizontalHeader()->hide();							//隐藏水平表头编号

        tableView->setAlternatingRowColors(true);							//设置行色
        tableView->horizontalHeader()->setStretchLastSection(true);			//对列进行设置，使其完全填充
        model->setHeaderData(0,Qt::Horizontal,QObject::tr("标签属性"));
        model->setHeaderData(1,Qt::Horizontal,QObject::tr("属性值"));

        treeWidget = new QTreeWidget(this);
        treeWidget->setAcceptDrops(true);
        treeWidget->header()->setVisible(false);
        treeWidget->header()->setSectionResizeMode(QHeaderView::Stretch);
        connect(treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(tableWidgetInitial(QTreeWidgetItem*,int)));

        splitter = new QSplitter(this);
        splitter->addWidget(treeWidget);
        splitter->setStretchFactor(0,30);
        splitter->addWidget(tableView);
        splitter->setStretchFactor(1,70);
        splitter->autoFillBackground();
        setCentralWidget(splitter);
        //tableView = new QTableView(this);
        //tableView->setGeometry(150,150,200,200);
        //tableView->resize(180,280);
        //tableView->setAcceptDrops(true);
        //model = new QStandardItemModel(0,2);
}

void MainWindow::createAction()
{
    openAction = new QAction(tr("&Open"),this);
    openAction->setShortcut(QKeySequence::Open);
    openAction->setIcon(QIcon(":/images/open.png"));//打开按钮
    connect(openAction,SIGNAL(triggered()),this,SLOT(slotOpenFile()));


    saveAction = new QAction(tr("&Save"),this);
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setIcon(QIcon(":/images/save.png"));//保存按钮

    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()),
            this, SLOT(slotOpenRecentFile()));
    }

    exitAction = new QAction(tr("&Exit"),this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setIcon(QIcon(":/images/Exit.png"));//退出按钮
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    AboutAction = new QAction(tr("&About"), this);
    AboutAction->setStatusTip(tr("Show the application's About box"));
    connect(AboutAction, SIGNAL(triggered()), this, SLOT(slotAbout()));

    AboutQtAction = new QAction(tr("About &Qt"), this);
    AboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
    connect(AboutQtAction, SIGNAL(triggered()), this, SLOT(slotAboutQt()));

}

void MainWindow::createMenu()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    separatorAction = fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        fileMenu->addAction(recentFileActions[i]);
    }
    fileMenu->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(AboutAction);
    helpMenu->addAction(AboutQtAction);

}

void MainWindow::createToolBar()
{
    toolBar = addToolBar(tr("&Filebak"));
    toolBar->addAction(openAction);
    toolBar->addAction(saveAction);
    toolBar->addSeparator();
    toolBar->addAction(exitAction);
}

void MainWindow::readSettings()
{
    QSettings settings("Software Inc.", "XMLReader");
    recentFiles = settings.value("recentFiles").toStringList();
    updateRecentFileActions();
}

void MainWindow::writeSettings()
{
    QSettings settings("Software Inc.", "XMLReader");
    settings.setValue("recentFiles", recentFiles);
}


void MainWindow::closeEvent(QCloseEvent *event)
{
        writeSettings();
        event->accept();
}

void MainWindow::updateRecentFileActions()
{
    QMutableStringListIterator i(recentFiles);
    while (i.hasNext()) {
        if (!QFile::exists(i.next()))
            i.remove();
    }

    for (int j = 0; j < MaxRecentFiles; ++j) {
        if (j < recentFiles.count()) {
            QString text = tr("&%1 %2")
                .arg(j + 1)
                .arg(strippedName(recentFiles[j]));
            recentFileActions[j]->setText(text);
            recentFileActions[j]->setData(recentFiles[j]);
            recentFileActions[j]->setVisible(true);
        } else {
            recentFileActions[j]->setVisible(false);
        }
    }
    separatorAction->setVisible(!recentFiles.isEmpty());
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::slotOpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open XML File"),
                                    ".",
                                    tr("XML files (*.xml *)"));
    if (fileName.isEmpty())
    {
        QMessageBox::information(this,tr("XMLREader"),tr("Open Error"));
    }

    parser = new DomParser(treeWidget);
    parser->readFile(fileName);

}

bool MainWindow::loadFile(const QString &fileName)
{
    parser = new DomParser(treeWidget);
    if (!parser->readFile(fileName)) {
        statusBar()->showMessage(tr("Loading canceled"), 2000);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowModified(false);

    QString shownName = tr("Untitled");
    if (!curFile.isEmpty()) {
        shownName = strippedName(curFile);
        recentFiles.removeAll(curFile);
        recentFiles.prepend(curFile);
        foreach(QWidget *win,QApplication::topLevelWidgets())
        {
            if (MainWindow *mainWin = qobject_cast<MainWindow *>(win))
            {
                mainWin->updateRecentFileActions();
            }

        }
    }

    setWindowTitle(tr("%1[*] - %2").arg(shownName)
        .arg(tr("XMLReader")));
}

void MainWindow::slotAbout()
{
    QMessageBox::about(this, tr("About XMLReader"),
        tr("<h2>XMLReader 1.1</h2>"
        "<p>Copyright &copy; 2017 Software Inc."
        "<p>XML file Reader! WellCome appreciation!"
        "<P>Author:Fanyun"
        "<p>Data:2012-06-12"
        "<p> CodingRoom"));
}

void MainWindow::slotAboutQt()
{
    QMessageBox::about(this, tr("About XMLReader"),
    tr("<h2>Reader 1.2</h2>"
       "<p>Reader data; 2017 "));
}

void MainWindow::tableWidgetInitial(QTreeWidgetItem *item, int column)
{
    _NodeMap = parser->getNodeMap();

    tableView->setModel(model);			//设置数据模型

    QModelIndex index;
    int i =0;
    QMapIterator<QString,QMap<QString,QString> > iter(_NodeMap);
    while (iter.hasNext())
    {
        iter.next();
        if (iter.key() == item->text(column))//如果当前标签名与
        {
            const QMap<QString,QString>& keyValues = iter.value();
            model->setRowCount(model->rowCount() + keyValues.count());			//i行2列
            //model->setColumnCount(2);
            //model->setRowCount(2);											//2行i列
            //model->setColumnCount(model->columnCount()+keyValues.count());
            QMapIterator<QString,QString> it(keyValues);
            while (it.hasNext())
            {
                it.next();
                index = model->index(i,0);										//i行2列
                //index = model->index(0,1);									//2行i列

                model->setData(index,it.key());									//标签属性名
                index = model->index(i,1);										//i行2列
                //index = model->index(1,i);									//2行i列
                QString varCommand = it.value();								//标签属性值
                model->setData(index,varCommand);
                model->setData(index,varCommand,Qt::UserRole);
                i++;
            }
        }
    }
    _NodeMap.clear();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();

    if (urls.isEmpty())
    {
        return ;
    }

    QString fileName = urls.first().toLocalFile();
    if (fileName.isEmpty())
    {
        return ;
    }

    parser = new DomParser(treeWidget);
    if (!parser->readFile(fileName))
    {
        return ;
    }
}

//void MainWindow::createQtreewidget()
//{
//    setWindowTitle(tr("XMLReader"));
//    setWindowIcon(QIcon(":/images/XmlIcon.png"));
//    resize(680,480);

//    createAction();
//    createMenu();
//    createToolBar();
//    readSettings();

//    tableView = new QTableView(this);
//    tableView->setAcceptDrops(true);
//    model = new QStandardItemModel(0,2);								//设置表格列数
//    treeWidget = new QTreeWidget(this);

//    //treeWidget->resize(80,80);
//    treeWidget->setGeometry(10,10,150,150);
//    treeWidget->setColumnCount(5);

//    QList<QTreeWidgetItem *> items;
//    for (int i = 0; i < 10; ++i)
//    items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("item: %1").arg(i))));
//    treeWidget->insertTopLevelItems(0, items);

//}

//void MainWindow::createButton()
//{
//    btn = new QPushButton(this);
//    //btn->resize(230,180);
//    tableView->setGeometry(180,180,120,120);
//}

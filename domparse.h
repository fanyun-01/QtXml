#ifndef DOMPARSE
#define DOMPARSE
#include <QMap>
#include <QFile>
#include <QDomElement>

class QDomElement;
class QString;
class QTreeWidget;
class QTreeWidgetItem;
class QDomNode;
class DomParser
{
public:
    DomParser(QTreeWidget *tree);
    ~DomParser();
    bool readFile(const QString &fileName);//读取XML文件

    QMap<QString, QMap< QString, QString > > getNodeMap();

private:
    //分析父节点
        void parseRootElement(
        const QDomElement &element,
        QTreeWidgetItem *parent);

    //分析子节点
    void parseChildElement(
        const QDomElement &element,
        QTreeWidgetItem *parent);

    //获取相应的属性值
    void getAttruibutNameAndValue(const QDomElement &element);
    //清除Map
    void clearMap();


    static QString nodeName;							//
    QTreeWidget *XMLtree;								//XML树
    QMap<QString, QMap< QString, QString > > m_NodeMap;	//存储节点和其全部属性
    QMap<QString, QString> m_AttruibutMap;				//存储属性和值
};

#endif // DOMPARSE


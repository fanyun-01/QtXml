#include <QFile>
#include <QTreeWidgetItem>
#include <QtGui>
#include <iostream>
#include <QTreeWidget>
#include <QString>
#include "domparse.h"

DomParser::DomParser(QTreeWidget *tree)
{
    XMLtree = tree;
    clearMap();
}

bool DomParser::readFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString())
                  << endl;
        return false;
    }

    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine,
                        &errorColumn)) {
        std::cerr << "Error: Parse error at line " << errorLine << ", "
                  << "column " << errorColumn << ": "
                  << qPrintable(errorStr) << endl;
        return false;
    }

    QDomElement root = doc.documentElement();
    parseRootElement(root,XMLtree->invisibleRootItem());

    return true;
}

void DomParser::parseRootElement(const QDomElement &element,QTreeWidgetItem *parent)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, element.nodeName());

    QDomNodeList list = element.childNodes();//获取所有子节点
    QDomElement node;							//节点对象
    for(int i = 0;i<list.count();i++)
    {
        node = list.at(i).toElement();
        getAttruibutNameAndValue(node);
        parseChildElement(node,item);
    }
}

void DomParser::parseChildElement(const QDomElement &element,
                                  QTreeWidgetItem *parent)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    QDomNode child = element.firstChild();

    item->setText(0, element.nodeName());
    while (!child.isNull())
    {
        parseChildElement(child.toElement(), item);
        getAttruibutNameAndValue(child.toElement());
        child = child.nextSibling();
    }
}

void DomParser::getAttruibutNameAndValue(const QDomElement &element)
{
    QString  m_AttName,m_AttValue;
    QDomNamedNodeMap  m_qmap = element.attributes();
    for(int i = 0;i<m_qmap.count();i++)
    {
       QDomNode m_AttNode  =   m_qmap.item(i);
       m_AttName = m_AttNode.nodeName();
       m_AttValue = m_AttNode.nodeValue();
       m_AttruibutMap.insert(m_AttName,m_AttValue);
    }
    m_NodeMap.insert(element.nodeName(),m_AttruibutMap);
}

QMap<QString, QMap< QString, QString > > DomParser::getNodeMap()
{
    return m_NodeMap;
}

void DomParser::clearMap()
{
    m_NodeMap.clear();
    m_AttruibutMap.clear();
}


DomParser::~DomParser()
{
    clearMap();
}


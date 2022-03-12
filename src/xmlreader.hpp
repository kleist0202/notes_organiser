#ifndef XMLREADER_HPP
#define XMLREADER_HPP

#include <QXmlStreamReader>
#include <QDebug>
#include "treeitem.hpp"

class XmlReader {
private:
    QXmlStreamReader xml;
    TreeItem* tree_root;

public:
    XmlReader(TreeItem* root);
    bool readFile(QIODevice* device);
    void removeFromMap(QMap<int, QString>& m, int val);
};

#endif // XMLREADER_HPP

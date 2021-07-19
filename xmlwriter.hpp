#ifndef XMLWRITER_HPP
#define XMLWRITER_HPP

#include <QXmlStreamWriter>
#include <QMap>
#include "treeitem.hpp"

class XmlWriter {
private:
    QXmlStreamWriter xml;
    TreeItem* tree_root;

public:
    explicit XmlWriter(TreeItem* root);
    bool writeFile(QIODevice* device);
    void writing(TreeItem* item, const QString& generation);

};

#endif // XMLWRITER_HPP

#include "xmlwriter.hpp"

XmlWriter::XmlWriter(TreeItem* root)
    : tree_root(root)
{
    xml.setAutoFormatting(true);
}

bool XmlWriter::writeFile(QIODevice* device)
{
    xml.setDevice(device);
    xml.writeStartDocument();

    QString generation = "CHILDREN";
    xml.writeStartElement("HEADERS");
    xml.writeAttribute("title", tree_root->getDataAt(0));
    xml.writeAttribute("text", tree_root->getDataAt(1));

    for (int i = 0; i < tree_root->getChildCount(); ++i) {
        xml.writeStartElement("FATHERS");
        xml.writeAttribute("title", tree_root->getChild(i)->getDataAt(0));
        xml.writeAttribute("text", tree_root->getChild(i)->getDataAt(1));
        writing(tree_root->getChild(i), generation);
        xml.writeEndElement(); // FATHERS
    }

    xml.writeEndDocument();
    return true;
}

void XmlWriter::writing(TreeItem* item, const QString& generation)
{
    for (int i = 0; i < item->getChildCount(); ++i) {
        xml.writeStartElement(generation);
        xml.writeAttribute("title", item->getChild(i)->getDataAt(0));
        xml.writeAttribute("text", item->getChild(i)->getDataAt(1));
        writing(item->getChild(i), "GRAND" + generation);
        xml.writeEndElement();
    }
}

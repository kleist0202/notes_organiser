#include "xmlreader.hpp"

XmlReader::XmlReader(TreeItem* root)
    : tree_root(root)
{}

bool XmlReader::readFile(QIODevice* device) 
{
    xml.setDevice(device);
    xml.readNext();
    xml.readNext();

    if (xml.isStartElement()) {
        if (xml.name() == "HEADERS") {
            QXmlStreamAttributes attr = xml.attributes();
            tree_root->setData({attr.value("title").toString(), attr.value("text").toString()});
        }
    }
    QStringList geners;
    QMap<QString, QStringList> all_data;
    QMap<QString, int> hierarchy;
    hierarchy[""] = -1;
    QMap<QString, TreeItem* > temp_items;
    int index = 0;
    TreeItem* current = tree_root;
    QString previous = "";

    xml.readNext();
    while(!xml.atEnd()) {
        if (xml.isStartElement()) {
            QString val = xml.name().toString();
            if (!geners.contains(val)) {
                hierarchy[val] = index;
                ++index;
            }
            QXmlStreamAttributes attr = xml.attributes();
            geners.append(val);
            all_data[val].append(attr.value("title").toString());
            all_data[val].append(attr.value("text").toString());
        }
        xml.readNext();
    }

    QStringList a;
    for (int i = 0; i < geners.size(); ++i) {
        if (hierarchy[previous] > hierarchy[geners[i]]) {
            QMap<QString, TreeItem* >::const_iterator it = temp_items.constBegin();
            while (it != temp_items.constEnd()) {
                if (hierarchy[it.key()] > hierarchy[geners[i]]) {
                    a.append(it.key());
                }
                ++it;
            }
            for (int j = 0; j < a.count(); ++j) {
                temp_items.remove(a[j]);
            }
            a.clear();
        }

        if (temp_items.contains(geners[i])) {
            temp_items[geners[i]]->addChild(new TreeItem({all_data[geners[i]][0], all_data[geners[i]][1]}, temp_items[geners[i]]));
            all_data[geners[i]].removeAt(0);
            all_data[geners[i]].removeAt(0);
            current = temp_items[geners[i]]->getChildren().constLast();
        }
        else {
            current->addChild(new TreeItem({all_data[geners[i]][0], all_data[geners[i]][1]}, current));
            all_data[geners[i]].removeAt(0);
            all_data[geners[i]].removeAt(0);
            temp_items[geners[i]] = current;
            current = current->getChild(0);
        }
        previous = geners[i];
    }

    return true;
}

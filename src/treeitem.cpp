#include "treeitem.hpp"

TreeItem::TreeItem(const QStringList& data, TreeItem* parent)
    : m_data(data), m_parent(parent)
{}

TreeItem::~TreeItem()
{
    qDeleteAll(m_children);
}

void TreeItem::addChild(TreeItem* item)
{
    m_children.append(item);
}

QString TreeItem::getDataAt(int index) const
{
    if (index < 0 or index >= m_data.count())
        return QString();
    return m_data[index];
}

QStringList TreeItem::getData() const
{
    return m_data;
}

void TreeItem::setData(const QStringList& data) 
{
    m_data = data;
}

TreeItem* TreeItem::getChild(int index) const
{
    if (index < 0 or index >= m_children.count())
        return nullptr;
    return m_children[index];
}

QVector<TreeItem* >& TreeItem::getChildren() 
{
    return m_children;
}

int TreeItem::getChildCount() const
{
    return m_children.count();
}

TreeItem* TreeItem::getParent() const 
{
    return m_parent;
}

void TreeItem::printTree(TreeItem* item, int indent_num)
{
    std::cout << item->m_data.join(", ").toStdString();
    for (int i = 0; i < item->m_children.count(); ++i) {
        std::cout << '\n';
        for(int j = 0; j < indent_num; ++j)
            std::cout << "   ";
        printTree(item->getChild(i), indent_num+1);
    }
}

TreeItem* TreeItem::searchTree(TreeItem* item, const QString& name)
{
    if (item->getDataAt(0) == name) {
        return item;
    }

    for (int i = 0; i < item->getChildCount(); ++i) {
        TreeItem* found = TreeItem::searchTree(item->getChild(i), name);
        if (found != nullptr)
            return found;
    }
    return nullptr;
}

void TreeItem::deleteItem(TreeItem* item)
{
    TreeItem* parent = item->getParent();
    for (int i = 0; i < parent->getChildCount(); ++i) {
        if (parent->getChild(i)->getDataAt(0) == item->getDataAt(0)) {
            parent->getChildren().removeAt(i);
            delete item;
            item = nullptr;
            break;
        }
    }
}

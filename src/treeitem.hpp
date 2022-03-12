#ifndef TREEITEM_HPP
#define TREEITEM_HPP

#include <QStringList>
#include <QDebug>
#include <QVector>
#include <iostream>

class TreeItem {
private:
    QStringList m_data;
    TreeItem* m_parent;
    QVector<TreeItem* > m_children;

public:
    explicit TreeItem(const QStringList& data, TreeItem* parent = nullptr);
    ~TreeItem();

    void addChild(TreeItem* item);

    QString getDataAt(int index) const;
    QStringList getData() const;
    void setData(const QStringList& data);

    TreeItem* getChild(int index) const;
    QVector<TreeItem* >& getChildren();
    int getChildCount() const;

    TreeItem* getParent() const;

    static void printTree(TreeItem* item, int indent_num=0);
    static TreeItem* searchTree(TreeItem* item, const QString& name);
    static void deleteItem(TreeItem* item);
};

#endif // TREEITEM_HPP

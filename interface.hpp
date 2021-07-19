#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <QtWidgets>
#include <QGridLayout>
#include <QMainWindow>
#include <QMenu>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QListWidget>
#include <QFile>
#include <QTreeWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QStyle>
#include <QDesktopWidget>
#include "textedit.hpp"
#include "treeitem.hpp"

class Interface : public QMainWindow
{
    Q_OBJECT

public:
    Interface(const QString& defaultFileName, QWidget* parent = nullptr);

private:
    QString fileName;
    QString defaultFileName;
    QFile file;

    QStringList tempList;

    QLineEdit       *titleLine;
    QLineEdit       *defaultFileLine;
    QLabel          *isSetDefault;
    TextEdit        *messageText;
    QTreeWidget     *tree;
    QTreeWidgetItem *savedItem;
    TreeItem        *rootItem;
    TreeItem        *selectedItem;

    QPushButton *addButton;
    QPushButton *addSubButton;
    QPushButton *removeButton;
    QPushButton *submitButton;
    QPushButton *editButton;
    QPushButton *saveButton;
    QPushButton *setDefaultButton;

    int new_entry_index;

    QTreeWidgetItem* createChildItem(QTreeWidgetItem *item, QStringList name);
    void makeTree(QTreeWidget* root, TreeItem* root_item);
    void buildTree(QTreeWidgetItem* child, TreeItem* item);
    void readingFromFile();
    
private slots:
    void addButtonClicked();
    void addSubButtonClicked();
    void removeButtonClicked();
    void submitButtonClicked();
    void editButtonClicked();
    void setDefaultButtonClicked();
    void clicked_tree_item(QTreeWidgetItem *item, int column);

    // menu functions
    void makeNew();
    void open();
    void save();
    void saveAs();
    void quit();
    void about();

};

#endif // INTERFACE_HPP

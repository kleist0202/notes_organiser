#include "interface.hpp"
#include "xmlwriter.hpp"
#include "xmlreader.hpp"
#include "entryshortener.hpp"

Interface::Interface(const QString &defaultFileName, QWidget* parent)
    : QMainWindow(parent)
{
    // setting default variables
    this->defaultFileName = defaultFileName;
    rootItem = new TreeItem({});
    savedItem = nullptr;
    new_entry_index = 0;

    // creating empty tree widget
    tree = new QTreeWidget;
    tree->header()->setSectionResizeMode(QHeaderView::Stretch);
    tree->setHeaderLabels({"Name", "Description"});
    tree->setColumnCount(2);

    // creating labels and widgets for editing
    QLabel *titleLabel = new QLabel("Title:");
    titleLine = new QLineEdit;
    defaultFileLine = new QLineEdit;

    QLabel *messageLabel = new QLabel("Text:");

    messageText = new TextEdit(200, 300);

    titleLine->setReadOnly(true);
    messageText->setReadOnly(true);

    // creating buttons
    addButton = new QPushButton("&Add");
    addSubButton = new QPushButton("&Add Subitem");
    submitButton = new QPushButton("&Submit");
    removeButton = new QPushButton("&Remove");
    editButton = new QPushButton("&Edit");
    setDefaultButton = new QPushButton("&Set Default");

    // at the beginning all buttons are disabled 
    setButtonsDisabled(QVector{addButton, addSubButton, submitButton, editButton, removeButton});

    // sending signals for various actions
    connect(addButton, SIGNAL(clicked()), this, SLOT(addButtonClicked()));
    connect(addSubButton, SIGNAL(clicked()), this, SLOT(addSubButtonClicked()));
    connect(submitButton, SIGNAL(clicked()), this, SLOT(submitButtonClicked()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeButtonClicked()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editButtonClicked()));
    connect(setDefaultButton, SIGNAL(clicked()), this, SLOT(setDefaultButtonClicked()));
    connect(tree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(clicked_tree_item(QTreeWidgetItem*, int)));
    
    // setting menus
    QMenu *fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(tr("New"), this, &Interface::makeNew);
    fileMenu->addAction(tr("Open..."), this, &Interface::open);
    fileMenu->addAction(tr("Save..."), this, &Interface::save);
    fileMenu->addAction(tr("Save as..."), this, &Interface::saveAs);
    fileMenu->addAction(tr("Make backup..."), this, &Interface::makeBackup);
    fileMenu->addAction(tr("Quit"), this, &QWidget::close);
    QMenu *helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction(tr("&About"), this, &Interface::about);

    // setting central layout
    QWidget *centralWidget = new QWidget;

    QGridLayout *mainLayout = new QGridLayout;
    QGridLayout *interLayout = new QGridLayout;
    QVBoxLayout *buttonLayout = new QVBoxLayout;
    QHBoxLayout *defaultLineLayout = new QHBoxLayout;

    interLayout->addWidget(titleLabel, 0, 1, Qt::AlignTop);
    interLayout->addWidget(titleLine, 0, 2, Qt::AlignTop);
    interLayout->addWidget(messageLabel, 1, 1, Qt::AlignTop);
    interLayout->addWidget(messageText, 1, 2);

    mainLayout->addLayout(defaultLineLayout, 0, 0);
    defaultLineLayout->addWidget(defaultFileLine);
    defaultLineLayout->addWidget(setDefaultButton);

    mainLayout->addWidget(tree, 1, 0);

    mainLayout->addLayout(interLayout, 1, 1);
    interLayout->addLayout(buttonLayout, 1, 3);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(addSubButton);
    buttonLayout->addWidget(removeButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(submitButton);
    buttonLayout->addStretch();

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // reading .defualt file 
    QFile defaultFile(defaultFileName);

    if (!defaultFile.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Organiser"),
             QString("Cannot read default file %1:\n%2.")
             .arg(QDir::toNativeSeparators(defaultFileName),
             defaultFile.errorString()));
    }
    else {
        QTextStream in(&defaultFile);
        fileName = in.readAll();
        if (fileName.isEmpty()) {
            defaultFile.close();
        }
        else {
            readingFromFile();
            defaultFile.close();
        }
    }
    // set current file name in the text line above tree...
    defaultFileLine->setText(fileName);
    // ...and it to read-only
    defaultFileLine->setReadOnly(true);
    
    // make window larger
    const QSize availableSize = screen()->availableGeometry().size();
    resize(availableSize.width() / 1.5, availableSize.height() / 2);

    // centering window
    this->setGeometry
    (
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            this->size(),
            qApp->desktop()->availableGeometry()
        )
    ); 
}

void Interface::addButtonClicked()
{
    if (!savedItem)
        return;
    
    submitButton->setEnabled(false);

    QStringList default_value({QString("new%1").arg(new_entry_index)});
    TreeItem* itemExist = TreeItem::searchTree(rootItem, default_value[0]);

    // preventing encountering existing "new*" item
    while(itemExist) {
        ++new_entry_index;
        default_value[0] = QString("new%1").arg(new_entry_index);
        itemExist = TreeItem::searchTree(rootItem, default_value[0]);
    }

    createChildItem(savedItem->parent(), default_value);

    // looking for selected item to get his parent
    TreeItem* found = TreeItem::searchTree(rootItem, savedItem->text(0));

    if (!found) {
        qDebug() << "addButtonClicked error";
        return;
    }

    // get found's parent to add 'brother' for selected item
    found->getParent()->addChild(new TreeItem(default_value, found->getParent()));
    
}

void Interface::addSubButtonClicked() 
{
    if (!savedItem)
        return;
    
    QStringList default_value({QString("new%1").arg(new_entry_index)});
    TreeItem* itemExist = TreeItem::searchTree(rootItem, default_value[0]);

    // preventing encountering existing "new*" item
    while(itemExist) {
        ++new_entry_index;
        default_value[0] = QString("new%1").arg(new_entry_index);
        itemExist = TreeItem::searchTree(rootItem, default_value[0]);
    }

    createChildItem(savedItem, default_value);
    TreeItem* found = TreeItem::searchTree(rootItem, savedItem->text(0));

    if (!found) {
        qDebug() << "addSubButtonClicked error";
        return;
    }

    found->addChild(new TreeItem(default_value, found));

    // disable submitButton
    setButtonsDisabled(QVector{submitButton});
}

void Interface::removeButtonClicked()
{
    delete tree->currentItem();
    TreeItem::deleteItem(selectedItem);

    titleLine->clear();
    messageText->clear();

    // disable all buttons
    setButtonsDisabled(QVector{addButton, addSubButton, submitButton, editButton, removeButton});
}

void Interface::editButtonClicked()
{
    messageText->setReadOnly(false);
    titleLine->setReadOnly(false);

    submitButton->setEnabled(true);

    // disable add, addsub and remove buttons
    setButtonsDisabled(QVector{addButton, addSubButton, removeButton});
}

void Interface::submitButtonClicked()
{
    TreeItem* itemExist = TreeItem::searchTree(rootItem, titleLine->text());

    if (itemExist and selectedItem != itemExist) {
        QMessageBox::warning(this, tr("Submit"),
                             tr("Title: %1 arleady exists!")
                             .arg(itemExist->getDataAt(0)));
        return;
    }

    EntryShortener shortened(messageText->toPlainText());

    tree->currentItem()->setData(0, Qt::DisplayRole, titleLine->text());
    tree->currentItem()->setData(1, Qt::DisplayRole, shortened.getShortenedData());

    selectedItem->setData({titleLine->text(), messageText->toPlainText()});

    titleLine->setReadOnly(true);
    messageText->setReadOnly(true);

    editButton->setEnabled(true);
    removeButton->setEnabled(true);
    addButton->setEnabled(true);
    addSubButton->setEnabled(true);

    // disable submit button
    setButtonsDisabled(QVector{submitButton});
}

void Interface::setDefaultButtonClicked()
{
    if (fileName.isEmpty())
        return;

    QFile write_default_file(defaultFileName);
    if (!write_default_file.open(QFile::WriteOnly | QFile::Text)) {
        return;
    }
    QTextStream write_stream(&write_default_file);
    write_stream << fileName;

    write_default_file.close();

    statusBar()->showMessage(QString("%1 set as default").arg(fileName.split("/").last()), 3000);
}

void Interface::clicked_tree_item(QTreeWidgetItem *item, int column)
{
    int absolute_column{0};

    titleLine->setText(item->text(absolute_column));

    savedItem = item;
    selectedItem = TreeItem::searchTree(rootItem, item->text(absolute_column));
    messageText->setText(selectedItem->getDataAt(1));

    removeButton->setEnabled(true);
    editButton->setEnabled(true);
    addButton->setEnabled(true);
    addSubButton->setEnabled(true);
}

QTreeWidgetItem* Interface::createChildItem(QTreeWidgetItem *item, QStringList name)
{
    QTreeWidgetItem* childItem;
    if (item) {
        childItem = new QTreeWidgetItem(item);
    } else {
        childItem = new QTreeWidgetItem(tree);
    }
    childItem->setData(0, Qt::DisplayRole, name[0]);
    if (name.count() > 1) {
        EntryShortener shortened(name[1]);
        childItem->setData(1, Qt::DisplayRole, shortened.getShortenedData());
    }
    return childItem;
}

void Interface::makeTree(QTreeWidget* root, TreeItem* root_item)
{
    for (int i = 0; i < root_item->getChildCount(); ++i) {
        createChildItem(root->topLevelItem(i), root_item->getChild(i)->getData());
        buildTree(root->topLevelItem(i), rootItem->getChild(i));
    }
}

void Interface::buildTree(QTreeWidgetItem* child, TreeItem* item)
{
    if (!item->getChildCount())
        return;
    
    for (int i = 0; i < item->getChildCount(); ++i) {
        createChildItem(child, item->getChild(i)->getData());
        buildTree(child->child(i), item->getChild(i));
    }
}

void Interface::readingFromFile()
{
    if (fileName.isEmpty())
        return;

    file.setFileName(fileName);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Organiser"),
             QString("Cannot read file %1:\n%2.")
             .arg(QDir::toNativeSeparators(fileName),
             file.errorString()));
        fileName.clear();
        return; 
    }

    tree->clear();
    delete rootItem;
    rootItem = new TreeItem({});

    XmlReader reader(rootItem);

    if (!reader.readFile(&file)) {
        file.close();
        QMessageBox::warning(this, "Organiser",
                QString("Cannot read file %1:\n%2.")
                .arg(QDir::toNativeSeparators(fileName),
                "wrong content format"));
        return;
    }

    statusBar()->showMessage("File loaded", 3000);

    titleLine->clear();
    messageText->clear();

    // set current file name in the text line above tree
    defaultFileLine->setText(fileName);

    file.close();

    makeTree(tree, rootItem);

    // disable all buttons
    setButtonsDisabled(QVector{addButton, addSubButton, submitButton, editButton, removeButton});
}

// --------------------- menu functions definitions ----------------------

void Interface::makeNew()
{
    tree->clear();
    delete rootItem;
    rootItem = new TreeItem({});

    statusBar()->showMessage(tr("Clear"), 3000);

    titleLine->clear();
    messageText->clear();
    defaultFileLine->clear();

    fileName.clear();

    rootItem->addChild(new TreeItem({"new"}, rootItem));
    makeTree(tree, rootItem);

    // disable all buttons
    setButtonsDisabled(QVector{addButton, addSubButton, submitButton, editButton, removeButton});
}

void Interface::open()
{
    QString path_to_notes = QDir::currentPath() + "/notes";
    fileName =
            QFileDialog::getOpenFileName(this, tr("Open Input File"),
            path_to_notes,
            tr("XML Files (*.xml)"));

    readingFromFile();
}

void Interface::save()
{
    QString path_to_notes = QDir::currentPath() + "/notes";

    if (fileName.isEmpty()) {
        QString file_name = QFileDialog::getSaveFileName(this, tr("Save Organiser File"),
                   path_to_notes,
                   tr("XML Files (*.xml)"));

        if (file_name.isEmpty())
            return;

        fileName = file_name;
    }

    XmlWriter writer(rootItem);
    QFile file_to_write(fileName);
    if (!file_to_write.open(QFile::WriteOnly | QFile::Text)) {
        return;
    }

    if (writer.writeFile(&file_to_write)) {
        file_to_write.close();
        statusBar()->showMessage("File saved", 3000);
    }
    else {
        file_to_write.close();
        QMessageBox::warning(this, "Organiser", "File was not saved properly");
        return;
    }
    
    // set current file name in the text line above tree
    defaultFileLine->setText(fileName);
}

void Interface::saveAs()
{
    QString path_to_notes = QDir::currentPath() + "/notes";

    QString file_name = QFileDialog::getSaveFileName(this, tr("Save Organiser File"),
               path_to_notes,
               tr("XML Files (*.xbel *.xml)"));

    if (file_name.isEmpty())
        return;

    fileName = file_name;

    XmlWriter writer(rootItem);
    QFile file_to_write(fileName);
    if (!file_to_write.open(QFile::WriteOnly | QFile::Text)) {
        return;
    }

    if (writer.writeFile(&file_to_write)) {
        file_to_write.close();
        statusBar()->showMessage("File saved", 3000);
    }
    else {
        file_to_write.close();
        QMessageBox::warning(this, "Organiser", "File was not saves properly");
        return;
    }

    // set current file name in the text line above tree
    defaultFileLine->setText(fileName);
}

void Interface::makeBackup()
{
    QString path_to_current_file = QDir::currentPath() + "/notes/" + fileName.split("/").last();

    // cannot copy when file exists, so it must be removed
    if (QFile::exists(path_to_current_file + ".bak")) {
        QFile::remove(path_to_current_file + ".bak");
    } 
    else {
        qDebug() << "cannot remove file";
    }
    if(!QFile::copy(path_to_current_file, path_to_current_file + ".bak")) {
        QMessageBox::warning(this, "Organiser", "Backup cannot be done!");
    }
    else {
        QMessageBox::information(this, "Organiser", "Backup created successfully!");
    }
}

void Interface::quit()
{
    QWidget::close();
}

void Interface::about()
{
    QMessageBox::about(this, tr("About Organiser"),
                tr("The <b>Organiser</b> is my first qt app. "
                "Kinda boring, but I like it."));
}

// -----------------------------------------------------------------------

void Interface::setButtonsDisabled(const QVector<QPushButton* >& buttons)
{
    for (int i = 0; i < buttons.size(); ++i)
        buttons[i]->setEnabled(false);
}

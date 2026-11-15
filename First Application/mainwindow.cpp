#include "MainWindow.h"

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QComboBox>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QGroupBox>

MainWindow::MainWindow()
{
    setWindowTitle("Collection Manager");

    input = new QLineEdit();
    input->setPlaceholderText("Enter a number...");

    insertButton = new QPushButton("Insert");
    removeButton = new QPushButton("Remove");
    searchButton = new QPushButton("Search");

    sortButton = new QPushButton("Sort");
    clearButton = new QPushButton("Clear");

    clearInputButton = new QPushButton("Clear Input");
    
    indexInput = new QLineEdit();
    indexInput->setPlaceholderText("Index");
    
    atButton = new QPushButton("Get");

    undoButton = new QPushButton("Undo");
    redoButton = new QPushButton("Redo");

    saveButton = new QPushButton("Save");
    loadButton = new QPushButton("Load");
    
    list = new QListWidget();

    status = new QLabel("Ready");
    count = new QLabel("Items: 0");
    currentType = new QLabel("Current type: Integer");

    typeBox = new QComboBox();

    typeBox->addItem("Integer");
    typeBox->addItem("Double");
    typeBox->addItem("String");

    // Layouts
    QGroupBox* indexGroup = new QGroupBox("Index");

    QHBoxLayout* indexLayout = new QHBoxLayout();

    indexLayout->addWidget(indexInput);
    indexLayout->addWidget(atButton);

    indexGroup->setLayout(indexLayout);

    QGroupBox* valueGroup = new QGroupBox("Value");
    QVBoxLayout* valueLayout = new QVBoxLayout();

    valueLayout->addWidget(typeBox);
    valueLayout->addWidget(input);

    QHBoxLayout* valueButtons = new QHBoxLayout();

    valueButtons->addWidget(insertButton);
    valueButtons->addWidget(removeButton);
    valueButtons->addWidget(searchButton);
    valueButtons->addWidget(clearInputButton);

    valueLayout->addLayout(valueButtons);

    valueGroup->setLayout(valueLayout);

    QGroupBox* collectionGroup = new QGroupBox("Collection");

    QVBoxLayout* collectionLayout = new QVBoxLayout();

    collectionLayout->addWidget(list);
    collectionLayout->addWidget(count);

    collectionGroup->setLayout(collectionLayout);

    QHBoxLayout* actionLayout = new QHBoxLayout();

    actionLayout->addWidget(sortButton);
    actionLayout->addWidget(clearButton);
    actionLayout->addWidget(saveButton);
    actionLayout->addWidget(loadButton);
    actionLayout->addWidget(undoButton);
    actionLayout->addWidget(redoButton);

    QVBoxLayout* mainLayout = new QVBoxLayout();

    mainLayout->addWidget(valueGroup);
    mainLayout->addWidget(indexGroup);
    mainLayout->addWidget(collectionGroup);
    mainLayout->addLayout(actionLayout);
    mainLayout->addWidget(currentType);
    mainLayout->addWidget(status);

    setStyleSheet(
    "QPushButton {"
    "    padding: 6px 12px;"
    "}"
    "QLineEdit {"
    "    padding: 6px;"
    "}"
    "QComboBox {"
    "    padding: 6px;"
    "}"
    "QGroupBox {"
    "    font-weight: bold;"
    "}"
);

    setLayout(mainLayout);

    // Dropdown
    connect(typeBox, &QComboBox::currentTextChanged, [this]()
    {
        QString type = typeBox->currentText();

        currentType->setText(
            "Current type: " + type
        );

        if (type == "Integer")
        {
            input->setValidator(new QIntValidator(input));
        }
        else if (type == "Double")
        {
            input->setValidator(new QDoubleValidator(input));
        }
        else
        {
            input->setValidator(nullptr);
        }

        updateList();
    });

    // Clear Input
    connect(clearInputButton, &QPushButton::clicked, [this]()
    {
        input->clear();
        status->setText("Input cleared.");
    });

    // Insert
    connect(insertButton, &QPushButton::clicked, [this]()
    {
        QString text = input->text();
        QString type = typeBox->currentText();

        if (text.isEmpty())
        {
            status->setText("Please enter a value.");
            return;
        }

        if (type == "Integer")
        {
            bool ok;
            int value = text.toInt(&ok);

            if (!ok)
            {
                status->setText("Please enter a valid integer.");
                return;
            }

            if (intCollection.insert(value))
            {
                status->setText("Integer inserted.");
                input->clear();
            }
            else
                status->setText("Integer already exists.");
        }
        else if (type == "Double")
        {
            bool ok;
            double value = text.toDouble(&ok);

            if (!ok)
            {
                status->setText("Please enter a valid decimal.");
                return;
            }

            if (doubleCollection.insert(value))
            {
                status->setText("Double inserted.");
                input->clear();
            }
            else
                status->setText("Double already exists.");
        }
        else if (type == "String")
        {
            if (stringCollection.insert(text.toStdString()))
            {
                status->setText("String inserted.");
                input->clear();
            }
            else
                status->setText("String already exists.");
        }

        updateList();
    });

    // Insert with Enter
    connect(input, &QLineEdit::returnPressed, [this]()
    {
        insertButton->click();
    });

    // Remove
    connect(removeButton, &QPushButton::clicked, [this]()
    {
        QString text = input->text();
        QString type = typeBox->currentText();

        if (type == "Integer")
        {
            bool ok;
            int value = text.toInt(&ok);

            if (!ok)
            {
                status->setText("Please enter a valid integer.");
                return;
            }

            if (intCollection.remove(value))
                status->setText("Integer removed.");
            else
                status->setText("Integer not found.");
        }
        else if (type == "Double")
        {
            bool ok;
            double value = text.toDouble(&ok);

            if (!ok)
            {
                status->setText("Please enter a valid decimal.");
                return;
            }

            if (doubleCollection.remove(value))
                status->setText("Double removed.");
            else
                status->setText("Double not found.");
        }
        else if (type == "String")
        {
            if (stringCollection.remove(text.toStdString()))
                status->setText("String removed.");
            else
                status->setText("String not found.");
        }

        updateList();
    });

    // Sort
    connect(sortButton, &QPushButton::clicked, [this]()
    {
        QString type = typeBox->currentText();

        if (type == "Integer")
            intCollection.sort();
        else if (type == "Double")
            doubleCollection.sort();
        else if (type == "String")
            stringCollection.sort();

        updateList();
        status->setText("Collection sorted.");
    });

    // Search
    connect(searchButton, &QPushButton::clicked, [this]()
    {
        QString text = input->text();
        QString type = typeBox->currentText();

        int index = -1;

        if (type == "Integer")
        {
            bool ok;
            int value = text.toInt(&ok);

            if (!ok)
            {
                status->setText("Please enter a valid integer.");
                return;
            }

            index = intCollection.search(value);
        }
        else if (type == "Double")
        {
            bool ok;
            double value = text.toDouble(&ok);

            if (!ok)
            {
                status->setText("Please enter a valid decimal.");
                return;
            }

            index = doubleCollection.search(value);
        }
        else if (type == "String")
        {
            index = stringCollection.search(text.toStdString());
        }

        if (index != -1)
        {
            list->setCurrentRow(index);

            list->scrollToItem(
                list->currentItem()
            );

            status->setText(
                "Found at index " + QString::number(index)
            );
        }
        else
        {
            status->setText("Item not found.");
        }
    });

    // Clear
    connect(clearButton, &QPushButton::clicked, [this]()
    {
        QMessageBox::StandardButton answer;

        answer = QMessageBox::question(
            this,
            "Clear Collection",
            "Are you sure you want to clear the current collection?"
        );

        if (answer != QMessageBox::Yes)
            return;

        QString type = typeBox->currentText();

        if (type == "Integer")
            intCollection.makeEmpty();
        else if (type == "Double")
            doubleCollection.makeEmpty();
        else if (type == "String")
            stringCollection.makeEmpty();

        updateList();
        status->setText("Collection cleared.");
    });
    
    // At
    connect(atButton, &QPushButton::clicked, [this]()
    {
        bool ok;
        int index = indexInput->text().toInt(&ok);
        
        if (!ok)
        {
            status->setText("Please enter a valid index.");
            return;
        }
        
        QString type = typeBox->currentText();
        
        try
        {
            if (type == "Integer")
            {
                int value = intCollection.at(index);

                list->setCurrentRow(index);
                list->scrollToItem(list->currentItem());
                
                status->setText(
                    "Item at index " +
                    QString::number(index) +
                    ": " +
                    QString::number(value)
                );
            }
            else if (type == "Double")
            {
                double value = doubleCollection.at(index);

                list->setCurrentRow(index);
                list->scrollToItem(list->currentItem());
                
                status->setText(
                    "Item at index " +
                    QString::number(index) +
                    ": " +
                    QString::number(value)
                );
            }
            else if (type == "String")
            {
                string value = stringCollection.at(index);

                list->setCurrentRow(index);
                list->scrollToItem(list->currentItem());
                
                status->setText(
                    "Item at index " +
                    QString::number(index) +
                    ": " +
                    QString::fromStdString(value)
                );
            }
        }
        catch (const out_of_range& e)
        {
            status->setText(e.what());
        }
    });
    
    // Save
    connect(saveButton, &QPushButton::clicked, [this]()
    {
        QString filename = QFileDialog::getSaveFileName(
            this,
            "Save Collection",
            "",
            "Text Files (*.txt)"
        );

        if (filename.isEmpty())
            return;

        QString type = typeBox->currentText();

        if (type == "Integer")
            intCollection.save(filename.toStdString());
        else if (type == "Double")
            doubleCollection.save(filename.toStdString());
        else if (type == "String")
            stringCollection.save(filename.toStdString());

        status->setText("Collection saved.");
    });

    // Load
    connect(loadButton, &QPushButton::clicked, [this]()
    {
        QString filename = QFileDialog::getOpenFileName(
            this,
            "Load Collection",
            "",
            "Text Files (*.txt)"
        );

        if (filename.isEmpty())
            return;

        QString type = typeBox->currentText();

        if (type == "Integer")
            intCollection.load(filename.toStdString());
        else if (type == "Double")
            doubleCollection.load(filename.toStdString());
        else if (type == "String")
            stringCollection.load(filename.toStdString());

        updateList();
        status->setText("Collection loaded.");
    });
}

void MainWindow::updateList()
{
    list->clear();

    QString type = typeBox->currentText();

    if (type == "Integer")
    {
        for (int i = 0; i < intCollection.size(); i++)
        {
            list->addItem(
                QString::number(i) + ": " +
                QString::number(intCollection.at(i))
            );
        }

        count->setText(
            "Items: " + QString::number(intCollection.size())
        );
    }
    else if (type == "Double")
    {
        for (int i = 0; i < doubleCollection.size(); i++)
        {
            list->addItem(
                QString::number(i) + ": " +
                QString::number(doubleCollection.at(i))
            );
        }

        count->setText(
            "Items: " + QString::number(doubleCollection.size())
        );
    }
    else if (type == "String")
    {
        for (int i = 0; i < stringCollection.size(); i++)
        {
            list->addItem(
                QString::number(i) + ": " +
                QString::fromStdString(stringCollection.at(i))
            );
        }

        count->setText(
            "Items: " + QString::number(stringCollection.size())
        );
    }
}
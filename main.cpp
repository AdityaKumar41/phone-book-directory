#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>

class ContactBookWidget : public QWidget {
    Q_OBJECT

private:
    QListWidget* contactList;
    QLineEdit* nameLineEdit;
    QLineEdit* numberLineEdit;
    QPushButton* addButton;
    QPushButton* deleteButton;
    QPushButton* clearButton;
    QPushButton* editButton;
    QPushButton* saveButton;
    QPushButton* searchButton;
    QLabel* editLabel;
    int editIndex;

public:
    ContactBookWidget(QWidget* parent = nullptr) : QWidget(parent), editIndex(-1) {
        QVBoxLayout* layout = new QVBoxLayout(this);

        QLabel* nameLabel = new QLabel("Name:");
        QLabel* numberLabel = new QLabel("Phone Number:");

        nameLineEdit = new QLineEdit(this);
        numberLineEdit = new QLineEdit(this);

        setWindowTitle("Contact Book Application");

        addButton = new QPushButton("Add Contact", this);
        deleteButton = new QPushButton("Delete Contact", this);
        clearButton = new QPushButton("Clear Contacts", this);
        editButton = new QPushButton("Edit Contact", this);
        editLabel = new QLabel("", this);
        layout->addWidget(editButton);
        layout->addWidget(editLabel);
        saveButton = new QPushButton("Save Edit", this);
        saveButton->hide();  // Initially hide the Save button

        searchButton = new QPushButton("Search Contact", this);
        contactList = new QListWidget(this);

        layout->addWidget(nameLabel);
        layout->addWidget(nameLineEdit);
        layout->addWidget(numberLabel);
        layout->addWidget(numberLineEdit);
        layout->addWidget(addButton);
        layout->addWidget(deleteButton);
        layout->addWidget(clearButton);
        layout->addWidget(editButton);
        layout->addWidget(saveButton);
        layout->addWidget(searchButton);
        layout->addWidget(editLabel);
        layout->addWidget(contactList);

        // New connection for the edit button
        connect(editButton, SIGNAL(clicked()), this, SLOT(editContact()));
        connect(addButton, SIGNAL(clicked()), this, SLOT(addContact()));
        connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteContact()));
        connect(clearButton, SIGNAL(clicked()), this, SLOT(clearContacts()));
        connect(saveButton, SIGNAL(clicked()), this, SLOT(saveEditedContact()));
        connect(searchButton, SIGNAL(clicked()), this, SLOT(searchContact()));
    }

public slots:
    void addContact() {
        QString newName = nameLineEdit->text();
        QString newNumber = numberLineEdit->text();

        // Check for duplicates
        bool duplicate = false;
        for (int i = 0; i < contactList->count(); ++i) {
            QListWidgetItem* item = contactList->item(i);
            QString contact = item->text();
            QStringList parts = contact.split(" - ");
            if (parts.size() == 2) {
                QString existingName = parts[0];
                QString existingNumber = parts[1];
                if (existingName == newName || existingNumber == newNumber) {
                    duplicate = true;
                    break;
                }
            }
        }

        if (!duplicate) {
            // No duplicate found, add the contact
            QString contact = newName + " - " + newNumber;
            QListWidgetItem* newItem = new QListWidgetItem(contact);
            contactList->addItem(newItem);

            // Clear input fields
            nameLineEdit->clear();
            numberLineEdit->clear();
        } else {
            // Display a warning about the duplicate
            QMessageBox::warning(this, "Duplicate Contact", "A contact with the same name or mobile number already exists.");
        }

    }

    void deleteContact() {
        QListWidgetItem* selectedItem = contactList->currentItem();
        if (selectedItem) {
            delete selectedItem;
        } else {
            QMessageBox::warning(this, "Contact Not Found", "Please select a contact to delete.");
        }
    }

    void clearContacts() {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Clear Contacts", "Are you sure you want to clear all contacts?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            contactList->clear();
        }
    }
    void editContact() {
        QListWidgetItem* selectedItem = contactList->currentItem();
        if (selectedItem) {
            QString contact = selectedItem->text();
            QStringList parts = contact.split(" - ");
            if (parts.size() == 2) {
                QString name = parts[0];
                QString number = parts[1];

                // If already in edit mode, save changes
                if (editIndex != -1) {
                    saveEditedContact();
                }

                // Set the edit mode and update the fields
                editIndex = contactList->currentRow();
                editLabel->setText("Editing contact: " + contact);
                nameLineEdit->setText(name);
                numberLineEdit->setText(number);
                saveButton->show();  // Show the Save button during editing
            }
        } else {
            QMessageBox::warning(this, "Contact Not Found", "Please select a contact to edit.");
        }
    }
    void saveEditedContact() {
        // Save the edited contact back to the list
        if (editIndex != -1) {
            QString editedName = nameLineEdit->text();
            QString editedNumber = numberLineEdit->text();
            QString editedContact = editedName + " - " + editedNumber;

            contactList->item(editIndex)->setText(editedContact);
            editIndex = -1;  // Reset editIndex after saving changes
            editLabel->clear();
            nameLineEdit->clear();
            numberLineEdit->clear();
            saveButton->hide();  // Hide the Save button after saving
        }
    }
    void searchContact() {
        QString searchTerm = nameLineEdit->text();
        if (searchTerm.isEmpty()) {
            searchTerm = numberLineEdit->text();
        }

        if (!searchTerm.isEmpty()) {
            bool found = false;
            for (int i = 0; i < contactList->count(); ++i) {
                QListWidgetItem* item = contactList->item(i);
                if (item->text().contains(searchTerm, Qt::CaseInsensitive)) {
                    contactList->setCurrentItem(item);
                    found = true;
                    break;
                }
            }

            if (!found) {
                QMessageBox::information(this, "Contact Not Found", "No contact found matching the search term.");
            }
        } else {
            QMessageBox::information(this, "Empty Search Term", "Please enter a name or phone number to search.");
        }
    }
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    ContactBookWidget contactBookWidget;
    contactBookWidget.show();

    return app.exec();
}


#include "main.moc"

/*
In a library, for each book book-id, serial number (denotes copy number of a
book), title, author, publisher and price are stored. Book-id and serial number
together will be unique identifier for a book. Members are either student or
faculty. Each member has unique member-id. Name, e-mail, address are also to
be stored. For any transaction (book issue or return), members are supposed to
place transactions slip. User will submit member-id, book-id, and serial number
(only for book return). While processing a transaction, check the validity of the
member. While issuing, availability of a copy of the book is to be checked. While
returning a book, it is to be checked whether this copy was issued to the member
or not. A student member can have 2 books issued at a point of time. For faculty
members it is 10. Transaction information is to be stored like date of transaction,
member-id, book-id, serial number, returned or not. An entry is made when book
is issued and updated when the book is returned.
Design the classes and implement. For list consider memory data structure.
*/
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <ctime>

class Member {
protected:
    std::string memberId;
    std::string name;
    std::string email;
    std::string address;
    int booksIssued;

public:
    Member(const std::string& memberId, const std::string& name, const std::string& email, const std::string& address)
        : memberId(memberId), name(name), email(email), address(address), booksIssued(0) {}

    virtual int getMaxBooks() const = 0;
    std::string getMemberId() const { return memberId; }

    bool canIssueMoreBooks() const { return booksIssued < getMaxBooks(); }

    void issueBook() { ++booksIssued; }
    void returnBook() { --booksIssued; }

    virtual ~Member() {}
};

class Student : public Member {
public:
    Student(const std::string& memberId, const std::string& name, const std::string& email, const std::string& address)
        : Member(memberId, name, email, address) {}

    int getMaxBooks() const override {
        return 2;
    }
};

class Faculty : public Member {
public:
    Faculty(const std::string& memberId, const std::string& name, const std::string& email, const std::string& address)
        : Member(memberId, name, email, address) {}

    int getMaxBooks() const override {
        return 10;
    }
};

class Book {
private:
    std::string bookId;
    int serialNumber;
    std::string title;
    std::string author;
    std::string publisher;
    double price;
    bool isIssued;

public:
    Book(const std::string& bookId, int serialNumber, const std::string& title, const std::string& author, const std::string& publisher, double price)
        : bookId(bookId), serialNumber(serialNumber), title(title), author(author), publisher(publisher), price(price), isIssued(false) {}

    std::string getBookId() const { return bookId; }
    int getSerialNumber() const { return serialNumber; }

    bool checkAvailability() const { return !isIssued; }
    void markIssued() { isIssued = true; }
    void markReturned() { isIssued = false; }
};

class Transaction {
private:
    std::string memberId;
    std::string bookId;
    int serialNumber;
    std::time_t date;
    bool isReturned;

public:
    Transaction(const std::string& memberId, const std::string& bookId, int serialNumber)
        : memberId(memberId), bookId(bookId), serialNumber(serialNumber), date(std::time(0)), isReturned(false) {}

    void markReturned() { isReturned = true; }
    bool checkTransaction(const std::string& memId, const std::string& bId, int serNum) const {
        return memberId == memId && bookId == bId && serialNumber == serNum && !isReturned;
    }
};

class Library {
private:
    std::map<std::string, std::map<int, Book*>> books; 
    std::map<std::string, Member*> members;
    std::vector<Transaction*> transactions;

public:
    ~Library() {
        for (auto& bookMap : books) {
            for (auto& book : bookMap.second) {
                delete book.second;
            }
        }
        for (auto& member : members) {
            delete member.second;
        }
        for (auto& transaction : transactions) {
            delete transaction;
        }
    }

    void addBook(const std::string& bookId, int serialNumber, const std::string& title, const std::string& author, const std::string& publisher, double price) {
        if (books[bookId].find(serialNumber) != books[bookId].end()) {
            std::cout << "Error: Book with this ID and serial number already exists.\n";
            return;
        }
        books[bookId][serialNumber] = new Book(bookId, serialNumber, title, author, publisher, price);
    }

    void addMember(const std::string& memberId, const std::string& name, const std::string& email, const std::string& address, bool isFaculty) {
        if (members.find(memberId) != members.end()) {
            std::cout << "Error: Member with this ID already exists.\n";
            return;
        }
        Member* member = nullptr;
        if (isFaculty) {
            member = new Faculty(memberId, name, email, address);
        } else {
            member = new Student(memberId, name, email, address);
        }
        members[memberId] = member;
    }

    void issueBook(const std::string& memberId, const std::string& bookId) {
        if (members.find(memberId) == members.end()) {
            std::cout << "Error: Invalid member ID.\n";
            return;
        }
        if (books.find(bookId) == books.end()) {
            std::cout << "Error: Book ID not found.\n";
            return;
        }
        Member* member = members[memberId];
        if (!member->canIssueMoreBooks()) {
            std::cout << "Error: Member has reached the maximum limit of issued books.\n";
            return;
        }
        for (auto& bookPair : books[bookId]) {
            Book* book = bookPair.second;
            if (book->checkAvailability()) {
                book->markIssued();
                member->issueBook();
                transactions.push_back(new Transaction(memberId, bookId, book->getSerialNumber()));
                std::cout << "Book issued successfully.\n";
                return;
            }
        }
        std::cout << "Error: No available copy of the book.\n";
    }

    void returnBook(const std::string& memberId, const std::string& bookId, int serialNumber) {
        if (members.find(memberId) == members.end()) {
            std::cout << "Error: Invalid member ID.\n";
            return;
        }
        if (books.find(bookId) == books.end() || books[bookId].find(serialNumber) == books[bookId].end()) {
            std::cout << "Error: Book ID or serial number not found.\n";
            return;
        }
        for (auto& transaction : transactions) {
            if (transaction->checkTransaction(memberId, bookId, serialNumber)) {
                transaction->markReturned();
                books[bookId][serialNumber]->markReturned();
                members[memberId]->returnBook();
                std::cout << "Book returned successfully.\n";
                return;
            }
        }
        std::cout << "Error: This book was not issued to this member.\n";
    }
};

class System {
public:
    static void run() {
        Library library;

        while (true) {
            std::cout << "\nMenu:\n";
            std::cout << "1. Add Book\n";
            std::cout << "2. Add Member\n";
            std::cout << "3. Issue Book\n";
            std::cout << "4. Return Book\n";
            std::cout << "5. Exit\n";
            std::cout << "Enter your choice: ";

            int choice;
            std::cin >> choice;

            switch (choice) {
                case 1: {
                    std::string bookId, title, author, publisher;
                    int serialNumber;
                    double price;
                    std::cout << "Enter Book ID: ";
                    std::cin >> bookId;
                    std::cout << "Enter Serial Number: ";
                    std::cin >> serialNumber;
                    std::cout << "Enter Title: ";
                    std::cin.ignore();
                    std::getline(std::cin, title);
                    std::cout << "Enter Author: ";
                    std::getline(std::cin, author);
                    std::cout << "Enter Publisher: ";
                    std::getline(std::cin, publisher);
                    std::cout << "Enter Price: ";
                    std::cin >> price;

                    library.addBook(bookId, serialNumber, title, author, publisher, price);
                    break;
                }
                case 2: {
                    std::string memberId, name, email, address;
                    bool isFaculty;
                    std::cout << "Enter Member ID: ";
                    std::cin >> memberId;
                    std::cout << "Enter Name: ";
                    std::cin.ignore();
                    std::getline(std::cin, name);
                    std::cout << "Enter Email: ";
                    std::getline(std::cin, email);
                    std::cout << "Enter Address: ";
                    std::getline(std::cin, address);
                    std::cout << "Is Faculty (1/0): ";
                    std::cin >> isFaculty;

                    library.addMember(memberId, name, email, address, isFaculty);
                    break;
                }
                case 3: {
                    std::string memberId, bookId;
                    std::cout << "Enter Member ID: ";
                    std::cin >> memberId;
                    std::cout << "Enter Book ID: ";
                    std::cin >> bookId;

                    library.issueBook(memberId, bookId);
                    break;
                }
                case 4: {
                    std::string memberId, bookId;
                    int serialNumber;

                    std::cout << "Enter Member ID: ";
                    std::cin >> memberId;
                    std::cout << "Enter Book ID: ";
                    std::cin >> bookId;
                    std::cout << "Enter Serial Number: ";
                    std::cin >> serialNumber;

                    library.returnBook(memberId, bookId, serialNumber);
                    break;
                }
                case 5: {
                    std::cout << "Exiting system.\n";
                    return;
                }
                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }
};

int main() {
    System::run();
    return 0;
}

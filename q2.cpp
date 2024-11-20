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

class Member {
private:
    std::string memberId;
    std::string name;
    std::string email;
    std::string address;
    int maxBooks;
    int booksIssued;

public:
    Member(const std::string& memberId, const std::string& name, const std::string& email, const std::string& address, int maxBooks)
        : memberId(memberId), name(name), email(email), address(address), maxBooks(maxBooks), booksIssued(0) {}

    std::string getMemberId() const { return memberId; }
    bool canIssueMoreBooks() const { return booksIssued < maxBooks; }
    void issueBook() { ++booksIssued; }
    void returnBook() { --booksIssued; }
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
        for (std::map<std::string, std::map<int, Book*>>::iterator it = books.begin(); it != books.end(); ++it) {
            for (std::map<int, Book*>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt) {
                delete jt->second;
            }
        }
        for (std::map<std::string, Member*>::iterator it = members.begin(); it != members.end(); ++it) {
            delete it->second;
        }
        for (std::vector<Transaction*>::iterator it = transactions.begin(); it != transactions.end(); ++it) {
            delete *it;
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
        int maxBooks = isFaculty ? 10 : 2;
        members[memberId] = new Member(memberId, name, email, address, maxBooks);
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
        for (std::map<int, Book*>::iterator it = books[bookId].begin(); it != books[bookId].end(); ++it) {
            if (it->second->checkAvailability()) {
                it->second->markIssued();
                member->issueBook();
                transactions.push_back(new Transaction(memberId, bookId, it->first));
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
        for (std::vector<Transaction*>::iterator it = transactions.begin(); it != transactions.end(); ++it) {
            if ((*it)->checkTransaction(memberId, bookId, serialNumber)) {
                (*it)->markReturned();
                books[bookId][serialNumber]->markReturned();
                members[memberId]->returnBook();
                std::cout << "Book returned successfully.\n";
                return;
            }
        }
        std::cout << "Error: This book was not issued to this member.\n";
    }
};

int main() {
    Library library;

    library.addBook("B101", 1, "The Great Gatsby", "F. Scott Fitzgerald", "Scribner", 10.99);
    library.addBook("B101", 2, "The Great Gatsby", "F. Scott Fitzgerald", "Scribner", 10.99);
    library.addMember("M001", "Alice", "alice@example.com", "123 Street", false);
    library.addMember("M002", "Dr. Bob", "bob@example.com", "456 Avenue", true);

    library.issueBook("M001", "B101");
    library.returnBook("M001", "B101", 1);

    return 0;
}

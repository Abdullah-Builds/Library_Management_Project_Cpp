#include <bits/stdc++.h>
#include <fstream>
#include <windows.h>
#include <stdio.h>
const char *RED_TEXT = "\033[31m";
const char *RESET = "\033[0m";
const char *BLACK = "\033[30m";
const char *RED = "\033[31m";
const char *GREEN = "\033[32m";
const char *YELLOW = "\033[33m";
const char *BLUE = "\033[34m";
const char *MAGENTA = "\033[35m";
const char *CYAN = "\033[36m";
const char *WHITE = "\033[37m";
const char *BOLD_TEXT = "\033[1m";

using namespace std;

void clearScreen()
{
    system("cls");
}

bool isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

string incrementDateBy5Days(const string &currentDate)
{
    stringstream ss(currentDate);
    int day, month, year;
    char delimiter;

    ss >> day >> delimiter >> month >> delimiter >> year;

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (isLeapYear(year))
    {
        daysInMonth[1] = 29;
    }

    day += 5;

    while (day > daysInMonth[month - 1])
    {
        day -= daysInMonth[month - 1];
        month++;

        if (month > 12)
        {
            month = 1;
            year++;
            if (isLeapYear(year))
            {
                daysInMonth[1] = 29;
            }
            else
            {
                daysInMonth[1] = 28;
            }
        }
    }

    stringstream result;
    result << setfill('0') << setw(2) << day << "/" << setw(2) << month << "/" << setw(2) << (year % 100);

    return result.str();
}

string date = "01/05/24";

class Book
{
    string title, author, ISBN;

public:
    Book(string title, string author, string ISBN) : title(title), author(author), ISBN(ISBN) {}

    string getTitle()
    {
        return this->title;
    }
    string getAuthor()
    {
        return this->author;
    }
    string getISBN()
    {
        return this->ISBN;
    }
};

vector<Book> books;

bool cmp(Book &a, Book &b)
{
    int x = stoi(a.getISBN());
    int y = stoi(b.getISBN());

    return (x < y);
}
void sortTheBooks()
{
    sort(books.begin(), books.end(), cmp);
}

class Person
{
protected:
    void displayBooksAvailable()
    {
        sortTheBooks();
        for (auto it : books)
        {
            cout << YELLOW << "Title: " << it.getTitle() << RESET << endl;
            cout << YELLOW << "Author: " << it.getAuthor() << RESET << endl;
            cout << YELLOW << "ISBN: " << it.getISBN() << RESET << endl
                 << endl;
        }
    }
};

class Admin : public Person
{
    string name, password;

public:
    Admin(string name, string password) : name(name), password(password) {}

    string getName()
    {
        return this->name;
    }
    string getPassword()
    {
        return this->password;
    }

    void addBook()
    {
        string title, author, isbn;
        cout << "Enter Title: ";
        getline(cin, title);
        cout << "\nEnter Author: ";
        getline(cin, author);
        cout << "\nEnter ISBN: ";
        getline(cin, isbn);
        bool flag = 0;
        for (auto it : books)
        {
            if (it.getISBN() == isbn)
            {
                flag = 1;
            }
        }
        if (flag)
        {
            cout << RED_TEXT << "\nERROR!!! Book with similiar ISBN already exists...\n"
                 << RESET;
        }
        else
        {
            books.push_back(Book(title, author, isbn));
        }
        sortTheBooks();
    }

    void removeBook()
    {
        cout << "\nEnter ISBN: ";
        string isbn;
        getline(cin, isbn);
        int index = -1;
        for (int i = 0; i < books.size(); i++)
        {
            if (books[i].getISBN() == isbn)
            {
                index = i;
                break;
            }
        }

        if (index == -1)
        {
            cout << "\nNo such book found...\n";
        }
        else
        {
            cout << RED_TEXT << "\nBook Removed successfully!!!\n"
                 << RESET;
            books.erase(books.begin() + index);
            // sortTheBooks(books);
        }
        sortTheBooks();
    }

    void displayBooks()
    {
        Person::displayBooksAvailable();
    }
};

class User : public Person
{
    string id, name, password;
    double fine; // 500 for loosing a book, 2.5 for returning it late per day
    int noOfBooks;
    vector<pair<Book, string>> have; // first stores book, second stores issue date
public:
    User(string id, string name, string password, double fine, int noOfBooks, vector<pair<Book, string>> have) : id(id), name(name), password(password), fine(fine), noOfBooks(noOfBooks), have(have) {}

    string getID()
    {
        return this->id;
    }
    string getName()
    {
        return this->name;
    }
    string getPassword()
    {
        return this->password;
    }
    double getFine()
    {
        return this->fine;
    }
    void setFine(double fine)
    {
        this->fine = fine;
    }

    void payFine()
    {
        cout << RED_TEXT << "Your current fine is: $" << this->fine << RESET << endl;
        cout << "Enter the amount of fine you wish to pay: $";
        int x;
        cin >> x;
        this->fine -= x;
        cout << "\nRemaing balance: $" << this->fine << endl;
    }

    int getNoOfBooks()
    {
        return this->noOfBooks;
    }
    void setNoOfBooks(int noOfBooks)
    {
        this->noOfBooks = noOfBooks;
    }

    vector<pair<Book, string>> getBooks()
    {
        return this->have;
    }
    void setBooks(vector<pair<Book, string>> have)
    {
        this->have = have;
    }

    double calculateFine(const string &dueDate, const string &returnDate)
    {
        const double fineRate = 2.5;
        const int gracePeriod = 10;

        int dueDay, dueMonth, dueYear;
        sscanf(dueDate.c_str(), "%d/%d/%d", &dueDay, &dueMonth, &dueYear);

        int returnDay, returnMonth, returnYear;
        sscanf(returnDate.c_str(), "%d/%d/%d", &returnDay, &returnMonth, &returnYear);

        int daysPastDue = (returnYear - dueYear) * 365 + (returnMonth - dueMonth) * 30 + (returnDay - dueDay);

        if (daysPastDue > gracePeriod)
        {
            int extraDays = daysPastDue - gracePeriod;
            return extraDays * fineRate;
        }
        else
        {
            return 0.0;
        }
    }

    void returnBook()
    {
        string returnDate = date;
        cout << "\nEnter ISBN of the book you wish to return: ";
        string isbn;
        getline(cin, isbn);

        bool flag = 0;
        for (auto it : have)
        {
            if (it.first.getISBN() == isbn)
            {
                this->fine += calculateFine(it.second, returnDate);
                flag = 1;
                books.push_back(it.first);
                break;
            }
        }
        if (flag)
        {
            cout << "\nBook returned...\n";
        }
        else
        {
            cout << RED_TEXT << "\nNo such book found.\n"
                 << RESET;
        }
        sortTheBooks();
    }

    void displayMyBooks()
    {
        for (auto it : this->have)
        {
            cout << YELLOW << "Issue Date: " << it.second << RESET << endl;
            cout << YELLOW << "Title: " << it.first.getTitle() << RESET << endl;
            cout << YELLOW << "Author: " << it.first.getAuthor() << RESET << endl;
            cout << YELLOW << "ISBN: " << it.first.getISBN() << RESET << endl
                 << endl;
        }
    }

    void displayBooksAvailable()
    {
        Person::displayBooksAvailable();
    }

    bool searchBook(string isbn)
    {
        for (auto it : books)
        {
            if (it.getISBN() == isbn)
            {
                return true;
            }
        }
        return false;
    }

    void issueBook(string isbn)
    {
        int index = -1;
        for (int i = 0; i < books.size(); i++)
        {
            if (books[i].getISBN() == isbn)
            {
                index = i;
                break;
            }
        }
        if (index == -1)
        {
            cout << RED_TEXT << "No such book found...\n"
                 << RESET;
        }
        else
        {
            this->have.push_back({books[index], date});
            books.erase(books.begin() + index);
            cout << "Book has been issued successfully...\n";
        }
        sortTheBooks();
    }

    void reportLostBook()
    {
        cout << "Enter the ISBN of the book you lost: ";
        string isbn;
        getline(cin, isbn);
        int index = -1;
        for (int i = 0; i < this->have.size(); i++)
        {
            if (this->have[i].first.getISBN() == isbn)
            {
                index = i;
                this->fine += 500;
                break;
            }
        }
        if (index != -1)
        {
            cout << "Your fine has been updated to : $" << this->fine << endl;
            cout << RED_TEXT << "\nPlease try not to loose any other book\n"
                 << RESET;
            this->have.erase(this->have.begin() + index);
        }
        else
        {
            cout << RED_TEXT << "\nNo such book found...\n"
                 << RESET;
        }
        sortTheBooks();
    }
};

vector<User> users;
vector<Admin> admins;

void readAdmins()
{
    ifstream file("admins.txt");

    if (!file.is_open())
    {
        cerr << "Failed to open file 'admins.txt'" << endl;
    }

    string line;
    string name, password;
    bool readName = true; // Flag to alternate between reading name and password

    while (getline(file, line))
    {
        if (readName)
        {
            name = line;
        }
        else
        {
            password = line;
            admins.emplace_back(name, password);
        }
        readName = !readName;
    }

    file.close();
}

void readUsers()
{
    ifstream file("users.txt");

    if (!file.is_open())
    {
        cerr << "Failed to open file 'users.txt'" << endl;
        return;
    }

    string line;
    string id, name, password;
    double fine;
    int noOfBooks;
    vector<pair<Book, string>> have; // to store user's books with issue dates

    while (getline(file, line))
    {
        // Read user's ID, name, password, fine, and number of books
        id = line;
        getline(file, name);
        getline(file, password);
        file >> fine;
        file.ignore(); // ignore newline after reading fine
        file >> noOfBooks;
        file.ignore(); // ignore newline after reading noOfBooks

        // Read user's books with issue dates
        have.clear(); // clear the vector before populating it
        for (int i = 0; i < noOfBooks; i++)
        {
            string title, author, isbn, issueDate;
            getline(file, title);
            getline(file, author);
            getline(file, isbn);
            getline(file, issueDate);
            Book book(title, author, isbn);
            have.push_back({book, issueDate});
        }

        // Create User object and add to users vector
        users.push_back(User(id, name, password, fine, noOfBooks, have));
    }

    file.close();
}

void readBooks()
{
    ifstream file("books.txt"); // Open books.txt file for reading

    if (!file.is_open())
    {
        cerr << "Failed to open file 'books.txt'" << endl;
        return;
    }

    string title, author, isbn;
    sortTheBooks();

    // Read book data from file line by line
    while (getline(file, title))
    {
        getline(file, author);
        getline(file, isbn);

        // Create a Book object with the read data and add to the books vector
        books.push_back(Book(title, author, isbn));
    }

    file.close(); // Close the file after reading all data
}

void writeUsersToFile()
{
    ofstream file("users.txt");

    if (!file.is_open())
    {
        cerr << "Failed to open file 'users.txt' for writing" << endl;
        return;
    }

    for (auto &user : users)
    {
        file << user.getID() << endl;
        file << user.getName() << endl;
        file << user.getPassword() << endl;
        file << user.getFine() << endl;
        file << user.getNoOfBooks() << endl;

        for (auto &bookPair : user.getBooks())
        {
            Book &book = bookPair.first;
            file << book.getTitle() << endl;
            file << book.getAuthor() << endl;
            file << book.getISBN() << endl;
            file << bookPair.second << endl; // issue date
        }
    }

    file.close();
}

void writeBooksToFile()
{
    ofstream file("books.txt");

    if (!file.is_open())
    {
        cerr << "Failed to open file 'books.txt' for writing" << endl;
        return;
    }

    for (auto &book : books)
    {
        file << book.getTitle() << endl;
        file << book.getAuthor() << endl;
        file << book.getISBN() << endl;
    }

    file.close();
}

void loading()
{
    int bar1 = 177, bar2 = 219;
    cout << WHITE << BOLD_TEXT << "\n\n\n\n\n\n\n\t\t\t\tLOADING... " << RESET;
    cout << "\n\n\n\n\n\t\t\t\t\t\t";

    for (int i = 0; i < 25; i++)
        cout << YELLOW << (char)bar1 << RESET;
    cout << "\r";
    cout << "\t\t\t\t\t\t";

    for (int i = 0; i < 25; i++)
    {
        cout << YELLOW << (char)bar2 << RESET;
        Sleep(150);
    }
}

int main()
{
    loading();
    clearScreen();
    readAdmins();
    readUsers();
    readBooks();

    // Printing welcome message
    cout << "\n";
    cout << "\t\t\t------------------------------------------------------------------------------------------------------------------------\n";
    cout << YELLOW << "\t\t\t\t*****\t WELCOME TO THE OFFICIAL FAST NUCES LIBRARY MANAGEMENT SYSTEM\t *****\n"
         << RESET;
    cout << "\t\t\t------------------------------------------------------------------------------------------------------------------------\n";
    cout << "\n";
    cout << "\n";

    // Displaying options

    while (true)
    {
        date = incrementDateBy5Days(date);
        int choice = -1;
        cout << WHITE << "Select an option:\n"
             << RESET;
        cout << GREEN << "1. Sign in as an admin\n"
             << RESET;
        cout << GREEN << "2. Sign in as a user\n"
             << RESET;
        cout << GREEN << "3. Register as a new user\n"
             << RESET;
        cout << RED << "4. Exit\n";
        cout << MAGENTA << "Enter your choice (1-4): " << RESET;
        cin >> choice;
        cin.ignore();
        string username, password, id;
        int index;
        bool flag;
        switch (choice)
        {
        case 1:
            clearScreen();
            // Code for admin sign-in
            cout << "Signing in as admin...\n";
            cout << MAGENTA << "Enter username: " << RESET;
            getline(cin, username);
            cout << MAGENTA << "\nEnter password: " << RESET;
            getline(cin, password);
            cout << endl;

            index = -1;
            for (int i = 0; i < admins.size(); i++)
            {
                if (admins[i].getName() == username && admins[i].getPassword() == password)
                {
                    index = i;
                    break;
                }
            }

            if (index != -1)
            {
                clearScreen();
                cout << GREEN << "\t WELCOME " << username << " TO THE LIBRARY!!!\n"
                     << RESET;
                flag = 1;
                while (flag)
                {
                    cout << MAGENTA << "Admin Menu\n"
                         << RESET;
                    cout << BLUE << "1. Add Book\n"
                         << RESET;
                    cout << BLUE << "2. Remove Book\n"
                         << RESET;
                    cout << BLUE << "3. Display Books\n"
                         << RESET;
                    cout << BLUE << "4. Logout\n"
                         << RESET;
                    cout << WHITE << "Enter your choice (1-4): " << RESET;

                    cin >> choice;
                    cin.ignore();

                    switch (choice)
                    {
                    case 1:
                        admins[index].addBook();
                        writeBooksToFile();
                        break;
                    case 2:
                        admins[index].removeBook();
                        writeBooksToFile();
                        break;
                    case 3:
                        admins[index].displayBooks();
                        break;
                    case 4:
                        cout << RED << "Logging out...\n"
                             << RESET;
                        flag = 0;
                        break;
                    default:
                        cout << RED << "Invalid choice. Please enter a number between 1 and 4.\n"
                             << RESET;
                    }
                    date = incrementDateBy5Days(date);
                }
                clearScreen();
            }
            else
            {
                cout << RED << "INVALID USERNAME OR PASSWORD!!!\n"
                     << RESET;
            }

            break;
        case 2:
            // Code for user sign-in
            date = incrementDateBy5Days(date);
            clearScreen();
            cout << "Signing in as user...\n"
                 << RESET;
            cout << BLUE << "Enter user id: " << RESET;
            getline(cin, id);
            cout << BLUE << "\nEnter username: " << RESET;
            getline(cin, username);
            cout << BLUE << "\nEnter password: " << RESET;
            getline(cin, password);

            index = -1;
            for (int i = 0; i < users.size(); i++)
            {
                if (users[i].getID() == id && users[i].getName() == username && users[i].getPassword() == password)
                {
                    index = i;
                    break;
                }
            }

            if (index == -1)
            {
                cout << RED << "INVALID CREDENTIALS...\nPLEASE TRY AGAIN...\n"
                     << RESET;
            }
            else
            {

                cout << YELLOW << "\t WELCOME " << username << " TO THE LIBRARY!!!\n"
                     << RESET;
                flag = 1;
                while (flag)
                {
                    cout << "\nUser Menu:\n"
                         << RESET;
                    cout << GREEN << "1. Return Book\n"
                         << RESET;
                    cout << GREEN << "2. Display My Books\n"
                         << RESET;
                    cout << GREEN << "3. Display Available Books\n"
                         << RESET;
                    cout << GREEN << "4. Search Book by ISBN\n"
                         << RESET;
                    cout << GREEN << "5. Issue Book\n"
                         << RESET;
                    cout << GREEN << "6. Pay fine\n"
                         << RESET;
                    cout << GREEN << "7. Report lost book\n"
                         << RESET;
                    cout << RED << "8. Exit\n"
                         << RESET;
                    cout << MAGENTA << "Enter your choice (1-6): " << RESET;

                    int choice;
                    cin >> choice;
                    cin.ignore();

                    switch (choice)
                    {
                    case 1:
                        users[index].returnBook();
                        writeBooksToFile();
                        writeUsersToFile();
                        break;
                    case 2:
                        users[index].displayMyBooks();
                        break;
                    case 3:
                        users[index].displayBooksAvailable();
                        break;
                    case 4:
                    {
                        string isbn;
                        cout << GREEN << "Enter ISBN: " << RESET;
                        getline(cin, isbn);
                        if (users[index].searchBook(isbn))
                        {
                            cout << CYAN << "\nBook found!\n"
                                 << RESET;
                        }
                        else
                        {
                            cout << RED << "\nBook not found!\n"
                                 << RESET;
                        }
                        break;
                    }
                    case 5:
                    {
                        string isbn;
                        cout << GREEN << "Enter ISBN: " << RESET;
                        getline(cin, isbn);
                        users[index].issueBook(isbn);
                        writeBooksToFile();
                        writeUsersToFile();
                        break;
                    }
                    case 6:
                    {
                        users[index].payFine();
                        writeUsersToFile();
                        break;
                    }
                    case 7:
                    {
                        users[index].reportLostBook();
                        writeUsersToFile();
                        break;
                    }
                    case 8:
                    {
                        cout << RED << "Exiting...\n"
                             << RESET;
                        flag = 0;
                        break;
                    }
                    default:
                        cout << RED << "Invalid choice. Please enter a number between 1 and 6.\n"
                             << RESET;
                    }
                    date = incrementDateBy5Days(date);
                }
            }
            break;
        case 3:
            clearScreen();
            // Code for user registration
            cout << CYAN << "Registering as a new user...\n"
                 << RESET;

            cout << CYAN << "Enter user id: " << RESET;
            getline(cin, id);
            cout << CYAN << "\nEnter username: " << RESET;
            getline(cin, username);
            cout << CYAN << "\nEnter password: " << RESET;
            getline(cin, password);
            flag = 1;
            for (auto it : users)
            {
                if (it.getID() == id)
                {
                    flag = 0;
                }
            }
            if (flag)
            {
                cout << GREEN << "User created successfully...\n"
                     << RESET;
                users.push_back(User(id, username, password, 0.0, 0, {}));
                writeUsersToFile();
            }
            else
            {
                cout << RED << "User with same ID already exists...\n"
                     << RESET;
            }
            break;
        case 4:
            clearScreen();
            // Exiting the program
            cout << RED << "Exiting the library management system. Goodbye!\n"
                 << RESET;
            return 0;
            break;
        default:
            clearScreen();
            cout << RED << "Invalid choice. Please enter a number between 1 and 4.\n"
                 << RESET;
        }
    }

    return 0;
}

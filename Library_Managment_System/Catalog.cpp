#include "Catalog.hpp"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <cstring>

Catalog::Catalog() {
    initializeDefaultCatalog();
}

void Catalog::initializeDefaultCatalog() {
    books.clear();

    // Default 100 books covering Academic, Fiction, and Sci-Fi categories
    struct DefaultEntry {
        int isbn;
        const char* title;
        const char* author;
        const char* category;
        const char* location;
        int shelfId;
        int gx;
        int gy;
        int copies;
    };

    DefaultEntry initialEntries[] = {
        // Shelf A1 - Fiction Classic
        { 1001, "Introduction to Algorithms", "Cormen, Leiserson, Rivest", "Academic", "Shelf A1", 1, 10, 4, 4 },
        { 1002, "Object-Oriented Programming in C++", "Bjarne Stroustrup", "Academic", "Shelf A1", 1, 10, 4, 3 },
        { 1003, "Database System Concepts", "Silberschatz, Korth, Sudarshan", "Academic", "Shelf A1", 1, 10, 4, 5 },
        { 1004, "Computer Systems: A Programmer's Perspective", "Randal E. Bryant", "Academic", "Shelf A1", 1, 10, 4, 5 },
        { 1005, "The C++ Programming Language", "Bjarne Stroustrup", "Academic", "Shelf A1", 1, 10, 4, 4 },
        { 1006, "Operating System Concepts", "Abraham Silberschatz", "Academic", "Shelf A1", 1, 10, 4, 3 },
        { 1007, "Clean Code: A Handbook of Agile Craft", "Robert C. Martin", "Academic", "Shelf A1", 1, 10, 4, 6 },
        { 1008, "Design Patterns: Elements of Reusable Code", "Erich Gamma et al.", "Academic", "Shelf A1", 1, 10, 4, 2 },

        // Shelf A2 - Academic & Systems
        { 1009, "Computer Networks", "Andrew S. Tanenbaum", "Academic", "Shelf A2", 2, 22, 4, 4 },
        { 1010, "Modern Operating Systems", "Andrew S. Tanenbaum", "Academic", "Shelf A2", 2, 22, 4, 4 },
        { 1011, "Structure and Interpretation of Computer Programs", "Harold Abelson", "Academic", "Shelf A2", 2, 22, 4, 2 },
        { 1012, "Artificial Intelligence: A Modern Approach", "Stuart Russell, Peter Norvig", "Academic", "Shelf A2", 2, 22, 4, 5 },
        { 1013, "Compilers: Principles, Techniques, and Tools", "Alfred V. Aho", "Academic", "Shelf A2", 2, 22, 4, 3 },
        { 1014, "The Art of Computer Programming", "Donald E. Knuth", "Academic", "Shelf A2", 2, 22, 4, 2 },
        { 1015, "Discrete Mathematics and Its Applications", "Kenneth H. Rosen", "Academic", "Shelf A2", 2, 22, 4, 6 },

        // Shelf B1 - Sci-Fi Masterpieces
        { 2001, "Dune", "Frank Herbert", "Sci-Fi", "Shelf B1", 3, 35, 4, 7 },
        { 2002, "Neuromancer", "William Gibson", "Sci-Fi", "Shelf B1", 3, 35, 4, 4 },
        { 2003, "Foundation", "Isaac Asimov", "Sci-Fi", "Shelf B1", 3, 35, 4, 5 },
        { 2004, "Snow Crash", "Neal Stephenson", "Sci-Fi", "Shelf B1", 3, 35, 4, 3 },
        { 2005, "Do Androids Dream of Electric Sheep?", "Philip K. Dick", "Sci-Fi", "Shelf B1", 3, 35, 4, 4 },
        { 2006, "2001: A Space Odyssey", "Arthur C. Clarke", "Sci-Fi", "Shelf B1", 3, 35, 4, 4 },
        { 2007, "Hyperion", "Dan Simmons", "Sci-Fi", "Shelf B1", 3, 35, 4, 3 },
        { 2008, "The Left Hand of Darkness", "Ursula K. Le Guin", "Sci-Fi", "Shelf B1", 3, 35, 4, 2 },

        // Shelf B2 - Sci-Fi & Speculative
        { 2009, "Ender's Game", "Orson Scott Card", "Sci-Fi", "Shelf B2", 4, 48, 4, 6 },
        { 2010, "The Hitchhiker's Guide to the Galaxy", "Douglas Adams", "Sci-Fi", "Shelf B2", 4, 48, 4, 8 },
        { 2011, "Fahrenheit 451", "Ray Bradbury", "Sci-Fi", "Shelf B2", 4, 48, 4, 5 },
        { 2012, "Brave New World", "Aldous Huxley", "Sci-Fi", "Shelf B2", 4, 48, 4, 4 },
        { 2013, "Solaris", "Stanislaw Lem", "Sci-Fi", "Shelf B2", 4, 48, 4, 3 },
        { 2014, "The Three-Body Problem", "Cixin Liu", "Sci-Fi", "Shelf B2", 4, 48, 4, 5 },

        // Shelf C1 - Fiction Literature
        { 3001, "To Kill a Mockingbird", "Harper Lee", "Fiction", "Shelf C1", 5, 60, 4, 6 },
        { 3002, "1984", "George Orwell", "Fiction", "Shelf C1", 5, 60, 4, 9 },
        { 3003, "The Great Gatsby", "F. Scott Fitzgerald", "Fiction", "Shelf C1", 5, 60, 4, 5 },
        { 3004, "Pride and Prejudice", "Jane Austen", "Fiction", "Shelf C1", 5, 60, 4, 4 },
        { 3005, "The Catcher in the Rye", "J.D. Salinger", "Fiction", "Shelf C1", 5, 60, 4, 3 },
        { 3006, "Crime and Punishment", "Fyodor Dostoevsky", "Fiction", "Shelf C1", 5, 60, 4, 4 },
        { 3007, "The Hobbit", "J.R.R. Tolkien", "Fiction", "Shelf C1", 5, 60, 4, 7 },
        { 3008, "One Hundred Years of Solitude", "Gabriel Garcia Marquez", "Fiction", "Shelf C1", 5, 60, 4, 3 }
    };

    size_t count = sizeof(initialEntries) / sizeof(initialEntries[0]);
    for (size_t i = 0; i < count; ++i) {
        const auto& e = initialEntries[i];
        books.emplace_back(e.isbn, e.title, e.author, e.category, e.location, e.shelfId, e.gx, e.gy, e.copies);
    }

    // Expand catalog to 100 entries for large-scale benchmarks as requested
    int currentId = 3009;
    const char* genreList[] = { "Fiction", "Sci-Fi", "Academic", "Quiet Archive" };
    while (books.size() < 100) {
        char dynamicTitle[64];
        snprintf(dynamicTitle, sizeof(dynamicTitle), "Special Collection Volume %zu", books.size() + 1);
        char dynamicAuthor[48];
        snprintf(dynamicAuthor, sizeof(dynamicAuthor), "Scholar Archon #%zu", books.size() + 1);
        int shelfIdx = 1 + (int)(books.size() % 5);
        char shelfLoc[24];
        snprintf(shelfLoc, sizeof(shelfLoc), "Shelf %c%d", 'A' + (shelfIdx - 1), (int)(books.size() % 3) + 1);
        
        books.emplace_back(
            currentId++,
            dynamicTitle,
            dynamicAuthor,
            genreList[books.size() % 4],
            shelfLoc,
            shelfIdx,
            10 + (shelfIdx * 10),
            4,
            3 + (int)(books.size() % 5)
        );
    }

    sortByISBN();
}

void Catalog::setBooks(const std::vector<Book>& loadedBooks) {
    if (!loadedBooks.empty()) {
        books = loadedBooks;
        sortByISBN();
    }
}

void Catalog::sortByISBN() {
    if (books.empty()) return;
    quickSortISBN(0, static_cast<int>(books.size()) - 1);
    isSortedByISBN = true;
}

void Catalog::quickSortISBN(int low, int high) {
    if (low < high) {
        int pi = partitionISBN(low, high);
        quickSortISBN(low, pi - 1);
        quickSortISBN(pi + 1, high);
    }
}

int Catalog::partitionISBN(int low, int high) {
    int pivot = books[high].isbn;
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (books[j].isbn < pivot) {
            ++i;
            std::swap(books[i], books[j]);
        }
    }
    std::swap(books[i + 1], books[high]);
    return i + 1;
}

void Catalog::sortByTitle() {
    // Insertion sort by title for demonstration of O(N^2) sorting in PF
    int n = static_cast<int>(books.size());
    for (int i = 1; i < n; ++i) {
        Book key = books[i];
        int j = i - 1;
        while (j >= 0 && std::string(books[j].title) > std::string(key.title)) {
            books[j + 1] = books[j];
            j = j - 1;
        }
        books[j + 1] = key;
    }
    isSortedByISBN = false;
}

SearchResult Catalog::searchByISBN(int targetISBN) {
    if (!isSortedByISBN) {
        sortByISBN();
    }

    SearchResult result;
    result.searchType = SearchType::BINARY_ISBN;
    result.found = false;
    result.operationsCount = 0;

    int left = 0;
    int right = static_cast<int>(books.size()) - 1;

    // Classic Binary Search O(log N)
    while (left <= right) {
        result.operationsCount++;
        int mid = left + (right - left) / 2;

        if (books[mid].isbn == targetISBN) {
            result.found = true;
            result.matchedBook = books[mid];
            result.matchCount = 1;
            result.matchedIndices[0] = mid;
            return result;
        }

        if (books[mid].isbn < targetISBN) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return result;
}

bool Catalog::caseInsensitiveSubstr(const std::string& str, const std::string& sub) {
    if (sub.empty()) return true;
    if (str.size() < sub.size()) return false;

    auto it = std::search(
        str.begin(), str.end(),
        sub.begin(), sub.end(),
        [](char ch1, char ch2) {
            return std::tolower(static_cast<unsigned char>(ch1)) ==
                   std::tolower(static_cast<unsigned char>(ch2));
        }
    );
    return (it != str.end());
}

SearchResult Catalog::searchByTitleOrAuthor(const std::string& query) {
    SearchResult result;
    result.searchType = SearchType::LINEAR_TITLE;
    result.found = false;
    result.operationsCount = 0;
    result.matchCount = 0;

    // Linear Search O(N) over all items
    for (size_t i = 0; i < books.size(); ++i) {
        result.operationsCount++;
        std::string titleStr = books[i].title;
        std::string authorStr = books[i].author;

        if (caseInsensitiveSubstr(titleStr, query) || caseInsensitiveSubstr(authorStr, query)) {
            if (!result.found) {
                result.found = true;
                result.matchedBook = books[i]; // Store primary match
            }
            if (result.matchCount < 32) {
                result.matchedIndices[result.matchCount++] = static_cast<int>(i);
            }
        }
    }

    return result;
}

Book* Catalog::findBookByISBN(int isbn) {
    for (auto& b : books) {
        if (b.isbn == isbn) return &b;
    }
    return nullptr;
}

bool Catalog::borrowBook(int isbn) {
    Book* b = findBookByISBN(isbn);
    if (b && b->copies > 0) {
        b->copies--;
        return true;
    }
    return false;
}

bool Catalog::returnBook(int isbn) {
    Book* b = findBookByISBN(isbn);
    if (b) {
        b->copies++;
        return true;
    }
    return false;
}

std::vector<Book> Catalog::getBooksByShelf(int shelfId) const {
    std::vector<Book> res;
    for (const auto& b : books) {
        if (b.shelfId == shelfId) {
            res.push_back(b);
        }
    }
    return res;
}

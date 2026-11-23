#pragma once

#include "Structs.hpp"
#include <vector>
#include <string>

class Catalog {
private:
    std::vector<Book> books;
    bool isSortedByISBN{ false };

    void quickSortISBN(int low, int high);
    int partitionISBN(int low, int high);
    static bool caseInsensitiveSubstr(const std::string& str, const std::string& sub);

public:
    Catalog();

    void initializeDefaultCatalog();
    void setBooks(const std::vector<Book>& loadedBooks);
    const std::vector<Book>& getBooks() const { return books; }
    std::vector<Book>& getBooks() { return books; }
    size_t getBookCount() const { return books.size(); }

    SearchResult searchByISBN(int targetISBN);
    SearchResult searchByTitleOrAuthor(const std::string& query);

    void sortByISBN();
    void sortByTitle();

    Book* findBookByISBN(int isbn);
    bool borrowBook(int isbn);
    bool returnBook(int isbn);
    std::vector<Book> getBooksByShelf(int shelfId) const;
};

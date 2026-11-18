#pragma once

#include "Structs.hpp"
#include <vector>
#include <string>

// ============================================================================
// Catalog: Dual-Engine Search & Dynamic Inventory Management
// Implements O(log N) Binary Search, O(N) Linear Search, and Sorting Algorithms
// ============================================================================

class Catalog {
private:
    std::vector<Book> books;
    bool isSortedByISBN{ false };

    // Helper sorting routines
    void quickSortISBN(int low, int high);
    int partitionISBN(int low, int high);
    static bool caseInsensitiveSubstr(const std::string& str, const std::string& sub);

public:
    Catalog();

    // Population & Initializing
    void initializeDefaultCatalog();
    void setBooks(const std::vector<Book>& loadedBooks);
    const std::vector<Book>& getBooks() const { return books; }
    std::vector<Book>& getBooks() { return books; }
    size_t getBookCount() const { return books.size(); }

    // Dual-Engine Search Functionality
    SearchResult searchByISBN(int targetISBN);
    SearchResult searchByTitleOrAuthor(const std::string& query);

    // Sorting Routines
    void sortByISBN();
    void sortByTitle();

    // Inventory operations
    Book* findBookByISBN(int isbn);
    bool borrowBook(int isbn);
    bool returnBook(int isbn);
    std::vector<Book> getBooksByShelf(int shelfId) const;
};

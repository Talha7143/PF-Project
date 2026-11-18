#include "../Library_Managment_System/Catalog.hpp"
#include <iostream>
#include <cassert>

// ============================================================================
// Unit Test: Search Algorithm Verification
// Tests Binary Search O(log N) and Linear Search O(N) edge cases & correctness
// ============================================================================

void testBinarySearchValid() {
    Catalog catalog;
    SearchResult res = catalog.searchByISBN(1004);
    assert(res.found == true);
    assert(res.matchedBook.isbn == 1004);
    assert(res.operationsCount <= 7); // ceil(log2(100)) is 7
    std::cout << "[PASS]: testBinarySearchValid (Executed in " << res.operationsCount << " ops)\n";
}

void testBinarySearchNotFound() {
    Catalog catalog;
    SearchResult res = catalog.searchByISBN(9999);
    assert(res.found == false);
    assert(res.operationsCount <= 7);
    std::cout << "[PASS]: testBinarySearchNotFound\n";
}

void testLinearSearchSubstring() {
    Catalog catalog;
    SearchResult res = catalog.searchByTitleOrAuthor("Algorithms");
    assert(res.found == true);
    assert(res.matchCount >= 1);
    std::cout << "[PASS]: testLinearSearchSubstring (Matches: " << res.matchCount << ")\n";
}

void testLinearSearchCaseInsensitivity() {
    Catalog catalog;
    SearchResult resUpper = catalog.searchByTitleOrAuthor("DUNE");
    SearchResult resLower = catalog.searchByTitleOrAuthor("dune");
    assert(resUpper.found == resLower.found);
    std::cout << "[PASS]: testLinearSearchCaseInsensitivity\n";
}

int main() {
    std::cout << "Running Search Algorithm Unit Tests...\n";
    testBinarySearchValid();
    testBinarySearchNotFound();
    testLinearSearchSubstring();
    testLinearSearchCaseInsensitivity();
    std::cout << "All Search Tests Passed Successfully!\n";
    return 0;
}

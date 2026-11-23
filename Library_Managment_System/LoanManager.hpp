#include "LoanManager.hpp"
#include <cstring>
#include <iostream>

LoanManager::LoanManager() {
}

bool LoanManager::issueLoan(int patronId, const char* patronName, int isbn, Catalog& catalog, int currentDay, std::string& outMsg) {
    Book* book = catalog.findBookByISBN(isbn);
    if (!book) {
        outMsg = "Error: Book with ISBN " + std::to_string(isbn) + " does not exist in catalog.";
        return false;
    }

    if (book->copies <= 0) {
        outMsg = "Error: All copies of '" + std::string(book->title) + "' are currently checked out.";
        return false;
    }

    book->copies--;

    ActiveLoan loan;
    loan.loanId = nextLoanId++;
    loan.patronId = patronId;
    strncpy_s(loan.patronName, patronName ? patronName : "Patron", sizeof(loan.patronName));
    loan.isbn = isbn;
    strncpy_s(loan.bookTitle, book->title, sizeof(loan.bookTitle));
    loan.issueDay = currentDay;
    loan.dueDay = currentDay + 3;
    loan.isReturned = false;
    loan.fineAmount = 0.0f;

    activeLoans.push_back(loan);
    totalIssued++;

    outMsg = "LOAN APPROVED: '" + std::string(book->title) + "' issued to " + std::string(loan.patronName) + " (Due Day " + std::to_string(loan.dueDay) + ")";
    std::cout << "[LOAN CIRCULATION]: " << outMsg << "\n";
    return true;
}

bool LoanManager::returnLoan(int isbn, Catalog& catalog, int currentDay, float& fineCollected, std::string& outMsg) {
    fineCollected = 0.0f;

    for (auto& loan : activeLoans) {
        if (!loan.isReturned && loan.isbn == isbn) {
            loan.isReturned = true;

            Book* book = catalog.findBookByISBN(isbn);
            if (book) {
                book->copies++;
            }

            if (currentDay > loan.dueDay) {
                int overdueDays = currentDay - loan.dueDay;
                loan.fineAmount = overdueDays * 2.50f;
                fineCollected = loan.fineAmount;
                totalFinesCollected += fineCollected;
                outMsg = "RETURN PROCESSED (OVERDUE by " + std::to_string(overdueDays) + " days): Late fine of $" + std::to_string(static_cast<int>(fineCollected)) + " collected.";
            } else {
                outMsg = "RETURN PROCESSED ON TIME: '" + std::string(loan.bookTitle) + "' returned in good standing.";
            }

            totalReturned++;
            std::cout << "[LOAN CIRCULATION]: " << outMsg << "\n";
            return true;
        }
    }

    Book* book = catalog.findBookByISBN(isbn);
    if (book) {
        book->copies++;
        totalReturned++;
        outMsg = "RETURN PROCESSED: Restocked copy of '" + std::string(book->title) + "'.";
        return true;
    }

    outMsg = "Error: Book with ISBN " + std::to_string(isbn) + " unrecognized.";
    return false;
}

int LoanManager::getActiveLoanCount() const {
    int count = 0;
    for (const auto& l : activeLoans) {
        if (!l.isReturned) count++;
    }
    return count;
}

float LoanManager::calculateOverdueFines(int currentDay) {
    float totalFines = 0.0f;
    for (auto& l : activeLoans) {
        if (!l.isReturned && currentDay > l.dueDay) {
            int overdueDays = currentDay - l.dueDay;
            l.fineAmount = overdueDays * 2.50f;
            totalFines += l.fineAmount;
        }
    }
    return totalFines;
}

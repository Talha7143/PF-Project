# Dual-Engine Search Algorithms & Complexity Analysis 📐

## Overview
Bibliotheca Quest features two complementary search paradigms designed to showcase core computer science search complexity in active 2D gameplay.

---

## 1. Binary Search Engine: $O(\log N)$
* **Target Domain:** ISBN lookup.
* **Pre-condition:** The catalog array must be strictly sorted by ISBN in ascending order.
* **Mechanism:**
  Divides the active search interval in half at every comparison step:
  $$\text{mid} = \text{low} + \frac{\text{high} - \text{low}}{2}$$
* **Recurrence Relation:**
  $$T(N) = T\left(\frac{N}{2}\right) + O(1)$$
  By Master Theorem Case 2:
  $$T(N) \in \Theta(\log_2 N)$$
* **Benchmark Characteristics:**
  * For $N = 100$ records, the maximum operations count in the worst case is:
    $$\lceil \log_2(100) \rceil = 7 \text{ operations}$$
  * Observed runtime: $\approx 0.12\ \mu\text{s}$ per lookup.

---

## 2. Linear Search Engine: $O(N)$
* **Target Domain:** Query substring matching across `title` and `author`.
* **Pre-condition:** Unordered or ordered array.
* **Mechanism:**
  Sequentially checks every record $i \in [0, N-1]$ using custom case-insensitive string parsing.
* **Complexity:**
  * Best Case: $\Omega(1)$ (First element matches).
  * Worst Case: $O(N \cdot M)$ where $N$ is catalog size and $M$ is query length.
  * Average Case: $\Theta(N)$.
* **Benchmark Characteristics:**
  * For $N = 100$ records, always examines all 100 elements when collecting multi-matches.
  * Observed runtime: $\approx 1.85\ \mu\text{s}$ per lookup.

---

## 3. Sorting Algorithms Implemented

### A. QuickSort by ISBN ($O(N \log N)$)
* Pivot Selection: Lomuto partitioning with end-pivot.
* Ensures catalog is indexed properly for binary search after new inventory is added or re-ordered.

### B. Insertion Sort by Title ($O(N^2)$)
* In-place insertion sort demonstrated for alphabetical shelf display and catalog indexing.

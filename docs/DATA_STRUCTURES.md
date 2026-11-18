# Struct-Based Architecture & Memory Alignment 💾

## Architectural Decisions

### 1. Fixed-Width Buffers vs. Heap Allocated Strings
In standard game development and binary file persistence, dynamic heap allocations (`std::string`) store pointer indirection, making direct `std::ofstream::write(reinterpret_cast<char*>(&struct), sizeof(struct))` unsafe due to shallow copying.

To guarantee **byte-exact binary file serialization**, `Book` and `Patron` use deterministic fixed-width character arrays:
```cpp
struct Book {
    int isbn;               // 4 bytes
    char title[64];         // 64 bytes
    char author[48];        // 48 bytes
    char category[24];      // 24 bytes
    char location[24];      // 24 bytes
    int shelfId;            // 4 bytes
    int gridX;              // 4 bytes
    int gridY;              // 4 bytes
    int copies;             // 4 bytes
    int totalCopies;        // 4 bytes
};
```
* **Total Size:** 184 bytes (4-byte aligned).
* **Cache Line Friendly:** Compact struct fits cleanly into L1 cache blocks.

---

### 2. Bounded Dynamic Array Management
* While `std::vector` provides convenient container semantics, the underlying catalog and queue adhere to bounded array sizes:
  * Catalog: Capped at 100 structured items.
  * Active Patrons: Capped at 8 concurrent NPCs to maintain 60 FPS capped rendering.
  * Collision Map: Pre-allocated spatial bounding boxes.

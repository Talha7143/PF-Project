# Bibliotheca Quest 📚
*Programming Fundamentals - Mid Evaluation Project (Fall 2026)*  
**FAST-NUCES, Lahore**

---

## 🚀 Project Overview
**Bibliotheca Quest** is a 2D library management simulation and game prototype built using **C++** and the **SFML graphics library**. This project demonstrates core programming fundamentals, including structured data modeling, binary file handling persistence, search algorithm implementations, and real-time 2D rendering.

---

## ✨ Key Features
* **SFML 2D Graphical Window:** Manages an 800x600 window with a 60 FPS capped game loop, event polling, and custom color-coded entity rendering.
* **Struct-Based Data Modeling:** Utilizes custom `Book` structs (tracking ISBN, title, author, location, and copies) and `Patron` structs (tracking coordinates, velocity vectors, and patience).
* **Binary File Handling (`fstream`):** Implements automated serialization via `saveCatalogBinary()`, flushing catalog records directly into a local `catalog.dat` file.
* **Search Algorithms:** Built-in console and terminal logs demonstrating both **Binary Search ($O(\log N)$)** and **Linear Search ($O(N)$)** capabilities.
* **Interactive Player Mechanics:** Smooth real-time keyboard movement (WASD/Arrow keys) controlling the librarian entity within screen bounds.
* **Autonomous Patrons & HUD:** Features moving patron entities with boundary collision logic alongside a dynamic on-screen HUD panel rendering real-time statistics.

---

## 🎮 Controls
| Key | Action |
| :--- | :--- |
| **W, A, S, D** / **Arrow Keys** | Move the librarian character across the library floor |
| **TAB** | Execute & log a **Binary Search ($O(\log N)$)** for target catalog items |
| **E** | Execute & log a **Linear Search ($O(N)$)** for query substrings |
| **ESC** / Close Window | Automatically save current catalog state to `catalog.dat` and exit |

---

## 🛠️ Setup & Prerequisites
1. **Compiler:** Visual Studio or Visual Studio Code with a C++ compiler supporting C++17 or higher.
2. **SFML Library:** Installed and linked properly to your development environment workspace.
3. **Font Asset:** Ensure `C:/Windows/Fonts/arial.ttf` is accessible for HUD text rendering, or update the font path in the code.

---

## 👥 Group Members
* **Member 1:** [Muhammad Talha Shahzad / Name] — Project Lead & Setup
* **Member 2:** [Muhammad Noman] — Backend Logic, Structs & File Handling (`fstream`)
* **Member 3:** [Muhammad Haris] — SFML Graphics, UI & Game Loop Integration

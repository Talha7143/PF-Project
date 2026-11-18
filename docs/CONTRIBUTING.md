# Contribution & Code Style Guidelines 🛠️

## Course Standards (Programming Fundamentals Fall 2026)
This project follows strict academic and production C++ guidelines:

### 1. Naming Conventions
* **Structs & Classes:** PascalCase (e.g., `Catalog`, `PatronManager`, `Vector2D`).
* **Functions & Methods:** camelCase (e.g., `searchByISBN`, `updatePatronAI`).
* **Variables & Struct Members:** camelCase (e.g., `carriedBookIsbn`, `timeOfDay`).
* **Constants & Macros:** UPPER_SNAKE_CASE (e.g., `TARGET_FPS`, `MAX_ACTIVE_PATRONS`).

### 2. Memory Safety & Pointers
* Avoid raw unmanaged dynamic memory where simple stack allocation or deterministic RAII containers suffice.
* Ensure array index bounds checking is performed prior to any lookup.

### 3. Git Workflow
* Keep commits atomic and feature-specific.
* Write informative commit messages describing the subsystem and functionality added.

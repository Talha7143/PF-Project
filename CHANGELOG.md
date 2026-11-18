# Changelog 📜

All notable changes to **Bibliotheca Quest** are documented in this file.

## [v1.0.0] - 2026-11-19
### Added
- Complete Multi-State Machine (`MainMenuState`, `GameplayState`, `CatalogUIState`, `PauseState`, `GameOverState`, `LeaderboardState`).
- Real-time HUD panel displaying active librarian, books carried, patrons waiting, time of day, day number, and score.
- In-game interactive Catalog Terminal UI overlay matching proposal mockup.
- Dual-Engine Search: $O(\log N)$ Binary Search and $O(N)$ Linear Search with real-time operations counter.
- AI Patron simulation with FIFO checkout queues, patience meters, and request dialogue balloons.
- Circulation desk loan manager with loan issue, return, and overdue fine calculations.
- Persistent binary file storage (`catalog.dat`, `savegame.bin`, `scores.dat`, `transaction.log`).
- High-precision algorithmic performance benchmarking suite (1,000 iterations comparison).
- Unit test suites for search algorithms and binary file serialization.
- Automated Windows build and run batch scripts.

## [v0.3.0] - 2026-11-12
### Added
- Initial 30% project prototype with basic rectangular shapes for shelves, patron movement, and hardcoded terminal search triggers.
- Initial binary file saving demonstration (`catalog.dat`).

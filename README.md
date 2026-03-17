# 🏭 NetSim – Factory Network Simulation

## 📌 About The Project

**NetSim** is a robust, turn-based simulation system modeling a factory production line and supply chain network.

Developed in **C++17**, the project demonstrates advanced **Object-Oriented Programming (OOP)** concepts, strong adherence to **SOLID principles** (especially *Single Responsibility* and *Open/Closed*), and practical use of **design patterns**.

The system simulates the flow of manufactured items (*Package*) from initial delivery, through multi-stage processing, to final storage.

It features a modular architecture that supports:
- dynamic network topology parsing  
- robust graph validation  
- customizable reporting strategies  

---

## ✨ Core Architecture & Technical Highlights

### 1. Network Nodes & Probability-Based Routing

The network consists of:
- **Ramp** – sources  
- **Worker** – processing nodes  
- **Storehouse** – sinks  

#### 🔹 Key Concepts

- **Polymorphic Flow**  
  Packages are transferred using interfaces (`IPackageReceiver`) and base classes (`PackageSender`), enabling easy extensibility.

- **Deterministic Randomness**  
  Routing decisions are based on configurable probability distributions (`ReceiverPreferences`).  
  A `ProbabilityGenerator` wrapper allows injection of custom RNGs for fully testable and deterministic behavior.

- **State Safety**  
  Active processing uses `std::optional<Package>` as a buffer, preventing data overwrites during delays (especially for LIFO queues).

---

### 2. Storage & Queue Strategies

#### 🔹 Flexible Queue System

- Workers use an `IPackageQueue` interface supporting:
  - FIFO  
  - LIFO  

#### 🔹 Design Details

- **Adapter Pattern**  
  Queues are implemented as adapters over `std::list`.

- **Why `std::list`?**  
  - doubly-linked list structure  
  - no iterator/pointer invalidation during insertions/deletions  

- **ID Management**  
  - Package IDs are globally tracked  
  - Freed IDs are reused and prioritized for new objects  

---

### 3. Simulation Engine & I/O

#### 🔹 Turn-Based Execution

Each simulation round follows a strict pipeline:

```
Delivery → Passing → Processing → Reporting
```

#### 🔹 Design Patterns

- **Strategy Pattern**  
  Reporting is injected via `std::function`, allowing flexible strategies:
  - `SpecificTurnsReportNotifier`  
  - `IntervalReportNotifier`  

#### 🔹 Graph Validation

- Implemented using **Depth-First Search (DFS)**  
- Ensures:
  - no infinite cycles  
  - every node has a valid path to a `Storehouse`  

---

## 🚀 Technologies

- **Language:** C++17  
- **Build System:** CMake  
- **Libraries:**  
  - `<optional>`  
  - `<memory>`  
  - `<map>`  
  - `<functional>`  
  - `<random>`  
  - `<algorithm>`  

---


## 📊 Features

- production network simulation  
- probabilistic routing  
- modular and extensible architecture  
- deterministic testing support  
- advanced memory management  
- flexible reporting system  

---

## 👨‍💻 Authors

- Aleksander Młynarski
- Mateusz Łaś
- Miłosz Maciejewski

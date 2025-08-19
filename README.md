AVL Tree Database (C++)
Project Overview

This project implements a simple database management system in C++ using an AVL tree as the underlying data structure. The AVL tree maintains balance automatically during insertions and deletions, ensuring that record storage, retrieval, and range queries can all be performed in logarithmic time. The database also supports file input/output to persist data between sessions.

My Role

A provided .hpp skeleton defined the structure of the system. My contribution was implementing the full controller logic in the .cpp file (around 400 lines of code). This included writing the algorithms for AVL insertion, rotations, deletion, and search, as well as integrating persistence with file operations. I also tested the system against large datasets to confirm efficiency and correctness.

Key Concepts Applied

The project reinforced several core computer science concepts: balancing binary search trees with rotations, recursive algorithms for insertion and deletion, file handling in C++, and designing modular object-oriented components that interact through a clean interface.

Outcome

The result is a working in-memory database capable of efficiently handling thousands of records with fast query performance. It demonstrates practical experience with advanced data structures and shows how they can be applied to real database operations.

# CIS 452: Operating Systems

This repository contains lecture notes, assignments, and lab exercises completed as part of **CIS 452: Operating Systems**. The course provides an in-depth understanding of operating system design, architecture, and functionality, focusing on topics like process management, memory management, scheduling, and synchronization.

---

## Overview

Operating Systems (OS) are the backbone of modern computing. They manage hardware resources, provide services for application programs, and enable user interaction with the system. This course covers both the theoretical and practical aspects of OS design, equipping students with the knowledge and skills to understand and develop operating system components.

---

## Table of Contents

### **Concepts**
1. **Unix and Command Line Basics**  
   Introduction to Unix commands, manpages, and text editors for efficient interaction with the system.

2. **What is an Operating System?**  
   Overview of OS responsibilities, system structures, and their role in managing resources.

3. **Processes**  
   Understanding process life cycles, control blocks, scheduling, creation, and termination.

4. **Threads**  
   Benefits, multithreading models, and programming challenges in parallel and concurrent execution.

5. **Process Synchronization**  
   Critical section problem, mutual exclusion, semaphores, and locks.

6. **CPU Scheduling**  
   Algorithms like FCFS, SJF, Priority Scheduling, and Round Robin with optimization criteria.

7. **Memory Management**  
   Paging, segmentation, TLB, and memory allocation strategies.

8. **Virtual Memory and Page Replacement**  
   Techniques to optimize memory usage, including allocation algorithms and thrashing prevention.

9. **I/O Systems**  
   Hard disk and solid-state drive architecture, file systems, and device drivers.

10. **Advanced Topics**  
    Dynamic linking, buddy systems, slab allocation, and memory-mapped files.

---

## Assignments

1. **Pointers (Assignment 1)**  
   Practiced advanced pointer manipulation and memory allocation in C.

2. **Forks (Assignment 2)**  
   Explored process creation using `fork()` and inter-process communication.

3. **Pipes (Assignment 3)**  
   Implemented IPC mechanisms to allow processes to communicate using pipes.

4. **Threads (Assignment 4)**  
   Developed a multithreaded application to demonstrate thread creation and resource sharing.

5. **Peterson’s Solution (Assignment 5)**  
   Implemented mutual exclusion using Peterson’s algorithm for two-process synchronization.

6. **Test-and-Set (Assignment 6)**  
   Practiced implementing atomic locking mechanisms for critical section protection.

7. **Shared Memory (Assignment 7)**  
   Explored shared memory concepts and synchronization between processes.

8. **Semaphores (Assignment 8)**  
   Used counting and binary semaphores to solve producer-consumer problems.

9. **Scheduling (Assignment 9)**  
   Simulated CPU scheduling algorithms like FCFS, SJF, and Round Robin.

10. **Memory Segmentation (Assignment 10)**  
    Implemented and analyzed memory segmentation techniques.

11. **Page Tables (Assignment 11)**  
    Explored paging concepts and developed page table management algorithms.

12. **Virtual Memory (Assignment 12)**  
    Simulated virtual memory allocation, page replacement, and thrashing prevention.

---

## Repository Organization

The repository is divided into the following sections:

1. **Lectures**  
   Organized by chapter:
   - `ch1-intro`: Basics of operating systems.
   - `ch2-os-structures`: OS design and structures.
   - `ch3-processes`: Process management.
   - `ch4-threads`: Threads and multithreading.
   - `ch5-process-sync`: Synchronization mechanisms.
   - `ch6-scheduling`: CPU scheduling algorithms.
   - `ch7-main-memory`: Memory management.
   - `ch8-virtual-memory`: Virtual memory and page replacement.

2. **Assignments**  
   Contains all programming assignments, each in its respective folder (e.g., `assignment1-pointers`, `assignment2-forks`).

3. **Labs**  
   Lab exercises for hands-on practice with OS concepts.

---

## Lessons Learned

This course provided hands-on experience with:
- Process creation, scheduling, and synchronization.
- Memory management techniques, including paging, segmentation, and virtual memory.
- File systems and I/O device management.
- Advanced synchronization methods like semaphores and mutexes.

Through these assignments and exercises, I gained an understanding of how operating systems function as resource managers and facilitators of efficient, secure computing.

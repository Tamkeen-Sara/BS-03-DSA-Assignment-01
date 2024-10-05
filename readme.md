# DSA Assignment 01
This repository contains solutions to the first assignment of the Data Structures and Algorithms (DSA) course (c++). The tasks demonstrate the application of **singly linked lists** and **circular linked lists**, providing optimized solutions to the given problems.
## Table of Contents
1. Project Overview
2. Approach
3. Assumptions
4. Challenges Faced
5. Sample Output Snippets
6. Github Repository Link
## Project Overview
This project consists of two main components:
### Process Scheduler (Task 1):
A circular linked list-based CPU process scheduler that simulates the execution of processes in a round-robin fashion.
### Large Number Primality Checker (Task 2):
A system for generating and checking the primality of 1024-bit numbers using Miller-Rabin primality test.
## Approach
### Task 1 Process Scheduler
The Process Scheduler uses a circular linked list to manage processes. Key aspects include:
#### Data Structure:
1. Process class represents individual processes with attributes like process ID, execution time, and remaining time.
2. ProcessScheduler class manages the circular linked list of processes.
#### Scheduling Algorithm:
1. Implements a round-robin scheduling with a fixed CPU time period (3 units per cycle).
2. Processes are executed in a circular manner by providing appropriate CPU time to every process.
#### Process Management:
1. New processes can be added to the scheduler dynamically.
2. Completed processes are removed from the list and reported.
#### Simulation:
1. The simulate() function runs cycles until all processes are completed.
2. Each cycle updates the remaining time for processes and handles process completion.
### Task 2 Large Number Primality Checker
This component deals with generating and checking the primality of large (1024-bit) numbers. The approach involves:
#### Data Structure:
Custom LargeNumber class to handle 1024-bit numbers using a linked list of 64-bit integers.
#### Number Generation:
Random generation of 1024-bit numbers.
#### Primality Testing:
1. Implementation of the Miller-Rabin primality test.
2. Modular arithmetic operations (addition, multiplication, exponentiation) for large numbers.
#### Optimization:
Appropriate algorithms for operations like division by two and subtraction.
## Assumptions
### Task 1 Process Scheduler:
1. The CPU time per cycle is fixed at 3 units.
2. Processes are identified by integers starting from 1.
3. The simulation continues until all processes are completed.
### Task 2 Large Number Primality Checker:
1. The system assumes 64-bit architecture for optimal performance.
2. The primality test is probabilistic (Miller-Rabin).
3. The random number generator is assumed to provide sufficient randomness.
## Challenges Faced
### Task 1 Process Scheduler:
1. Implementing a circular linked list correctly and handling different cases.
2. Managing memory efficiently, avoiding memory leaks.
3. Correctly updating the head of the list when processes are removed.
### Task 2 Large Number Primality Checker:
1. Implementing efficient arithmetic operations for 1024-bit numbers.
2. Optimizing the Miller-Rabin primality test for large numbers to achieve reasonable performance.
3. Ensuring the correctness of the arithmetic operations.
4. Avoiding memory leaks.
## Sample Output Snippets
### Task 1
1. ![T1.Snippet01](https://github.com/user-attachments/assets/eda0501d-5c5e-4e76-9c52-a6f76616cf71)
2. ![T1.Snippet02](https://github.com/user-attachments/assets/d07c0725-6802-43b0-853b-6fd542c261ca)
### Task 2
1. ![T2.Snippet01](https://github.com/user-attachments/assets/e2f9b830-8db1-4795-a8a6-b04088a90f96)
2. ![T2.Snippet02](https://github.com/user-attachments/assets/61eb1a66-0b4a-4cf2-9d1e-57fd1f2c61b4)
## Github Repository Link
A publicly accessible github link for this repository is given as follows:
[Github Repository Link](https://github.com/Tamkeen-Sara/BS-03-DSA-Assignment-01.git)

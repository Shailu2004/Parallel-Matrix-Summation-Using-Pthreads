# Parallel Matrix Summation Using Pthreads

## Overview
This project implements a parallel matrix summation program in C++ using pthreads. The program efficiently calculates the sum of all elements in an N×N matrix by dividing the work among multiple threads. Key highlights include:

- **Uses Pthreads:** Utilizes the POSIX threads library for creating and managing threads.
- **Parallel Execution:** Splits the matrix among multiple threads to perform concurrent computation.
- **Thread Assignment Display:** Clearly shows which rows each thread is responsible for processing.
- **Execution Time Reporting:** Measures and reports the total time taken for computation.
- **Robust Error Handling:** Validates user input to ensure that only valid integers are accepted for matrix size, matrix elements, and thread count.

## Features
- **Multi-threading:** Implements parallel summation using pthreads.
- **Flexible Input Options:** Supports both manual input and random generation of matrix elements.
- **Work Distribution Visualization:** Displays the assignment of matrix rows to each thread.
- **Performance Metrics:** Reports the total sum and execution time.
- **Error Handling:** Uses a helper function to validate integer input and handles non-numeric input errors robustly.

## Structure and Function Descriptions

### Data Structures
- **`struct ThreadData`:**  
  - **Fields:**
    - `startRow`: The starting row (inclusive) for a thread.
    - `endRow`: The ending row (exclusive) for a thread.
  - **Purpose:**  
    This structure is used to pass the row range information to each thread, so that each thread knows which part of the matrix to process.

### Functions
- **`int getValidInteger(const string &prompt)`**  
  - **Purpose:**  
    Prompts the user for an integer input and validates it.
  - **Operation:**  
    Displays the prompt, reads input using `cin`, and if the input is invalid (non-numeric), clears the error state, ignores the invalid input, and re-prompts until a valid integer is provided.

- **`void getMatrixInput(vector<vector<int>> &matrix, int N)`**  
  - **Purpose:**  
    Populates the matrix based on the user's chosen input mode.
  - **Operation:**  
    Prompts the user to select between manual input (option 1) and random generation (option 2).  
    - **Manual Input:** Uses `getValidInteger` to obtain each matrix element.
    - **Random Generation:** Automatically fills the matrix with random numbers (0–99) and prints the generated matrix.
  - **Error Handling:**  
    If an invalid option is chosen, the program outputs an error message and exits.

- **`void printMatrix(const vector<vector<int>> &matrix)`**  
  - **Purpose:**  
    Prints the entire matrix in a readable format.
  - **Operation:**  
    Iterates through each row and prints its elements.

- **`int getNumThreads(int N)`**  
  - **Purpose:**  
    Obtains a valid number of threads from the user.
  - **Operation:**  
    Prompts the user (using `getValidInteger`) for a thread count and ensures that the value is between 1 and N. Continues to prompt until a valid value is provided.

- **`void printThreadAssignments(const vector<ThreadData>& threadData)`**  
  - **Purpose:**  
    Displays the row ranges assigned to each thread.
  - **Operation:**  
    Iterates over the vector of `ThreadData` structures and prints the start and end rows for each thread along with the total number of rows it will process.

- **`void* computePartialSum(void* arg)`**  
  - **Purpose:**  
    Acts as the thread function that computes the partial sum for its assigned rows.
  - **Operation:**  
    Iterates through the assigned rows and sums the matrix elements. It then safely updates the global `total_sum` using a mutex to ensure thread safety. The thread exits when its work is complete.

- **`void createAndJoinThreads(int num_threads)`**  
  - **Purpose:**  
    Manages the creation, assignment, and joining of threads.
  - **Operation:**  
    - Divides the matrix rows among the threads, distributing extra rows to the first few threads if necessary.
    - Calls `printThreadAssignments` to display the work distribution.
    - Creates threads that execute `computePartialSum` and waits for all threads to finish.
    - Measures and prints the overall execution time.

## Compilation Instructions
Ensure you have a C++ compiler that supports C++11 (or later) and that the pthreads library is installed. To compile the program, run:

```bash
g++ source_code.cpp -o binary_file ( for compile ) 
./binary_file ( for running the code ) 

## Follow these interactive prompts:

Matrix Size:
Enter the size (N) of the matrix.

Input Mode:
Choose 1 for manual input (you will be prompted to enter each element).
Choose 2 for random generation (the program will generate and display a random matrix).

Thread Count:
Enter the number of threads to use (must be between 1 and N).

The program will display the matrix (if generated), the thread assignments, the total sum of the matrix, and the execution time.

```
## Limitations

**Platform Dependency:**
  - This solution uses pthreads and is designed for POSIX-compliant systems (Linux, macOS, etc.).

**Input Validation:**
  - The program robustly handles non-numeric inputs by re-prompting the user, but it assumes that the user will eventually provide valid data.

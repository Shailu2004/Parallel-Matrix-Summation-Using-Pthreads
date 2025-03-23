#include <iostream>
#include <pthread.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <limits>

using namespace std;
using namespace std::chrono;

// Global variables
int N;                      // Size of the matrix (N x N)
vector<vector<int>> matrix; // Matrix stored as a vector of vectors
long long total_sum = 0;    // Shared variable for the final sum

pthread_mutex_t sum_mutex;  // Mutex for synchronizing total_sum updates

// Structure to pass thread-specific data
struct ThreadData {
    int startRow; // starting row (inclusive)
    int endRow;   // ending row (exclusive)
};

// Function prototypes
void getMatrixInput(vector<vector<int>> &matrix, int N);
int getNumThreads(int N);
void printMatrix(const vector<vector<int>> &matrix);
void printThreadAssignments(const vector<ThreadData>& threadData);
void* computePartialSum(void* arg);
void createAndJoinThreads(int num_threads);
int getValidInteger(const string &prompt);


// Helper function to get a valid integer from user input
int getValidInteger(const string &prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cout << "Invalid input. Please enter a valid integer." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear rest of line
            return value;
        }
    }
}

// Function to take matrix input based on user's choice
void getMatrixInput(vector<vector<int>> &matrix, int N) {
    int option = getValidInteger("Choose input mode:\n1. Manual input\n2. Random generation\nEnter option (1 or 2): ");
    
    switch(option) {
        case 1:
            cout<<endl;
            cout << "Enter the matrix elements:" << endl;
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    matrix[i][j] = getValidInteger("Element (" + to_string(i) + ", " + to_string(j) + "): ");
                }
            }
            break;
        case 2:
            cout<<endl;
            srand(static_cast<unsigned int>(time(NULL)));
            // Generate matrix with random numbers between 0 and 99
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    matrix[i][j] = rand() % 100;
                }
            }
            cout << "Generated Matrix:" << endl;
            printMatrix(matrix);
            break;
        default:
            cout << "Invalid option. Exiting." << endl;
            exit(1);
    }
}

// Function to print the matrix
void printMatrix(const vector<vector<int>> &matrix) {
    for (const auto &row : matrix) {
        for (const auto &val : row) {
            cout << val << " ";
        }
        cout << "\n";
    }
}

// Function to get a valid number of threads from the user
int getNumThreads(int N) {
    int threads;
    while (true) {
        threads = getValidInteger("Enter number of threads to use (1 to " + to_string(N) + "): ");
        if (threads > 0 && threads <= N)
            return threads;
        cout << "Invalid input. Please enter a number greater than 0 and less than or equal to " << N << ".\n";
    }
}

// Function to print how many rows each thread will execute
void printThreadAssignments(const vector<ThreadData>& threadData) {
    cout << "\nThread assignments:" << endl;
    for (size_t i = 0; i < threadData.size(); i++) {
        int rowsAssigned = threadData[i].endRow - threadData[i].startRow;
        cout << "Thread " << i << " will process rows " 
             << threadData[i].startRow << " to " << threadData[i].endRow - 1 
             << " (Total rows: " << rowsAssigned << ")" << endl;
    }
}

// Thread function that computes the partial sum of assigned rows
void* computePartialSum(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    long long local_sum = 0;
    for (int i = data->startRow; i < data->endRow; i++) {
        for (int j = 0; j < N; j++) {
            local_sum += matrix[i][j];
        }
    }
    pthread_mutex_lock(&sum_mutex);
    total_sum += local_sum;
    pthread_mutex_unlock(&sum_mutex);
    pthread_exit(nullptr);
}

// Function to create threads, assign work, and join them
void createAndJoinThreads(int num_threads) {
    vector<pthread_t> threads(num_threads);
    vector<ThreadData> threadData(num_threads);

    // Calculate rows per thread and distribute any extra rows
    int rowsPerThread = N / num_threads;
    int extraRows = N % num_threads;
    int currentRow = 0;
    
    // Assign rows to each thread
    for (int i = 0; i < num_threads; i++) {
        threadData[i].startRow = currentRow;
        threadData[i].endRow = currentRow + rowsPerThread + (i < extraRows ? 1 : 0);
        currentRow = threadData[i].endRow;
    }
    
    // Print thread assignments
    printThreadAssignments(threadData);
    
    // Record start time for computation
    auto start_time = high_resolution_clock::now();

    // Create threads with their assigned portion of the matrix
    for (int i = 0; i < num_threads; i++) {
        int rc = pthread_create(&threads[i], nullptr, computePartialSum, (void*)&threadData[i]);
        if (rc) {
            cerr << "Error: unable to create thread, " << rc << "\n";
            exit(-1);
        }
    }
    
    // Wait for all threads to complete execution
    for (int i = 0; i < num_threads; i++) {
        int rc = pthread_join(threads[i], nullptr);
        if (rc) {
            cerr << "Error: unable to join thread, " << rc << "\n";
            exit(-1);
        }
    }
    
    // Record end time for computation and display duration
    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end_time - start_time);
    cout << "\nTotal Sum of Matrix: " << total_sum << "\n";
    cout << "Time taken: " << duration.count() << " microseconds\n";
}

int main() {
    // Get matrix size with validation
    N = getValidInteger("Enter the size of the matrix (N): ");
    matrix.resize(N, vector<int>(N));

    // Get matrix input using the separate function and switch-case
    getMatrixInput(matrix, N);
    
    // Get a valid number of threads
    int num_threads = getNumThreads(N);
    
    // Initialize the mutex
    pthread_mutex_init(&sum_mutex, nullptr);
    
    // Create threads to perform parallel summation and join them
    createAndJoinThreads(num_threads);
    
    // Destroy the mutex to free resources
    pthread_mutex_destroy(&sum_mutex);
    
    return 0;
}

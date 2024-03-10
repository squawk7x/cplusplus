//############################################################################
// Deadlock
//############################################################################

#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

using namespace std;

std::mutex mu;

class LogFile {
    std::mutex _mu;
    std::mutex _mu2;
    ofstream f;

public:
    LogFile() { f.open("log.txt"); }
    void shared_print(string id, int value) {
        std::lock_guard<mutex> locker(_mu);
        std::lock_guard<mutex> locker2(_mu2);
        // f << "From " << id << ": " << value << endl;
        cout << "From " << id << ": " << value << endl;
    }
    void shared_print2(string id, int value) {
        // use SAME ORDER to avoid DEDLOCK
        std::lock_guard<mutex> locker(_mu);
        std::lock_guard<mutex> locker2(_mu2);
        // f << "From " << id << ": " << value << endl;
        cout << "From " << id << ": " << value << endl;
    }

    // Prefer locking single mutex:
    void shared_print3(string id, int value) {
        {
            std::lock_guard<mutex> locker(_mu);
            // do other things
        }
        {
            std::lock_guard<mutex> locker2(_mu2);
            // f << "From " << id << ": " << value << endl;
            cout << "From " << id << ": " << value << endl;
        }
    }
};

void function_1(LogFile& log) {
    for (int i = 0; i > -100; i--) {
        log.shared_print("t1", i);
    }
}

int main() {
    LogFile log;
    std::thread t1(function_1, std::ref(log));
    for (int i = 0; i < 100; i++)
        log.shared_print2(string("from main: "), i);
    t1.join();

    return 0;
}

/* C++ 11 solution to avoid deadlock: std::lock */

// class LogFile {
// 	std::mutex m_mutex;
// 	std::mutex m_mutex_2;
// 	ofstream f;
// public:
// 	LogFile() {
// 		f.open("log.txt");
// 	}
// 	void shared_print(string id, int value) {
		// std::lock(m_mutex, m_mutex_2);
		// std::lock_guard<mutex> locker(m_mutex, std::adopt_lock);
		// std::lock_guard<mutex> locker2(m_mutex_2, std::adopt_lock);
// 		// f << "From " << id << ": " << value << endl;
// 		cout << "From " << id << ": " << value << endl;
// 	}
// 	void shared_print_2(string id, int value) {
// 		std::lock(m_mutex, m_mutex_2);
// 		std::lock_guard<mutex> locker(m_mutex, std::adopt_lock);
// 		std::lock_guard<mutex> locker2(m_mutex_2, std::adopt_lock);
// 		// f << "From " << id << ": " << value << endl;
// 		cout << "From " << id << ": " << value << endl;
// 	}
// };

// void function_1(LogFile& log) {
//     for (int i = 0; i > -100; i--) {
//         log.shared_print(string("from t1: "), i);
//     }
// }

// int main() {
// 	LogFile log;
// 	// Fctor fctor(log);
// 	std::thread t1(function_1, std::ref(log));

// 	for (int i=0; i<100; i++)
// 		log.shared_print("from main", i);

// 	t1.join();

// 	return 0;
// }

/* Avoiding deadlock
1. Prefer locking single mutex.
2. Avoid locking a mutex and then calling a user provided function.
3. Use std::lock() to lock more than one mutex.
4. Lock the mutex in same order for all threads.


Locking Granularity:
- Fine-grained lock: protects small amount of data
- Coarse-grained lock: protects big amount of data
*/

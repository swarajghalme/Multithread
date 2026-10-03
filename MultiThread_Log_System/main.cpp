#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;


// Shared count
int totalLogs = 0;

// Mutex
mutex mtx;


// Common method
void updateTotalLogs(int count)
{
    lock_guard<mutex> lock(mtx);
    totalLogs += count;
}


// ERROR thread
void processErrors(vector<string> errors)
{
    ofstream file("error.txt");

    for (int i = 0; i < errors.size(); i++)
    {
        file << errors[i] << endl;
    }

    //jitni size utne logs
    updateTotalLogs(errors.size());
}


// WARNING thread
void processWarnings(vector<string> warnings)
{
    ofstream file("warning.txt");

    for (string log : warnings)
    {
        file << log << endl;
    }

    updateTotalLogs(warnings.size());
}


// SUCCESS thread
void processSuccess(vector<string> success)
{
    ofstream file("success.txt");

    for (string log : success)
    {
        file << log << endl;
    }

    updateTotalLogs(success.size());
}


int main()
{
    ifstream input("log.txt");

    string line;

    vector<string> errors;
    vector<string> warnings;
    vector<string> success;


    while (getline(input, line))


    {

        // string::npos - NOT FOUND
        if (line.find("ERROR") != string::npos)
        {
            errors.push_back(line);
        }
        else if (line.find("WARNING") != string::npos)
        {
            warnings.push_back(line);
        }
        else if (line.find("SUCCESS") != string::npos)
        {
            success.push_back(line);
        }
    }

    input.close();


    // Create 3 threads
    thread t1(processErrors, errors);
    thread t2(processWarnings, warnings);
    thread t3(processSuccess, success);


    // Wait for all threads
    t1.join();
    t2.join();
    t3.join();


    cout << "Total logs processed: " << totalLogs << endl;
    cout << "Log processing completed!" << endl;

    return 0;
}
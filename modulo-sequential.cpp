#include <iostream>
#include <omp.h>
#include <cmath>
#include <chrono>

using namespace std;

constexpr auto MAX_RANGE = 50* 1000000;
constexpr auto MIN_RANGE = 25 * 1000000;
bool resultMatrix[MAX_RANGE - MIN_RANGE + 1];
int primesCount = 0;



void printPrimes(int minRange, int maxRange,int a, int b) {
    int tmp=0;
    
    for (int i = a - minRange; i < b - minRange + 1; i++)
    {
        if (resultMatrix[i] == true)
        {
            cout << minRange + i << " ";
            tmp++;
            if (tmp % 10 == 0)
            {
                cout << endl;
            }
        }
    }
    cout << endl;
}


void countPrimes(int minRange, int maxRange, int a, int b) {
    int primes_num = 0;
    if(a<minRange || b>maxRange){
        std::cout << "Przedzial <a,b> poza sprawdzaonym zakresem"<< std::endl;
        exit(1);
    }
    for (int i = a - minRange; i < b - minRange + 1; i++)
    {
        if (resultMatrix[i] == true)
        {
            primes_num++;
        }
    }
    std::cout << std::endl;
    std::cout << "W przedziale ["<<a<<", "<<b<<"] znaleziono " << primes_num << " liczb pierwszych" << std::endl;
}
void getAB(int argc, char **argv, int counter, int *a, int *b){
    if (argc >= counter + 2){
        string arg = argv[counter ];
        try {
            size_t pos;
            *a = stoi(arg, &pos);
            if (pos < arg.size()) {
                cerr << "Trailing characters after number: " << arg << '\n';
            }
        } catch (invalid_argument const &ex) {
            cerr << "Invalid number: " << arg << '\n';
            exit(1);
        } catch (out_of_range const &ex) {
            cerr << "Number out of range: " << arg << '\n';
            exit(1);
        }
        
        string arg2 = argv[counter + 1];
        try {
            size_t pos;
            *b = stoi(arg2, &pos);
            if (pos < arg2.size()) {
                cerr << "Trailing characters after number: " << arg2 << '\n';
            }
        } catch (invalid_argument const &ex) {
            cerr << "Invalid number: " << arg2 << '\n';
            exit(1);
        } catch (out_of_range const &ex) {
            cerr << "Number out of range: " << arg2 << '\n';
            exit(1);
        }
     }else{
         cerr << "Nie podano zakresu liczb pierwszych do wyswietlenia" << '\n';
         exit(1);
    }
}
void clearResultMatrix(int minRange, int maxRange, bool resultMatrix[]) {
    for (int i = 0; i < maxRange - minRange + 1; i++)
    {
        resultMatrix[i] = true;
    }
}

void divisonSequential(int minRange, int maxRange, bool resultMatrix[]) {
    for (int i = minRange; i <= maxRange; i++)
    {
        for (int j = 2; j * j <= i; j++)
        {
            if (i % j == 0)
            {
                resultMatrix[i - minRange] = false;
                break;
            }
        }
    }
}


int main(int argc, char **argv){
    bool printAllPrimes=false;
    int a, b = 0;
    
    if (argc >= 2){
        string arg = argv[1];
        if(arg=="-p"){
            printAllPrimes = true;
            getAB(argc, argv, 2, &a, &b);
        }
    }

    clearResultMatrix(MIN_RANGE, MAX_RANGE, resultMatrix);

    
    cout << "Modulo sekwencyjnie" << "\n";
    divisonSequential(MIN_RANGE, MAX_RANGE, resultMatrix);


    if(printAllPrimes){
        countPrimes(MIN_RANGE, MAX_RANGE, a, b);
        printPrimes(MIN_RANGE, MAX_RANGE, a, b);
    }else{
        countPrimes(MIN_RANGE, MAX_RANGE, MIN_RANGE, MAX_RANGE);
    }
    
}
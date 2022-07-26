#include <iostream>
#include <omp.h>
#include <cmath>
#include <chrono>

using namespace std;
int numThreads = 1;

constexpr auto MAX_RANGE = 50* 1000000;
constexpr auto MIN_RANGE = 25 * 1000000;
bool resultMatrix[MAX_RANGE - MIN_RANGE + 1];
int primesMatrix[(int)std::sqrt(MAX_RANGE)+1];
int chunkSize = 1000000;
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

void findPrimes(int maxRange, int primesMatrix[], int *primesCount){
    int i = 0;
    for (int num = 2; num <= (int)pow(maxRange, 1.0 / 2); num++)
	{
		bool flag = true;
		for (int div = 2; div <= pow(num, 1.0 / 2); div++)
		{
			if (num%div == 0)
			{
				flag = false;
				break;
			}
		}
		if (flag)
		{
			primesMatrix[i++] = num;
            *primesCount = *primesCount + 1;
		}
	}
}
void calcRange(int start, int end, int prime, int *left, int *right){
    *left = std::max((double)2, ceil((double)start/(double)prime));
    *right = floor((double)end/(double)prime);
}

void sieveDomainParallel(int minRange, int maxRange, bool resultMatrix[], int primesMatrix[], int primesCount, int chunkSize){ 
	#pragma omp parallel 
    #pragma omp for schedule(static, 1) nowait
	for (int chunkNum = 0; chunkNum <= (maxRange-minRange)/chunkSize ; chunkNum++)
	{
		int localStart = minRange + chunkNum * chunkSize;
		int localEnd = localStart + chunkSize;

		if (localEnd > maxRange){
            localEnd = maxRange;
        }

		for (int i = 0; i < primesCount; i++)
		{
            int prime = primesMatrix[i];
            int leftRange,rightRange = 0;
            calcRange(localStart, localEnd, prime, &leftRange, &rightRange);

            for (int j = leftRange; j <= rightRange; j++)
            {
                resultMatrix[j*prime - minRange] = false;
            }
		}
	}
}


int main(int argc, char **argv){
    bool printAllPrimes=false;
    int a, b = 0;
    if (argc >= 2){
        string arg = argv[1];
        try {
            size_t pos;
            numThreads = stoi(arg, &pos);
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
        cout << "Liczba watkow: " << numThreads << "\n";
        
    }else{
        cerr << "Za malo argumentow" << '\n';
        exit(1);
    }
    if (argc >= 3){
        string arg = argv[2];
        if(arg=="-p"){
            printAllPrimes = true;
            getAB(argc, argv, 3, &a, &b);
        }
    }
    omp_set_num_threads(numThreads);

    clearResultMatrix(MIN_RANGE, MAX_RANGE, resultMatrix);
    findPrimes(MAX_RANGE, primesMatrix, &primesCount);
    
    cout << "Sito rownolegle domenowe" << "\n";
    sieveDomainParallel(MIN_RANGE, MAX_RANGE, resultMatrix, primesMatrix, primesCount, chunkSize);
        
    if(printAllPrimes){
        countPrimes(MIN_RANGE, MAX_RANGE, a, b);
        printPrimes(MIN_RANGE, MAX_RANGE, a, b);
    }else{
        countPrimes(MIN_RANGE, MAX_RANGE, MIN_RANGE, MAX_RANGE);
    }
    
}
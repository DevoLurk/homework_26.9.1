#include<iostream>
#include<iomanip>
#include<thread>
#include<chrono>
#include<vector>

#define SIZE 100000000 // arr size
#define THRDS 2       // number of threads 

class IntArray
{
private:
    int* arr;
    size_t M;   // M — количество частей, на которые будет разбит массив.
    size_t arr_size;

public:
    IntArray(int threads_cnt);
    ~IntArray();
    void fillArray(int min, int max);
    void printArray(int width = 16);
    int calcThreads();
    int calcNoThreads();
    void sum_of_part(int* arr, int l_index, int r_index, int& result);

};


int main()
{
    IntArray array1(THRDS);
    long elapsed_time;
    int result = 0;

    auto print = [&](char* str) { std::cout << str << ':' << std::endl << "\tSUM: " << result << "  Time (ms): " << elapsed_time << std::endl; };

    array1.fillArray(-10, 10);
	//array1.printArray();
    std::cout << std::endl;

    auto t_threads_start = std::chrono::system_clock::now();
    result = array1.calcThreads();
    auto t_threads_end = std::chrono::system_clock::now();
    elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(t_threads_end - t_threads_start).count();
    print((char*)"Threads");

    auto t_NOthreads_start = std::chrono::system_clock::now();
    result = array1.calcNoThreads();
    auto t_NOthreads_end = std::chrono::system_clock::now();
    elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(t_NOthreads_end - t_NOthreads_start).count();
    print((char*)"No Threads");

    return 0;
}

void IntArray::fillArray(int min, int max)
{
	srand(time(NULL));

	for (auto i{ 0 }; i < arr_size; i++)
	{
		arr[i] = min + rand() % (abs(min) + abs(max) + 1);
	}
}

void IntArray::printArray(int width)
{
	for (auto i{ 0 }; i < arr_size; i++)
	{
		if (i % width == 0)
			std::cout << std::endl;

		std::cout << std::setw(4) << arr[i] << ',';
	}
}
int IntArray::calcThreads()
{
    std::unique_ptr < std::thread[] > threads{new std::thread[M]};
    int l_index = 0;
    int r_index = 0;
    size_t elements_count = arr_size / M;
    int result = 0;

    for (int i = 0; i < M; i++)
    {
        if (i == M - 1)
            r_index = arr_size;    //if we have left elements, we consider them
        else
            r_index += elements_count;

        std::thread t(&IntArray::sum_of_part, this, std::ref(arr), l_index, r_index - 1, std::ref(result));

        threads[i] = std::move(t);
        l_index += elements_count;
    }

    for (int i = 0; i < M; i++)
            threads[i].join();

    return result;
}

int IntArray::calcNoThreads()
{
    int result = 0;

    for (auto i{0}; i < arr_size; i++)
    {
        result += arr[i];
    }
    return result;
}

void IntArray::sum_of_part(int* arr, int l_index, int r_index, int& result)
{
    int s = 0;
    while (l_index <= r_index)
    {
        s += arr[l_index++];
    }
    result += s;
}

IntArray::IntArray(int threads_cnt)
{
    arr = new int[SIZE];
    M = threads_cnt;
    arr_size = SIZE;
}

IntArray::~IntArray()
{
    delete [] arr;
}
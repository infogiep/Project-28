#include <iostream>
#include <vector>
#include <future>
#include <algorithm>
#include <fstream>
#define PART_SIZE 5000
#define RAND_ARR_SIZE 2000

void print(const std::vector<int>& arr)
{
    for (int num : arr)
        std::cout << num << " ";
    std::cout << std::endl;
}

void merge(std::vector<int>& arr, int low, int mid, int high)
{
    std::vector<int> temp(high - low + 1);
    int i = low;
    int j = mid + 1;
    int k = 0;
    while (i <= mid && j <= high) {
        if (arr[i] <= arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }
    while (i <= mid)
        temp[k++] = arr[i++];
    while (j <= high)
        temp[k++] = arr[j++];
    for (int p = 0; p < k; p++)
        arr[low + p] = temp[p];
}

void mergeSort(std::vector<int>& arr, int low, int high)
{
    if (low < high) {
        if (high - low + 1 <= PART_SIZE) {
            std::sort(arr.begin() + low, arr.begin() + high + 1);
            return;
        }
        int mid = low + (high - low) / 2;
        std::future<void> leftFuture = std::async(std::launch::async, mergeSort, std::ref(arr), low, mid);
        std::future<void> rightFuture = std::async(std::launch::async, mergeSort, std::ref(arr), mid + 1, high);

        leftFuture.get();
        rightFuture.get();
        merge(arr, low, mid, high);
    }
}

int main()
{
    std::vector<int> arr(RAND_ARR_SIZE);
    std::ofstream file("array_data.txt");
    if (!file) {
        std::cout << "ERROR creating file." << std::endl;
        return 1;
    }
    srand(time(nullptr));
    for (int i = 0; i < RAND_ARR_SIZE; i++) {
        arr[i] = rand() % RAND_ARR_SIZE;
        file << arr[i] << " ";
    }
    file.close();
    auto start = std::chrono::high_resolution_clock::now();
    mergeSort(arr, 0, arr.size() - 1);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "SORTED array: ";
    print(arr);
    std::cout << "ELAPSED time: " << elapsed.count() << " sec" << std::endl;
    return 0;
}
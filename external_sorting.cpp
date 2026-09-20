#include <_stdio.h>
#include <cstddef>
#include <iostream>
#include <random>
#include <vector>

void sort_and_store_in_sec_storage(long long start_idx, std::vector<long long>& ram, std::vector<long long>& secondary_storage) {
    // sorting
    std::sort(ram.begin(), ram.end());

    // write to secondary storage
    for (long long i = 0; i < (long long)ram.size(); ++i) {
        long long curr_idx = start_idx + i;
        secondary_storage[curr_idx] = ram[i];
    }
}

void clear_ram(std::vector<long long>& ram) {
    for (long long i = 0; i < (long long)ram.size(); ++i) ram[i] = 0;
}

int main() {
    std::cout << "BLOCK SIZE IS 1000 BYTES (~64KB)" << '\n';
    std::cout << "Enter disk size in blocks and RAM size in bytes: (eg. 260000 1000) " << '\n';
    int block_size = 1000;
    long long n; // disk size in blocks
    long long m; // RAM size in bytes
    std::cin >> n >> m; 

    // allocate disk, sec storage, and RAM
    std::vector<long long> disk(n * block_size);
    std::vector<long long> secondary_storage(n * block_size);
    std::vector<long long> ram(m);

    // fill random values in the disk
    std::random_device rd; // obtain a random seed from hardware
    std::mt19937 gen(rd()); // initialize a pseudo-random engine
    std::uniform_int_distribution<long long> distrib(1, (long long)disk.size()); // choose a distribution and set range

    for (long long i = 0; i < (long long)disk.size(); ++i) {
        long long random_number = distrib(gen);
        disk[i] = random_number;
    }

    long long slots = m / block_size;
    if (n > slots * slots) {
        std::cout << "relation size in blocks cannot be greater than slots * slots";
        return 0;
    } 

    // Phase 1:
    // - load each partition (of size `slots`) into RAM
    // - sort each partition in place in RAM
    // - write the sorted partition to secondary memory

    long long ram_idx = 0;
    for (long long i = 0; i < (long long) n * block_size; ++i) {
        ram[ram_idx] = disk[i];
        ram_idx++;

        if ((i + 1) % slots == 0) {
            sort_and_store_in_sec_storage(i + 1 - slots, ram, secondary_storage);
            clear_ram(ram);
            ram_idx = 0;
        }
    }

}


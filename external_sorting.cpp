#include <algorithm>
#include <_stdio.h>
#include <cstddef>
#include <iostream>
#include <queue>
#include <random>
#include <vector>

// helpers

void sort_and_store_in_sec_storage(long long start_idx, long long slots, std::vector<long long>& ram, std::vector<long long>& secondary_storage) {
    // sorting
    std::sort(ram.begin(), ram.begin() + slots);

    // write to secondary storage
    for (long long i = 0; i < slots; ++i) {
        long long curr_idx = start_idx + i;
        secondary_storage[curr_idx] = ram[i];
    }
}

void clear_ram(std::vector<long long>& ram) {
    for (long long i = 0; i < (long long)ram.size(); ++i) ram[i] = 0;
}

struct HeapNode {
    long long value;        // calue stored in RAM
    long long partition_id; //maps to RAM slot (0 to num_partitions - 1)

    bool operator>(const HeapNode& other) const {
        return value > other.value;
    }
};

int main() {
    std::cout << "Enter block size in bytes, disk size in blocks and RAM size in bytes: (eg. 2 5 6) " << '\n';
    int block_size; // size of a block in bytes
    long long n; // disk size in blocks
    long long m; // RAM size in bytes
    std::cin >> block_size >> n >> m; 

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
        ram[ram_idx++] = disk[i];

        if ((i + 1) % slots == 0) {
            sort_and_store_in_sec_storage(i + 1 - slots, slots, ram, secondary_storage);
            clear_ram(ram);
            ram_idx = 0;
        }
    }

    // Phase 2:
    // - merge each sorted partition using RAM and output the sorted data

    std::vector<long long> sorted_relation(n * block_size);
    long long sr_idx = 0;
    long long num_partitions = ((n * block_size) + slots - 1 ) / slots; // calculates ceil of disk size (in B) / slots;
    
    // current index of each partition
    std::vector<long long> partition_cursors(num_partitions);
    for (long long p = 0; p < num_partitions; ++p) {
        partition_cursors[p] = p * slots;
    }

    std::priority_queue<HeapNode, std::vector<HeapNode>, std::greater<HeapNode>> min_heap; // to optimally pick indices from partitions

    // firs elem of each partition from secondary_storage into RAM slots
    for (long long p = 0; p < num_partitions; ++p) {
        long long disk_idx = partition_cursors[p];
        
        if (disk_idx < (long long)secondary_storage.size()) {
            ram[p] = secondary_storage[disk_idx]; // load into RAM slot p
            partition_cursors[p]++;               // advance disk pointer for partition p
            min_heap.push({ram[p], p});
        }
    }

    // PLAYING WITH INDICES!!!
    while (!min_heap.empty()) {
        HeapNode top = min_heap.top();
        min_heap.pop();

        long long p_id = top.partition_id;

        // storing min from RAM into sorted output disk
        sorted_relation[sr_idx++] = top.value;

        // read next element from secondary_storage into RAM slot `p_id`
        long long next_disk_idx = partition_cursors[p_id];
        long long partition_end_idx = std::min((p_id + 1) * slots, (long long)secondary_storage.size());

        if (next_disk_idx < partition_end_idx) {
            ram[p_id] = secondary_storage[next_disk_idx]; // overwrite RAM slot
            partition_cursors[p_id]++;                     // advance disk pointer
            min_heap.push({ram[p_id], p_id});
        }
    }

    // output the sorted disk data
    for (long long i = 0; i < (long long) sorted_relation.size(); ++i) {
        std::cout << sorted_relation[i] << ' ';
    }

}


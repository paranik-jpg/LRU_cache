# LRU Cache: High-Performance Data Eviction

A thread-safe, O(1) time-complexity Least Recently Used (LRU) Cache implementation in C++20. Designed for high-concurrency environments, this cache utilizes a combination of a hash map and a doubly linked list to ensure lightning-fast data retrieval and efficient eviction of stale entries.

## Features
- **Constant Time Complexity**: Achieves O(1) for both `get()` and `put()` operations.
- **Thread Safety**: Uses `std::mutex` and `std::lock_guard` to ensure safe access in multi-threaded environments.
- **Dynamic Eviction**: Automatically removes the least recently used item when the cache reaches its capacity.
- **Optimal Data Structures**: Combines `std::unordered_map` for fast lookup.

## Technical Architecture
- **Hash Map**: Maps keys to iterators of the linked list, allowing for instant access to any element.
- **Doubly Linked List**: Maintains the chronological order of access. The "Most Recently Used" (MRU) items are kept at the front, while "Least Recently Used" (LRU) items are pushed to the back for eviction.
- **Concurrency Strategy**: Employs fine-grained locking to ensure that concurrent read/write requests do not cause race conditions or memory corruption.

## Engineering Challenges & Solutions
- **Challenge**: Race conditions during concurrent `get()` and `put()` calls.
- **Solution**: Implemented a `std::mutex` within the cache class, ensuring that the critical section (updating the linked list and map) is atomic.

## Performance
By utilizing the hash-map-plus-linked-list pattern, this implementation minimizes memory overhead while providing predictable, low-latency access times, making it an ideal component for systems requiring high-frequency data retrieval like database engines or web proxies.

## License
This project is open-source and intended for educational use.
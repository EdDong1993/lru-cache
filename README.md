# lru-cache
A simple lru cache implementation with C++11.

#### usage

```cpp
// without deletor
lru_cache<int, std::string> cache(4);
cache.put(1, "one");

// with deletor
lru_cache<int, int *, std::default_delete<int>> cache_with_deletor(4);
cache_with_deletor.put(1, new int(1));
```
#### test

```
mkdir build
cd build
cmake ..
make -j4
./thread_pool_test
```

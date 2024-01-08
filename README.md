# bln_queue
Simple C++20 message queue.

### Example
```c++
#include <bln_queue/queue.h>

#include <iostream>
#include <string>
#include <thread>

auto main() -> int
{
    bln_queue::queue<std::string> queue;

    std::thread tui{[&queue](){
        std::string in;
        while (std::getline(std::cin, in))
            queue.put(in);
    }};

    std::thread printer{[&queue](){
        while (true)
            std::cout << queue.wait() << std::endl;
    }};

    tui.join();
    printer.join();

    return 0;
}
```

### Build Example
```bash
mkdir build
cd build
cmake -DBLN_QUEUE_EXAMPLES=ON ..
cmake --build . -j
./bin/example
```

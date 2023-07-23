# bln_queue
Small collection of message queues.

### Example
```c++
#include <bln_queue/msg_queue.hpp>

#include <iostream>
#include <string>
#include <thread>

auto main() -> int
{
    bln_queue::msg_queue<std::string> queue;

    std::thread ui{[&queue](){
        std::string in;
        while (std::getline(std::cin, in))
            queue.put(in);
    }};

    std::thread printer{[&queue](){
        while (true)
            std::cout << queue.wait() << std::endl;
    }};

    ui.join();
    printer.join();

    return 0;
}
```

### Build & Run example
```bash
mkdir build
cd build
cmake -DBLN_QUEUE_EXAMPLES=ON ..
cmake --build . -j
./bin/bln_queue_example
```

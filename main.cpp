#include <thread>
#include <iostream>
#include "RingBuffer.h"


void WriteThread()
{
    auto &buffer = RingBuffer<int>::GetInstance();
    for (int i = 1; i < 12; ++i)
    {
        bool ret = buffer.push(i);
        if (ret)
        {
            std::cout << " push  " << i << " success " << std::endl;
        }
        else
        {
            std::cout << " buffer full " << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

}

void ReadThread()
{
    auto &buffer = RingBuffer<int>::GetInstance();
    for (int i = 0; i < 12; ++i)
    {
        int data;
        bool ret = buffer.pop(data);
        if (ret)
        {
            std::cout << " pop " << data << " success " << std::endl;
        }
        else
        {
            std::cout << " buffer empty " << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(900));
    }
}

int main()
{
    RingBuffer<int>::GetInstance().CreateRingBuffer(12);
    RingBuffer<int>::GetInstance().CreateRingBuffer(10);
    std::thread write(WriteThread);
    std::thread read(ReadThread);

    write.join();
    read.join();

    return 0;
}

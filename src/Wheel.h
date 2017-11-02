#ifndef WHEEL_TIMER_WHEEL_H
#define WHEEL_TIMER_WHEEL_H

#include <vector>
#include <stdexcept>

template<class T>
class Wheel {

private:
    std::vector<std::vector<T>> wheel;
    unsigned int index;
    bool cascade = false;

public:
    Wheel(unsigned int size);
    void add(const T &item, unsigned int bucket) noexcept;

    std::unique_ptr<std::vector<T>> nextBucket() noexcept;

    const bool hasCascade() const noexcept;

    const unsigned int lenght;

    const unsigned int remainingTick() const noexcept;

};

template<class T>
Wheel<T>::Wheel(unsigned int size) : lenght{size}, wheel{size} {
    if (size == 0) throw std::runtime_error("Can not create a wheel with size 0");
    index = 0;
}

template<class T>
void Wheel<T>::add(const T &item, unsigned int bucket) noexcept{
    wheel[(bucket+index)%lenght].push_back(item);
}

template <class T>
std::unique_ptr<std::vector<T>> Wheel<T>::nextBucket() noexcept {
    cascade = false;
    if (++index==wheel.size()) {
        index = 0;
        cascade = true;
    }
    auto result = std::make_unique<std::vector<T>>(wheel[index]);
    wheel[index].clear();
    return result;
}

template <class T>
const bool Wheel<T>::hasCascade() const noexcept {
    return cascade;
}

template <class T>
const unsigned int Wheel<T>::remainingTick() const noexcept {
    return lenght - index;
}

#endif //WHEEL_TIMER_WHEEL_H

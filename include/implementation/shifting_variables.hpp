//
// Created by night on 5/9/19.
//

#include <iostream>
#include "shifting_variables.h"

// see: https://stackoverflow.com/a/43220064
#define for_indexed(...) for_indexed_v(i, __VA_ARGS__)
#define for_indexed_v(v, ...) for(bool _i_ = true, _break_ = false; _i_;) for(size_t v = 0; _i_; _i_ = false) for(__VA_ARGS__) if(_break_) break; else for(bool _j_ = true; _j_;) for(_break_ = true; _j_; _j_ = false) for(bool _k_ = true; _k_; v++, _k_ = false, _break_ = false)

template<class T>
shifting_var<T>::shifting_var(const shifting_config& config)
    : m_offsetDelay(config.offsetDelay)
    , m_offsetCount(config.offsetCount)
    , m_preformCheck(config.preformCheck)
    , m_shuffle(config.shuffle)
    , m_memoryPool(m_offsetCount)
    , m_currentPos(0)
    , m_rng(m_device())
    , m_dist(0, m_offsetCount - 1)
{
  m_variable = T();
  m_memoryPool[m_currentPos] = m_variable;

  initializer(this);
}

template<class T>
shifting_var<T>::shifting_var(T variable, const shifting_config& config)
    : m_offsetDelay(config.offsetDelay)
    , m_offsetCount(config.offsetCount)
    , m_preformCheck(config.preformCheck)
    , m_shuffle(config.shuffle)
    , m_memoryPool(m_offsetCount)
    , m_currentPos(0)
    , m_rng(m_device())
    , m_dist(0, m_offsetCount - 1)
{
  m_variable = variable;
  m_memoryPool[m_currentPos] = m_variable;

  initializer(this);
}

template<class T>
shifting_var<T>::shifting_var(T *variable, const shifting_config& config)
    : m_offsetDelay(config.offsetDelay)
    , m_offsetCount(config.offsetCount)
    , m_preformCheck(config.preformCheck)
    , m_shuffle(config.shuffle)
    , m_variable(variable)
    , m_memoryPool(m_offsetCount)
    , m_currentPos(0)
    , m_rng(m_device())
    , m_dist(0, m_offsetCount - 1)
{
  m_variable = variable;
  m_memoryPool[m_currentPos] = m_variable;

  initializer(this);
}

template<class T>
shifting_var<T>::shifting_var(T &variable, const shifting_config& config)
    : m_offsetDelay(config.offsetDelay)
    , m_offsetCount(config.offsetCount)
    , m_preformCheck(config.preformCheck)
    , m_shuffle(config.shuffle)
    , m_variable(variable)
    , m_memoryPool(m_offsetCount)
    , m_currentPos(0)
    , m_rng(m_device())
    , m_dist(0, m_offsetCount - 1)
{
  m_variable = variable;
  m_memoryPool[m_currentPos] = m_variable;

  initializer(this);
}

template<class T>
shifting_var<T>::~shifting_var() {
  // Disable checking incase we attempt to join mid check or shuffle
  m_preformCheck = false;
  m_shuffle = false;

  // Join the threads back to the main process
  for(std::thread& thread : m_thread)
  {
    thread.join();
  }
}

template<class T>
shifting_var<T>::operator T() {
  std::unique_lock<std::mutex> uniqueLock(m_mutex);

  return m_memoryPool[m_currentPos];
}

template<class T>
void shifting_var<T>::initializer(shifting_var *object) {
  m_object = object;

  m_thread[0] = std::thread([&]{validityChecker();});
  m_thread[1] = std::thread([&]{shuffler();});
}

template<class T>
void shifting_var<T>::validityChecker() {
  while(m_preformCheck)
  {
    std::unique_lock<std::mutex> uniqueLock(m_mutex);

    if(m_memoryPool[m_currentPos] != m_object->m_variable)
    {
      // TODO: Add a callback in the config
      // Post detection
      std::cout << "Discrepency detected!";
      std::cout << "\n m_memoryPool: " << m_memoryPool[m_currentPos] << "\n m_object: " << m_object->m_variable << "\n";
      m_object->m_variable = m_memoryPool[m_currentPos];
    }

    uniqueLock.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(m_offsetDelay));
  }
}

template<class T>
void shifting_var<T>::shuffler() {
  while(m_shuffle)
  {
    std::unique_lock<std::mutex> uniqueLock(m_mutex);

    m_currentPos = m_dist(m_rng);
    m_memoryPool[m_currentPos] = m_object->m_variable;

    uniqueLock.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(m_offsetDelay));
  }
}

template<class T>
shifting_var<T> &shifting_var<T>::operator=(const T &lhs) {
  std::unique_lock<std::mutex> uniqueLock(m_mutex);

  m_memoryPool[m_currentPos] = lhs;
  m_object->m_variable = lhs;

  uniqueLock.unlock();

  return *this;
}

template<class T>
template<class T2, typename has_operator_assignment<T, T2>::value>
shifting_var<T> &shifting_var<T>::operator=(const T2 &lhs) {
  std::unique_lock<std::mutex> uniqueLock(m_mutex);

  m_memoryPool[m_currentPos] = lhs;
  m_object->m_variable = lhs;

  uniqueLock.unlock();

  return *this;
}

template<class T>
shifting_var<T> &shifting_var<T>::operator+=(const T &lhs) {
  std::unique_lock<std::mutex> uniqueLock(m_mutex);

  m_memoryPool[m_currentPos] += lhs;
  m_object->m_variable += lhs;

  uniqueLock.unlock();

  return *this;
}

template<class T>
template<class T2, typename has_operator_bitwise_AND_assignment<T, T2>::value>
shifting_var<T> &shifting_var<T>::operator&=(const T2 &lhs) {
  std::unique_lock<std::mutex> uniqueLock(m_mutex);

  m_memoryPool[m_currentPos] &= lhs;
  m_object->m_variable &= lhs;

  uniqueLock.unlock();

  return *this;
}

template<class T>
shifting_var<T> &shifting_var<T>::operator&=(const T &lhs) {
  std::unique_lock<std::mutex> uniqueLock(m_mutex);

  m_memoryPool[m_currentPos] &= lhs;
  m_object->m_variable &= lhs;

  uniqueLock.unlock();

  return *this;
}

template<class T>
template<class T2, typename has_operator_bitwise_OR_assignment<T, T2>::value>
shifting_var<T> &shifting_var<T>::operator|=(const T2 &lhs) {
  std::unique_lock<std::mutex> uniqueLock(m_mutex);

  m_memoryPool[m_currentPos] |= lhs;
  m_object->m_variable |= lhs;

  uniqueLock.unlock();

  return *this;
}

template<class T>
shifting_var<T> &shifting_var<T>::operator|=(const T &lhs) {
  std::unique_lock<std::mutex> uniqueLock(m_mutex);

  m_memoryPool[m_currentPos] |= lhs;
  m_object->m_variable |= lhs;

  uniqueLock.unlock();

  return *this;
}

template<class T>
template<class T2, typename has_operator_bitwise_XOR_assignment<T, T2>::value>
shifting_var<T> &shifting_var<T>::operator^=(const T2 &lhs) {
  std::unique_lock<std::mutex> uniqueLock(m_mutex);

  m_memoryPool[m_currentPos] ^= lhs;
  m_object->m_variable ^= lhs;

  uniqueLock.unlock();

  return *this;
}

template<class T>
shifting_var<T> &shifting_var<T>::operator^=(const T &lhs) {
  std::unique_lock<std::mutex> uniqueLock(m_mutex);

  m_memoryPool[m_currentPos] ^= lhs;
  m_object->m_variable ^= lhs;

  uniqueLock.unlock();

  return *this;
}

template<class T>
template<class T2, typename has_operator_division_assignment<T, T2>::value>
shifting_var<T> &shifting_var<T>::operator/=(const T2 &lhs) {
  std::unique_lock<std::mutex> uniqueLock(m_mutex);

  m_memoryPool[m_currentPos] /= lhs;
  m_object->m_variable /= lhs;

  uniqueLock.unlock();

  return *this;
}

template<class T>
shifting_var<T> &shifting_var<T>::operator/=(const T &lhs) {
  std::unique_lock<std::mutex> uniqueLock(m_mutex);

  m_memoryPool[m_currentPos] /= lhs;
  m_object->m_variable /= lhs;

  uniqueLock.unlock();

  return *this;
}

template<class T>
template<class T2, typename has_operator_left_shift_assignment<T, T2>::value>
shifting_var<T> &shifting_var<T>::operator<<=(const T2 &lhs) {
  std::unique_lock<std::mutex> uniqueLock(m_mutex);

  m_memoryPool[m_currentPos] <<= lhs;
  m_object->m_variable <<= lhs;

  uniqueLock.unlock();

  return *this;
}

template<class T>
shifting_var<T> &shifting_var<T>::operator<<=(const T &lhs) {
  std::unique_lock<std::mutex> uniqueLock(m_mutex);

  m_memoryPool[m_currentPos] <<= lhs;
  m_object->m_variable <<= lhs;

  uniqueLock.unlock();

  return *this;
}

template<class T>
template<class T2, typename has_operator_modulo_assignment<T, T2>::value>
shifting_var<T> &shifting_var<T>::operator%=(const T2 &lhs) {
  std::unique_lock<std::mutex> uniqueLock(m_mutex);

  m_memoryPool[m_currentPos] %= lhs;
  m_object->m_variable %= lhs;

  uniqueLock.unlock();

  return *this;
}

template<class T>
shifting_var<T> &shifting_var<T>::operator%=(const T &lhs) {
  std::unique_lock<std::mutex> uniqueLock(m_mutex);

  m_memoryPool[m_currentPos] %= lhs;
  m_object->m_variable %= lhs;

  uniqueLock.unlock();

  return *this;
}

template<class T>
template<class T2, typename has_operator_multiplication_assignment<T, T2>::value>
shifting_var<T> &shifting_var<T>::operator*=(const T2 &lhs) {
  std::unique_lock<std::mutex> uniqueLock(m_mutex);

  m_memoryPool[m_currentPos] *= lhs;
  m_object->m_variable *= lhs;

  uniqueLock.unlock();

  return *this;
}

template<class T>
shifting_var<T> &shifting_var<T>::operator*=(const T &lhs) {
  std::unique_lock<std::mutex> uniqueLock(m_mutex);

  m_memoryPool[m_currentPos] *= lhs;
  m_object->m_variable *= lhs;

  uniqueLock.unlock();

  return *this;
}

template<class T>
template<class T2, typename has_operator_right_shift_assignment<T, T2>::value>
shifting_var<T> &shifting_var<T>::operator>>=(const T2 &lhs) {
  std::unique_lock<std::mutex> uniqueLock(m_mutex);

  m_memoryPool[m_currentPos] >>= lhs;
  m_object->m_variable >>= lhs;

  uniqueLock.unlock();

  return *this;
}

template<class T>
shifting_var<T> &shifting_var<T>::operator>>=(const T &lhs) {
  std::unique_lock<std::mutex> uniqueLock(m_mutex);

  m_memoryPool[m_currentPos] >>= lhs;
  m_object->m_variable >>= lhs;

  uniqueLock.unlock();

  return *this;
}

template<class T>
template<class T2, typename has_operator_subtraction_assignment<T, T2>::value>
shifting_var<T> &shifting_var<T>::operator-=(const T2 &lhs) {
  std::unique_lock<std::mutex> uniqueLock(m_mutex);

  m_memoryPool[m_currentPos] -= lhs;
  m_object->m_variable -= lhs;

  uniqueLock.unlock();

  return *this;
}

template<class T>
shifting_var<T> &shifting_var<T>::operator-=(const T &lhs) {
  std::unique_lock<std::mutex> uniqueLock(m_mutex);

  m_memoryPool[m_currentPos] -= lhs;
  m_object->m_variable -= lhs;

  uniqueLock.unlock();

  return *this;
}

template<class T>
template<class T2, typename has_operator_addition_assignment<T, T2>::value>
shifting_var<T> &shifting_var<T>::operator+=(const T2 &lhs) {
  std::unique_lock<std::mutex> uniqueLock(m_mutex);

  m_memoryPool[m_currentPos] += lhs;
  m_object->m_variable += lhs;

  uniqueLock.unlock();

  return *this;
}





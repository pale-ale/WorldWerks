#pragma once
#include <functional>

template <typename T>
using GetterFn = std::function<T()>;

template <typename T>
using SetterFn = std::function<void(const T &)>;

template <typename T>
struct Binding {
  GetterFn<T> get = nullptr;
  SetterFn<T> set = nullptr;
};

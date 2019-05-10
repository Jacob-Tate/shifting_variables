//
// Created by night on 5/9/19.
//

#pragma once

#include <type_traits>

template<typename T, typename U>
struct has_operator_assignment
{
private:
  typedef std::true_type  yes;
  typedef std::false_type no;

  template<typename LHS, typename RHS> static decltype(std::declval<LHS>().operator=(std::declval<RHS>()) == 0, yes()) test(int);
  template<typename> static no test(...);
public:
  static constexpr bool value = std::is_same<decltype(test<T,U>(0)), yes>::value;
};

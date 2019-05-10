//
// Created by night on 5/9/19.
//

#pragma once

#include <vector>
#include <thread>
#include <random>
#include <mutex>
#include <type_traits>

#include "shifting_config.h"

// --------------------------------------------------------------------------
// Assignment Operators
//
#include "overloads/assignment/operator_assignment.h"
#include "overloads/assignment/operator_addition_assignment.h"
#include "overloads/assignment/operator_bitwise_AND_assignment.h"
#include "overloads/assignment/operator_bitwise_OR_assignment.h"
#include "overloads/assignment/operator_bitwise_XOR_assignment.h"
#include "overloads/assignment/operator_division_assignment.h"
#include "overloads/assignment/operator_left_shift_assignment.h"
#include "overloads/assignment/operator_modulo_assignment.h"
#include "overloads/assignment/operator_multiplication_assignment.h"
#include "overloads/assignment/operator_right_shift_assignment.h"
#include "overloads/assignment/operator_subtraction_assignment.h"

// --------------------------------------------------------------------------
// Increment Operators
//

// --------------------------------------------------------------------------
// Arithmetic Operators
//

// --------------------------------------------------------------------------
// Logical Operators
//

// --------------------------------------------------------------------------
// Member Access Operators
//

// --------------------------------------------------------------------------
// Other Operators
//


template<class T>
class shifting_var
{
public:
  shifting_var(const shifting_config& config = shifting_config());
  ~shifting_var();

  shifting_var(T variable,  const shifting_config& config = shifting_config());
  shifting_var(T* variable, const shifting_config& config = shifting_config());
  shifting_var(T& variable, const shifting_config& config = shifting_config());

  // --------------------------------------------------------------------------
  // Assignment Operators
  //
  template<class T2, typename has_operator_assignment<T, T2>::value = 0>
  shifting_var<T>& operator=(const T2& lhs);
  shifting_var<T>& operator=(const T& lhs);

  template<class T2, typename has_operator_addition_assignment<T, T2>::value = 0>
  shifting_var<T>& operator+=(const T2& lhs);
  shifting_var<T>& operator+=(const T& lhs);

  template<class T2, typename has_operator_bitwise_AND_assignment<T, T2>::value = 0>
  shifting_var<T>& operator&=(const T2& lhs);
  shifting_var<T>& operator&=(const T& lhs);

  template<class T2, typename has_operator_bitwise_OR_assignment<T, T2>::value = 0>
  shifting_var<T>& operator|=(const T2& lhs);
  shifting_var<T>& operator|=(const T& lhs);

  template<class T2, typename has_operator_bitwise_XOR_assignment<T, T2>::value = 0>
  shifting_var<T>& operator^=(const T2& lhs);
  shifting_var<T>& operator^=(const T& lhs);

  template<class T2, typename has_operator_division_assignment<T, T2>::value = 0>
  shifting_var<T>& operator/=(const T2& lhs);
  shifting_var<T>& operator/=(const T& lhs);

  template<class T2, typename has_operator_left_shift_assignment<T, T2>::value = 0>
  shifting_var<T>& operator<<=(const T2& lhs);
  shifting_var<T>& operator<<=(const T& lhs);

  template<class T2, typename has_operator_modulo_assignment<T, T2>::value = 0>
  shifting_var<T>& operator%=(const T2& lhs);
  shifting_var<T>& operator%=(const T& lhs);

  template<class T2, typename has_operator_multiplication_assignment<T, T2>::value = 0>
  shifting_var<T>& operator*=(const T2& lhs);
  shifting_var<T>& operator*=(const T& lhs);

  template<class T2, typename has_operator_right_shift_assignment<T, T2>::value = 0>
  shifting_var<T>& operator>>=(const T2& lhs);
  shifting_var<T>& operator>>=(const T& lhs);

  template<class T2, typename has_operator_subtraction_assignment<T, T2>::value = 0>
  shifting_var<T>& operator-=(const T2& lhs);
  shifting_var<T>& operator-=(const T& lhs);

  // --------------------------------------------------------------------------
  // Increment Operators
  //

  // --------------------------------------------------------------------------
  // Arithmetic Operators
  //

  // --------------------------------------------------------------------------
  // Logical Operators
  //

  // --------------------------------------------------------------------------
  // Member Access Operators
  //

  // --------------------------------------------------------------------------
  // Other Operators
  //
  operator T();

private:

  const int m_offsetDelay; ///< The offset used to determine when to shuffle
  const int m_offsetCount; ///< The count of space in the memory pool

  /**
   * Initializes the shifting variable
   * @param object The object to use for validity checking
   */
  void initializer(shifting_var* object);

  /**
   * Checks if there are fishy things occuring in our memory
   *
   * @note In the case discrepencies are detected an error is displayed and the value is corrected
   */
  void validityChecker();

  /**
   * Shuffles the variables locations in memory
   */
  void shuffler();

  bool m_preformCheck; ///< Are we going to preform the validity check
  bool m_shuffle;      ///< Are we going to shuffle the memory

  T m_variable; ///< The variable we have stored [Used for validity checking]

  std::vector<T> m_memoryPool; ///< The memory we are going to use to store a shuffled copy [Used for validity checking]
  int m_currentPos; ///< The current position in the memory pool of the shuffled copy

  std::random_device m_device; ///< Random number device
  std::mt19937 m_rng; ///< Random number generator
  std::uniform_int_distribution<int> m_dist; ///< Int distribution

  std::thread m_thread[2]; ///< The threads we are using (checker & shuffler)
  std::mutex m_mutex; ///< Mutex for the variables

  shifting_var* m_object; ///< The object we are using for validity checking
};

#include "implementation/shifting_variables.hpp"

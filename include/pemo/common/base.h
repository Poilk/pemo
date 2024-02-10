//
// Created by poilk on 2024/2/10.
//

#ifndef PEMO_BASE_H
#define PEMO_BASE_H

/*!
 * \brief Disable copy constructor and assignment operator.
 *
 * If C++11 is supported, both copy and move constructors and
 * assignment operators are deleted explicitly. Otherwise, they are
 * only declared but not implemented. Place this macro in private
 * section if C++11 is not available.
 */
#ifndef DISALLOW_COPY_AND_ASSIGN
#  if DMLC_USE_CXX11
#    define DISALLOW_COPY_AND_ASSIGN(T) \
       T(T const&) = delete; \
       T(T&&) = delete; \
       T& operator=(T const&) = delete; \
       T& operator=(T&&) = delete
#  else
#    define DISALLOW_COPY_AND_ASSIGN(T) \
       T(T const&); \
       T& operator=(T const&)
#  endif
#endif

#endif  // PEMO_BASE_H

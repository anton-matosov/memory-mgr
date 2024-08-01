// Copyright (C) 2000 Stephen Cleary
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.memory-mgr.org/LICENSE_1_0.txt)
//
// See http://www.memory-mgr.org for updates, documentation, and revision history.

// This file was AUTOMATICALLY GENERATED from "stdin"
//  Do NOT include directly!
//  Do NOT edit!

template <typename T0>
element_type * construct(const T0 & a0)
{
  element_type * const ret = this->allocate();
  if (ret == 0)
    return ret;
  try { new (ret) element_type(a0); }
  catch (...) { this->deallocate(ret); throw; }
  return ret;
}
template <typename T0, typename T1>
element_type * construct(const T0 & a0, const T1 & a1)
{
  element_type * const ret = this->allocate();
  if (ret == 0)
    return ret;
  try { new (ret) element_type(a0, a1); }
  catch (...) { this->deallocate(ret); throw; }
  return ret;
}
template <typename T0, typename T1, typename T2>
element_type * construct(const T0 & a0, const T1 & a1, const T2 & a2)
{
  element_type * const ret = this->allocate();
  if (ret == 0)
    return ret;
  try { new (ret) element_type(a0, a1, a2); }
  catch (...) { this->deallocate(ret); throw; }
  return ret;
}


/*
Generic Memory Manager (memory-mgr)
http://memory-mgr.sourceforge.net/
Copyright (c) 2007-2008 Anton (shikin) Matosov

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3, 29 June 2007 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA <http://fsf.org/>


Please feel free to contact me via e-mail: shikin@users.sourceforge.net
*/

#pragma once

#define MGR_LINUX_PLATFORM

#include "memory-mgr/config/platform/posix.h"

#include "memory-mgr/detail/temp_buffer.h"
#include "memory-mgr/detail/types.h"

#include <sstream>

namespace memory_mgr
{
namespace osapi
{
static inline std::string get_executable_path()
{
  /*
  Linux:
  /proc/<pid>/exe

  Solaris:
  /proc/<pid>/object/a.out (filename only)
  /proc/<pid>/path/a.out (complete pathname)

  *BSD (and maybe Darwing too):
  /proc/<pid>/file
  */

  detail::char_buffer path(512);
  std::stringstream link;
  link << "/proc/" << getpid() << "/exe";
  auto size = readlink(link.str().c_str(), path, path.count());
  if (size == -1)
  {
    return {};
  }
  else
  {
    path[size] = 0;
  }
  return path.get();
}
}  // namespace osapi
}  // namespace memory_mgr

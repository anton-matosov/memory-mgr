/*
Generic Memory Manager (memory-mgr)
http://memory-mgr.sourceforge.net/
Copyright (c) 2007-2013 Anton Matosov

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

#define MGR_APPLE_PLATFORM

#include "memory-mgr/detail/temp_buffer.h"

#include "memory-mgr/config/platform/posix.h"

#include <libproc.h>
#include <sys/proc_info.h>

namespace memory_mgr
{
namespace osapi
{
static inline std::string get_executable_path()
{
  detail::char_buffer pathbuf(PROC_PIDPATHINFO_MAXSIZE);

  const pid_t pid = getpid();
  const int ret = proc_pidpath(pid, pathbuf, pathbuf.count());
  if (ret <= 0)
  {
    return {};
  }
  else
  {
    return pathbuf.get();
  }
}
}  // namespace osapi
}  // namespace memory_mgr

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

#include <fcntl.h>  //O_CREAT, O_*...
#include <libproc.h>
#include "memory-mgr/detail/temp_buffer.h"
#include "memory-mgr/detail/types.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>  //shm_xxx
#include <sys/stat.h>  //mode_t, S_IRWXG, S_IRWXO, S_IRWXU,
#include <sys/syslimits.h>
#include <unistd.h>  //ftruncate, close

#include <string>
#include <boost/interprocess/sync/named_recursive_mutex.hpp>

namespace memory_mgr
{
namespace osapi
{
typedef pthread_mutex_t critical_section;

typedef int file_handle_t;
typedef file_handle_t mapping_handle_t;
typedef ::mode_t mode_t;
static const mapping_handle_t invalid_mapping_handle = -1;
static void* invalid_mapping_address = MAP_FAILED;

static inline void initialize_critical_section(critical_section* cs)
{
  pthread_mutexattr_t mutexattr;  // Mutex attribute variable
  // Set the mutex as a recursive mutex
  pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init(cs, &mutexattr);
  // After initializing the mutex, the thread attribute can be destroyed
  pthread_mutexattr_destroy(&mutexattr);
}

static inline void delete_critical_section(critical_section* cs) { pthread_mutex_destroy(cs); }

static inline void enter_critical_section(critical_section* cs) { pthread_mutex_lock(cs); }

static inline void leave_critical_section(critical_section* cs) { pthread_mutex_unlock(cs); }

static inline int close_handle(file_handle_t handle) { return ::close(handle); }

static inline mapping_handle_t create_file_mapping(const std::string& name, int open_flag, mode_t access_mode, size_t /*size*/)
{
  return shm_open(name.c_str(), open_flag, access_mode);
}

static inline int resize_file_mapping(mapping_handle_t mapping, size_t size) { return ftruncate(mapping, size); }

static inline void* map_view_of_file(mapping_handle_t handle, int file_protect, std::size_t numbytes, int flags = MAP_SHARED, off_t offset = 0, void* base_addr = 0)
{
  return mmap(base_addr, numbytes, file_protect, flags, handle, offset);
}

static inline int unmap_view_of_file(void* address, size_t size) { return munmap(address, size); }

static inline int close_file_mapping(const std::string& name, mapping_handle_t mapping)
{
  shm_unlink(name.c_str());
  return osapi::close_handle(mapping);
}


using mutex_handle_t = pthread_mutex_t*;

static inline mutex_handle_t create_mutex(const std::string& name )
{
  throw std::runtime_error("Named mutex is not implemented yet ");
  return nullptr;
}

static inline mutex_handle_t open_mutex(const std::string& name, ulong access )
{
  throw std::runtime_error("Named mutex is not implemented yet ");
  return nullptr;
}

static inline bool release_mutex(mutex_handle_t handle)
{
  throw std::runtime_error("Named mutex is not implemented yet ");
}

enum lock_status{ lock_aquired, lock_abandoned, lock_timeout, lock_failed };


static inline lock_status lock_mutex( mutex_handle_t mutex )
{
  throw std::runtime_error("Named mutex is not implemented yet ");
  return lock_failed;
}

static inline bool unlock_mutex( mutex_handle_t mutex )
{
  throw std::runtime_error("Named mutex is not implemented yet ");
  return false;
}

static inline std::string get_executable_path()
{
  int ret;
  pid_t pid;
  char pathbuf[PROC_PIDPATHINFO_MAXSIZE];

  pid = getpid();
  ret = proc_pidpath(pid, pathbuf, sizeof(pathbuf));
  if (ret <= 0)
  {
    return std::string();
  }
  else
  {
    // printf("proc %d: %s\n", pid, pathbuf);
    return std::string(pathbuf);
  }
}

}  // namespace osapi
}  // namespace memory_mgr

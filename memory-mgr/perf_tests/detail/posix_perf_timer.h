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

#ifndef MGR_POSIX_PERF_TIMER_HEADER
#define MGR_POSIX_PERF_TIMER_HEADER

#include <memory-mgr/config/config.h>
#include <time.h>

#ifdef MGR_LINUX_PLATFORM

namespace memory_mgr
{
	namespace detail
	{
		static inline timespec operator-( const timespec& lhs, const timespec& rhs )
		{
			timespec t;
			t.tv_sec = lhs.tv_sec - rhs.tv_sec;
			t.tv_nsec = lhs.tv_nsec - rhs.tv_nsec;
			return t;
		}
		
		static inline timespec& operator+=( timespec& lhs, const timespec& rhs )
		{			
			lhs.tv_sec += rhs.tv_sec;
			lhs.tv_nsec += rhs.tv_nsec;
			return lhs;
		}

		class posix_perf_timer
		{
			enum {timer_id = CLOCK_THREAD_CPUTIME_ID};
		public:
			posix_perf_timer(bool start_now = false) 
				:m_is_running( false )
			{ 
				make_null(m_start);
				make_null(m_elapced);
				if(start_now)
				{
					start();
				}
			}

			posix_perf_timer(const posix_perf_timer& src)
			{
				copy(src);
			}

			virtual ~posix_perf_timer() 
			{}

			// start from current value or optionally from 0
			void start(bool reset = false)
			{
				if (reset)
				{
					make_null( m_elapced );
				}
				clock_gettime(timer_id, &m_start );
				m_is_running = true;
			}
			// stop timing. Use start afterwards to continue.
			void stop()
			{
				m_is_running = false;
				if ( !is_null(m_start) )
				{
					timespec end_time = {0, 0};
					clock_gettime(timer_id, &end_time );
					m_elapced += end_time - m_start;
				}
			}

			// Returns false if stopped.
			bool is_running()
			{return m_is_running;}

			// Returns false if performance counter not supported.
			// Call after constructing at least one win_perf_timer
			bool is_supported();


			// Returns timer resolution in seconds
			const long double resolution()
			{
				timespec resol = {0};
				clock_getres(timer_id, &resol );
				return resol.tv_sec + 
					static_cast<long double>(resol.tv_nsec)/1000000000.0L;
			}

			// Returns timer resolution in milliseconds
			const long double resolution_ms()
			{
				timespec resol = {0};
				clock_getres(timer_id, &resol );
				return resol.tv_sec * 1000 + 
						static_cast<long double>(resol.tv_nsec)/1000000.0;
			}

			// Returns timer resolution in microseconds
			const long double resolution_mcs()
			{
				timespec resol = {0};
				clock_getres(timer_id, &resol );
				return resol.tv_sec * 1000000 + 
					static_cast<long double>(resol.tv_nsec)/1000.0;
			}

			// Returns elapsed time in seconds
			const long double elapsed();
			// Returns elapsed time in milliseconds 
			const long double elapsed_ms()
			{
				posix_perf_timer result(*this);
				result.stop();
				return result.m_elapced.tv_sec * 1000 + 
					static_cast<long double>(result.m_elapced.tv_nsec)/1000000.0;
			}
			// Returns elapsed time in microseconds
			const long double elapsed_mcs()
			{
				posix_perf_timer result(*this);
				result.stop();
				return result.m_elapced.tv_sec * 1000000 + 
					static_cast<long double>(result.m_elapced.tv_nsec)/1000.0;
			}

			// Override for thread safe operation
			virtual void lock() const 
			{}

			virtual void unlock() const 
			{}
		private:
			void copy(const posix_perf_timer& src)
			{
				if (&src == this) 
				{// avoid deadlock if someone tries to copy it to itself
					return;
				}
				src.lock();
				lock();
				m_start = src.m_start; 
				m_elapced = src.m_elapced; 
				unlock();
				src.unlock();
			}

			static inline bool is_null( const timespec& t )
			{
				return (t.tv_sec == 0) && (t.tv_nsec == 0);
			}

			static inline void make_null( timespec& t )
			{
				t.tv_sec = 0;
				t.tv_nsec = 0;
			}

			

			timespec m_start;
			timespec m_elapced;
			bool m_is_running;
		};
	}
}

#endif//MGR_LINUX_PLATFORM

#endif//MGR_POSIX_PERF_TIMER_HEADER


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

// win_perf_timer - a simple Win32 performance counter wrapper
// by Dean Wyant dwyant@mindspring.com

/*

This class is simple to use. Just declare a variable(s) as type win_perf_timer,
call start() to start timimg and call stop() to stop timimg. You can pause a
timer by calling stop() and then you can call start() to resume. Retrieve the
elapsed time by calling an elapsed..() function. Assignment, addition, 
subtraction and comparison are supported. There are a few information calls
available also. All calls except start and stop can be performed on a timer
without stopping it.

*/

#pragma once

#include "memory-mgr/config/config.h"

#ifdef MGR_WINDOWS_PLATFORM


namespace memory_mgr
{
	namespace detail
	{
		class win_perf_timer
		{
		public:
			win_perf_timer(bool start_now = false) 
			{ init(start_now); }

			win_perf_timer(const win_perf_timer& src); 

			virtual ~win_perf_timer() 
			{}

			// start from current value or optionally from 0
			void start(bool reset = false);
			// stop timing. Use start afterwards to continue.
			void stop();

			// Returns false if stopped.
			bool is_running();

			// Returns false if performance counter not supported.
			// Call after constructing at least one win_perf_timer
			bool is_supported();
			

			// Returns timer resolution in seconds
			const double resolution();
			// Returns timer resolution in milliseconds
			const double resolution_ms();
			// Returns timer resolution in microseconds
			const double resolution_mcs();

			// Returns elapsed time in seconds
			const double elapsed();
			// Returns elapsed time in milliseconds 
			const double elapsed_ms();
			// Returns elapsed time in microseconds
			const double elapsed_mcs();

			// Override for thread safe operation
			virtual void lock() const 
			{}

			virtual void unlock() const 
			{}
		protected:
			void init(bool start_now);
			void copy(const win_perf_timer& src);

		private:
			__int64 m_start;
			// does not change while system is running
			static __int64 m_freq;   
			// Adjustment time it takes to start and stop
			static __int64 m_adjust; 
		};

		inline void win_perf_timer::init(bool start_now)
		{
			if (!m_freq) 
			{ // Initialization should only run once
				QueryPerformanceFrequency((LARGE_INTEGER *)&m_freq); 
				if (!m_freq)
				{
					// Timer will be useless but will not cause divide by zero
					m_freq = 1;
				}

				m_start = 0; 
				m_adjust = 0; 
				start();            // Time a stop
				stop(); 
				m_adjust = m_start;
			}

			// This is the only part that normally runs
			m_start = 0; 
			if (start_now)
			{
				start(); 
			}
		}

		inline win_perf_timer::win_perf_timer(const win_perf_timer& src)  
		{
			copy(src);
		}

		inline void win_perf_timer::copy(const win_perf_timer& src)
		{
			if (&src == this) 
			{// avoid deadlock if someone tries to copy it to itself
				return;
			}
			src.lock();
			lock();
			m_start = src.m_start; 
			unlock();
			src.unlock();
		}

		inline void win_perf_timer::start(bool reset) 
		{ // start from current value or optionally from 0
			__int64 i;
			QueryPerformanceCounter((LARGE_INTEGER *)&i);
			lock();
			if ((!reset) && (m_start < 0))
			{
				//We are starting with an accumulated time
				m_start += i;
			}
			else
			{// Starting from 0
				m_start = i;
			}
			unlock();
		} 

		inline void win_perf_timer::stop() 
		{ // stop timing. Use start afterwards to continue
			lock();
			if (m_start <= 0)
			{
				unlock();
				return;          // Was not running
			}
			__int64 i;
			QueryPerformanceCounter((LARGE_INTEGER *)&i);
			// Stopped timer keeps elapsed timer ticks as a negative 
			m_start += -i;
			// Do not overflow
			if (m_start < m_adjust)
			{
				// Adjust for time timer code takes to run
				m_start -= m_adjust;
			}
			else 
			{
				// stop must have been called directly after start
				m_start = 0;
			}
			unlock();
		} 

		inline bool win_perf_timer::is_running() 
		{ // Returns false if stopped.
			lock();
			// When < 0, holds elapsed clicks
			bool ret = (m_start > 0); 
			unlock();
			return ret;
		}
		inline const double win_perf_timer::elapsed()
		{ // Returns elapsed time in seconds
			win_perf_timer result(*this);
			result.stop();
			return (double)(-result.m_start)/(double)m_freq; 
		}

		inline const double win_perf_timer::elapsed_ms() 
		{ // Returns elapsed time in milliseconds
			win_perf_timer result(*this);
			result.stop();
			return (-result.m_start*1000.0)/(double)m_freq; 
		}

		inline const double win_perf_timer::elapsed_mcs() 
		{ // Returns elapsed time in microseconds
			win_perf_timer result(*this);
			result.stop();
			return (-result.m_start * 1000000.0)/(double)m_freq; 
		}
	}
}

#endif //MGR_WINDOWS_PLATFORM

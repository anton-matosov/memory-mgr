/* 
Generic Memory Manager (memory-mgr)
Copyright (c) 2007 Anton Matosov

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


// CPerfTimer - a simple Win32 performance counter wrapper
// by Dean Wyant dwyant@mindspring.com

#include "config/config.h"
#include "perf_timer.h"

// Declare and initialize static member vars that get set only once and never change
__int64 CPerfTimer::m_Freq = 0; 
__int64 CPerfTimer::m_Adjust = 0; 

// All functions defined inline for speed. After all, the performance counter is 
// supposed to be able to time very short events fairly accurately.



BOOL CPerfTimer::IsSupported()
{ // Returns FALSE if performance counter not supported.
  // Call after constructing at least one CPerfTimer
  return (m_Freq > 1);
}

const double CPerfTimer::Resolution()   
{ // Returns timer resolution in seconds
  return 1.0/(double)m_Freq; 
}

const double CPerfTimer::Resolutionms() 
{ // Returns timer resolution in milliseconds
  return 1000.0/(double)m_Freq; 
}

const double CPerfTimer::Resolutionus() 
{ // Returns timer resolution in microseconds
  return 1000000.0/(double)m_Freq; 
}




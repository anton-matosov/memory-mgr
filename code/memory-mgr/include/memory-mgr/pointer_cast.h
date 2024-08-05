//////////////////////////////////////////////////////////////////////////////
//
//  This file is the adaptation for Generic Memory Manager library
//  
// (C) Copyright Ion Gaztanaga 2005. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

namespace memory_mgr
{ 

	//static_pointer_cast overload for raw pointers
	template<class T, class U>
	inline T* static_pointer_cast(U *ptr)
	{  
		return static_cast<T*>(ptr);
	}

	//dynamic_pointer_cast overload for raw pointers
	template<class T, class U>
	inline T* dynamic_pointer_cast(U *ptr)
	{  
		return dynamic_cast<T*>(ptr);
	}

	//const_pointer_cast overload for raw pointers
	template<class T, class U>
	inline T* const_pointer_cast(U *ptr)
	{  
		return const_cast<T*>(ptr);
	}

	//reinterpret_pointer_cast overload for raw pointers
	template<class T, class U>
	inline T* reinterpret_pointer_cast(U *ptr)
	{  
		return reinterpret_cast<T*>(ptr);
	}

} // namespace memory_mgr

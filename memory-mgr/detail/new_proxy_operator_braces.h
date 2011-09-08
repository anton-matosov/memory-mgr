

object_pointer_type operator()()
{
	lock_type lock( m_alloc->get_lockable() );
	allocate();
	object_pointer_type object = m_object;

	if( m_alloc->construction_needed() )
	{
		for( size_t i = 0; i < m_num_items; ++i )
		{
			::new( object + i ) object_type();
		}
	}				

	return m_object;
}

template<  typename A0 >
object_pointer_type operator()( A0 a0)
{
	lock_type lock( m_alloc->get_lockable() );
	allocate();
	object_pointer_type object = m_object;

	if( m_alloc->construction_needed() )
	{
		for( size_t i = 0; i < m_num_items; ++i )
		{
			::new( object + i ) object_type( a0);
		}
	}				

	return m_object;
}

template<  typename A0 , typename A1 >
object_pointer_type operator()( A0 a0 , A1 a1)
{
	lock_type lock( m_alloc->get_lockable() );
	allocate();
	object_pointer_type object = m_object;

	if( m_alloc->construction_needed() )
	{
		for( size_t i = 0; i < m_num_items; ++i )
		{
			::new( object + i ) object_type( a0 , a1);
		}
	}				

	return m_object;
}

template<  typename A0 , typename A1 , typename A2 >
object_pointer_type operator()( A0 a0 , A1 a1 , A2 a2)
{
	lock_type lock( m_alloc->get_lockable() );
	allocate();
	object_pointer_type object = m_object;

	if( m_alloc->construction_needed() )
	{
		for( size_t i = 0; i < m_num_items; ++i )
		{
			::new( object + i ) object_type( a0 , a1 , a2);
		}
	}				

	return m_object;
}

template<  typename A0 , typename A1 , typename A2 , typename A3 >
object_pointer_type operator()( A0 a0 , A1 a1 , A2 a2 , A3 a3)
{
	allocate();
	object_pointer_type object = m_object;

	if( m_alloc->construction_needed() )
	{
		for( size_t i = 0; i < m_num_items; ++i )
		{
			::new( object + i ) object_type( a0 , a1 , a2 , a3);
		}
	}				

	return m_object;
}

template<  typename A0 , typename A1 , typename A2 , typename A3 , typename A4 >
object_pointer_type operator()( A0 a0 , A1 a1 , A2 a2 , A3 a3 , A4 a4)
{
	lock_type lock( m_alloc->get_lockable() );
	allocate();
	object_pointer_type object = m_object;

	if( m_alloc->construction_needed() )
	{
		for( size_t i = 0; i < m_num_items; ++i )
		{
			::new( object + i ) object_type( a0 , a1 , a2 , a3 , a4);
		}
	}				

	return m_object;
}

template<  typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 >
object_pointer_type operator()( A0 a0 , A1 a1 , A2 a2 , A3 a3 , A4 a4 , A5 a5)
{
	lock_type lock( m_alloc->get_lockable() );
	allocate();
	object_pointer_type object = m_object;

	if( m_alloc->construction_needed() )
	{
		for( size_t i = 0; i < m_num_items; ++i )
		{
			::new( object + i ) object_type( a0 , a1 , a2 , a3 , a4 , a5);
		}
	}				

	return m_object;
}

template<  typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 >
object_pointer_type operator()( A0 a0 , A1 a1 , A2 a2 , A3 a3 , A4 a4 , A5 a5 , A6 a6)
{
	lock_type lock( m_alloc->get_lockable() );
	allocate();
	object_pointer_type object = m_object;

	if( m_alloc->construction_needed() )
	{
		for( size_t i = 0; i < m_num_items; ++i )
		{
			::new( object + i ) object_type( a0 , a1 , a2 , a3 , a4 , a5 , a6);
		}
	}				

	return m_object;
}

template<  typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 >
object_pointer_type operator()( A0 a0 , A1 a1 , A2 a2 , A3 a3 , A4 a4 , A5 a5 , A6 a6 , A7 a7)
{
	lock_type lock( m_alloc->get_lockable() );
	allocate();
	object_pointer_type object = m_object;

	if( m_alloc->construction_needed() )
	{
		for( size_t i = 0; i < m_num_items; ++i )
		{
			::new( object + i ) object_type( a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7);
		}
	}				

	return m_object;
}

template<  typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 >
object_pointer_type operator()( A0 a0 , A1 a1 , A2 a2 , A3 a3 , A4 a4 , A5 a5 , A6 a6 , A7 a7 , A8 a8)
{
	lock_type lock( m_alloc->get_lockable() );
	allocate();
	object_pointer_type object = m_object;

	if( m_alloc->construction_needed() )
	{
		for( size_t i = 0; i < m_num_items; ++i )
		{
			::new( object + i ) object_type( a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8);
		}
	}				

	return m_object;
}

template<  typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 >
object_pointer_type operator()( A0 a0 , A1 a1 , A2 a2 , A3 a3 , A4 a4 , A5 a5 , A6 a6 , A7 a7 , A8 a8 , A9 a9)
{
	lock_type lock( m_alloc->get_lockable() );
	allocate();
	object_pointer_type object = m_object;

	if( m_alloc->construction_needed() )
	{
		for( size_t i = 0; i < m_num_items; ++i )
		{
			::new( object + i ) object_type( a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9);
		}
	}				

	return m_object;
}

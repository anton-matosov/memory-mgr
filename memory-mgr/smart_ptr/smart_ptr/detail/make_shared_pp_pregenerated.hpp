



template< typename T, typename Alloc  >
memory_mgr::shared_ptr< T > allocate_shared( const Alloc & alloc   )
{
	memory_mgr::shared_ptr< T > pt( static_cast< T* >( 0 ), memory_mgr::detail::sp_ms_deleter< T >(), alloc );

	memory_mgr::detail::sp_ms_deleter< T > * pd = memory_mgr::get_deleter< memory_mgr::detail::sp_ms_deleter< T > >( pt );

	void * pv = pd->address();

	::new( pv ) T();
	pd->set_initialized();

	T * pt2 = static_cast< T* >( pv );

	memory_mgr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
	return memory_mgr::shared_ptr< T >( pt, pt2 );
}

template< typename T, class MemMgr >
memory_mgr::shared_ptr< T > make_shared(MemMgr& mgr )
{							
	member_allocator<T, MemMgr> alloc( &mgr );
	return allocate_shared<T>( alloc  );
}

template< typename T, class MemMgr >
memory_mgr::shared_ptr< T > make_shared()
{																	
	return allocate_shared<T>( allocator<T, MemMgr>()  );
}

template< typename T, typename Alloc , typename A0 >
memory_mgr::shared_ptr< T > allocate_shared( const Alloc & alloc ,  const A0 &a0 )
{
	memory_mgr::shared_ptr< T > pt( static_cast< T* >( 0 ), memory_mgr::detail::sp_ms_deleter< T >(), alloc );

	memory_mgr::detail::sp_ms_deleter< T > * pd = memory_mgr::get_deleter< memory_mgr::detail::sp_ms_deleter< T > >( pt );

	void * pv = pd->address();

	::new( pv ) T( a0);
	pd->set_initialized();

	T * pt2 = static_cast< T* >( pv );

	memory_mgr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
	return memory_mgr::shared_ptr< T >( pt, pt2 );
}

template< typename T, class MemMgr , typename A0>
memory_mgr::shared_ptr< T > make_shared(MemMgr& mgr , const A0 &a0)
{							
	member_allocator<T, MemMgr> alloc( &mgr );
	return allocate_shared<T>( alloc , a0 );
}

template< typename T, class MemMgr , typename A0>
memory_mgr::shared_ptr< T > make_shared( const A0 &a0)
{																	
	return allocate_shared<T>( allocator<T, MemMgr>() , a0 );
}

template< typename T, typename Alloc , typename A0 , typename A1 >
memory_mgr::shared_ptr< T > allocate_shared( const Alloc & alloc ,  const A0 &a0 , const A1 &a1 )
{
	memory_mgr::shared_ptr< T > pt( static_cast< T* >( 0 ), memory_mgr::detail::sp_ms_deleter< T >(), alloc );

	memory_mgr::detail::sp_ms_deleter< T > * pd = memory_mgr::get_deleter< memory_mgr::detail::sp_ms_deleter< T > >( pt );

	void * pv = pd->address();

	::new( pv ) T( a0 , a1);
	pd->set_initialized();

	T * pt2 = static_cast< T* >( pv );

	memory_mgr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
	return memory_mgr::shared_ptr< T >( pt, pt2 );
}

template< typename T, class MemMgr , typename A0 , typename A1>
memory_mgr::shared_ptr< T > make_shared(MemMgr& mgr , const A0 &a0 , const A1 &a1)
{							
	member_allocator<T, MemMgr> alloc( &mgr );
	return allocate_shared<T>( alloc , a0 , a1 );
}

template< typename T, class MemMgr , typename A0 , typename A1>
memory_mgr::shared_ptr< T > make_shared( const A0 &a0 , const A1 &a1)
{																	
	return allocate_shared<T>( allocator<T, MemMgr>() , a0 , a1 );
}

template< typename T, typename Alloc , typename A0 , typename A1 , typename A2 >
memory_mgr::shared_ptr< T > allocate_shared( const Alloc & alloc ,  const A0 &a0 , const A1 &a1 , const A2 &a2 )
{
	memory_mgr::shared_ptr< T > pt( static_cast< T* >( 0 ), memory_mgr::detail::sp_ms_deleter< T >(), alloc );

	memory_mgr::detail::sp_ms_deleter< T > * pd = memory_mgr::get_deleter< memory_mgr::detail::sp_ms_deleter< T > >( pt );

	void * pv = pd->address();

	::new( pv ) T( a0 , a1 , a2);
	pd->set_initialized();

	T * pt2 = static_cast< T* >( pv );

	memory_mgr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
	return memory_mgr::shared_ptr< T >( pt, pt2 );
}

template< typename T, class MemMgr , typename A0 , typename A1 , typename A2>
memory_mgr::shared_ptr< T > make_shared(MemMgr& mgr , const A0 &a0 , const A1 &a1 , const A2 &a2)
{							
	member_allocator<T, MemMgr> alloc( &mgr );
	return allocate_shared<T>( alloc , a0 , a1 , a2 );
}

template< typename T, class MemMgr , typename A0 , typename A1 , typename A2>
memory_mgr::shared_ptr< T > make_shared( const A0 &a0 , const A1 &a1 , const A2 &a2)
{																	
	return allocate_shared<T>( allocator<T, MemMgr>() , a0 , a1 , a2 );
}

template< typename T, typename Alloc , typename A0 , typename A1 , typename A2 , typename A3 >
memory_mgr::shared_ptr< T > allocate_shared( const Alloc & alloc ,  const A0 &a0 , const A1 &a1 , const A2 &a2 , const A3 &a3 )
{
	memory_mgr::shared_ptr< T > pt( static_cast< T* >( 0 ), memory_mgr::detail::sp_ms_deleter< T >(), alloc );

	memory_mgr::detail::sp_ms_deleter< T > * pd = memory_mgr::get_deleter< memory_mgr::detail::sp_ms_deleter< T > >( pt );

	void * pv = pd->address();

	::new( pv ) T( a0 , a1 , a2 , a3);
	pd->set_initialized();

	T * pt2 = static_cast< T* >( pv );

	memory_mgr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
	return memory_mgr::shared_ptr< T >( pt, pt2 );
}

template< typename T, class MemMgr , typename A0 , typename A1 , typename A2 , typename A3>
memory_mgr::shared_ptr< T > make_shared(MemMgr& mgr , const A0 &a0 , const A1 &a1 , const A2 &a2 , const A3 &a3)
{							
	member_allocator<T, MemMgr> alloc( &mgr );
	return allocate_shared<T>( alloc , a0 , a1 , a2 , a3 );
}

template< typename T, class MemMgr , typename A0 , typename A1 , typename A2 , typename A3>
memory_mgr::shared_ptr< T > make_shared( const A0 &a0 , const A1 &a1 , const A2 &a2 , const A3 &a3)
{																	
	return allocate_shared<T>( allocator<T, MemMgr>() , a0 , a1 , a2 , a3 );
}

template< typename T, typename Alloc , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 >
memory_mgr::shared_ptr< T > allocate_shared( const Alloc & alloc ,  const A0 &a0 , const A1 &a1 , const A2 &a2 , const A3 &a3 , const A4 &a4 )
{
	memory_mgr::shared_ptr< T > pt( static_cast< T* >( 0 ), memory_mgr::detail::sp_ms_deleter< T >(), alloc );

	memory_mgr::detail::sp_ms_deleter< T > * pd = memory_mgr::get_deleter< memory_mgr::detail::sp_ms_deleter< T > >( pt );

	void * pv = pd->address();

	::new( pv ) T( a0 , a1 , a2 , a3 , a4);
	pd->set_initialized();

	T * pt2 = static_cast< T* >( pv );

	memory_mgr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
	return memory_mgr::shared_ptr< T >( pt, pt2 );
}

template< typename T, class MemMgr , typename A0 , typename A1 , typename A2 , typename A3 , typename A4>
memory_mgr::shared_ptr< T > make_shared(MemMgr& mgr , const A0 &a0 , const A1 &a1 , const A2 &a2 , const A3 &a3 , const A4 &a4)
{							
	member_allocator<T, MemMgr> alloc( &mgr );
	return allocate_shared<T>( alloc , a0 , a1 , a2 , a3 , a4 );
}

template< typename T, class MemMgr , typename A0 , typename A1 , typename A2 , typename A3 , typename A4>
memory_mgr::shared_ptr< T > make_shared( const A0 &a0 , const A1 &a1 , const A2 &a2 , const A3 &a3 , const A4 &a4)
{																	
	return allocate_shared<T>( allocator<T, MemMgr>() , a0 , a1 , a2 , a3 , a4 );
}

template< typename T, typename Alloc , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 >
memory_mgr::shared_ptr< T > allocate_shared( const Alloc & alloc ,  const A0 &a0 , const A1 &a1 , const A2 &a2 , const A3 &a3 , const A4 &a4 , const A5 &a5 )
{
	memory_mgr::shared_ptr< T > pt( static_cast< T* >( 0 ), memory_mgr::detail::sp_ms_deleter< T >(), alloc );

	memory_mgr::detail::sp_ms_deleter< T > * pd = memory_mgr::get_deleter< memory_mgr::detail::sp_ms_deleter< T > >( pt );

	void * pv = pd->address();

	::new( pv ) T( a0 , a1 , a2 , a3 , a4 , a5);
	pd->set_initialized();

	T * pt2 = static_cast< T* >( pv );

	memory_mgr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
	return memory_mgr::shared_ptr< T >( pt, pt2 );
}

template< typename T, class MemMgr , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5>
memory_mgr::shared_ptr< T > make_shared(MemMgr& mgr , const A0 &a0 , const A1 &a1 , const A2 &a2 , const A3 &a3 , const A4 &a4 , const A5 &a5)
{							
	member_allocator<T, MemMgr> alloc( &mgr );
	return allocate_shared<T>( alloc , a0 , a1 , a2 , a3 , a4 , a5 );
}

template< typename T, class MemMgr , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5>
memory_mgr::shared_ptr< T > make_shared( const A0 &a0 , const A1 &a1 , const A2 &a2 , const A3 &a3 , const A4 &a4 , const A5 &a5)
{																	
	return allocate_shared<T>( allocator<T, MemMgr>() , a0 , a1 , a2 , a3 , a4 , a5 );
}

template< typename T, typename Alloc , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 >
memory_mgr::shared_ptr< T > allocate_shared( const Alloc & alloc ,  const A0 &a0 , const A1 &a1 , const A2 &a2 , const A3 &a3 , const A4 &a4 , const A5 &a5 , const A6 &a6 )
{
	memory_mgr::shared_ptr< T > pt( static_cast< T* >( 0 ), memory_mgr::detail::sp_ms_deleter< T >(), alloc );

	memory_mgr::detail::sp_ms_deleter< T > * pd = memory_mgr::get_deleter< memory_mgr::detail::sp_ms_deleter< T > >( pt );

	void * pv = pd->address();

	::new( pv ) T( a0 , a1 , a2 , a3 , a4 , a5 , a6);
	pd->set_initialized();

	T * pt2 = static_cast< T* >( pv );

	memory_mgr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
	return memory_mgr::shared_ptr< T >( pt, pt2 );
}

template< typename T, class MemMgr , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6>
memory_mgr::shared_ptr< T > make_shared(MemMgr& mgr , const A0 &a0 , const A1 &a1 , const A2 &a2 , const A3 &a3 , const A4 &a4 , const A5 &a5 , const A6 &a6)
{							
	member_allocator<T, MemMgr> alloc( &mgr );
	return allocate_shared<T>( alloc , a0 , a1 , a2 , a3 , a4 , a5 , a6 );
}

template< typename T, class MemMgr , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6>
memory_mgr::shared_ptr< T > make_shared( const A0 &a0 , const A1 &a1 , const A2 &a2 , const A3 &a3 , const A4 &a4 , const A5 &a5 , const A6 &a6)
{																	
	return allocate_shared<T>( allocator<T, MemMgr>() , a0 , a1 , a2 , a3 , a4 , a5 , a6 );
}

template< typename T, typename Alloc , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 >
memory_mgr::shared_ptr< T > allocate_shared( const Alloc & alloc ,  const A0 &a0 , const A1 &a1 , const A2 &a2 , const A3 &a3 , const A4 &a4 , const A5 &a5 , const A6 &a6 , const A7 &a7 )
{
	memory_mgr::shared_ptr< T > pt( static_cast< T* >( 0 ), memory_mgr::detail::sp_ms_deleter< T >(), alloc );

	memory_mgr::detail::sp_ms_deleter< T > * pd = memory_mgr::get_deleter< memory_mgr::detail::sp_ms_deleter< T > >( pt );

	void * pv = pd->address();

	::new( pv ) T( a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7);
	pd->set_initialized();

	T * pt2 = static_cast< T* >( pv );

	memory_mgr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
	return memory_mgr::shared_ptr< T >( pt, pt2 );
}

template< typename T, class MemMgr , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7>
memory_mgr::shared_ptr< T > make_shared(MemMgr& mgr , const A0 &a0 , const A1 &a1 , const A2 &a2 , const A3 &a3 , const A4 &a4 , const A5 &a5 , const A6 &a6 , const A7 &a7)
{							
	member_allocator<T, MemMgr> alloc( &mgr );
	return allocate_shared<T>( alloc , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 );
}

template< typename T, class MemMgr , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7>
memory_mgr::shared_ptr< T > make_shared( const A0 &a0 , const A1 &a1 , const A2 &a2 , const A3 &a3 , const A4 &a4 , const A5 &a5 , const A6 &a6 , const A7 &a7)
{																	
	return allocate_shared<T>( allocator<T, MemMgr>() , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 );
}

template< typename T, typename Alloc , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 >
memory_mgr::shared_ptr< T > allocate_shared( const Alloc & alloc ,  const A0 &a0 , const A1 &a1 , const A2 &a2 , const A3 &a3 , const A4 &a4 , const A5 &a5 , const A6 &a6 , const A7 &a7 , const A8 &a8 )
{
	memory_mgr::shared_ptr< T > pt( static_cast< T* >( 0 ), memory_mgr::detail::sp_ms_deleter< T >(), alloc );

	memory_mgr::detail::sp_ms_deleter< T > * pd = memory_mgr::get_deleter< memory_mgr::detail::sp_ms_deleter< T > >( pt );

	void * pv = pd->address();

	::new( pv ) T( a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8);
	pd->set_initialized();

	T * pt2 = static_cast< T* >( pv );

	memory_mgr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
	return memory_mgr::shared_ptr< T >( pt, pt2 );
}

template< typename T, class MemMgr , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8>
memory_mgr::shared_ptr< T > make_shared(MemMgr& mgr , const A0 &a0 , const A1 &a1 , const A2 &a2 , const A3 &a3 , const A4 &a4 , const A5 &a5 , const A6 &a6 , const A7 &a7 , const A8 &a8)
{							
	member_allocator<T, MemMgr> alloc( &mgr );
	return allocate_shared<T>( alloc , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 );
}

template< typename T, class MemMgr , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8>
memory_mgr::shared_ptr< T > make_shared( const A0 &a0 , const A1 &a1 , const A2 &a2 , const A3 &a3 , const A4 &a4 , const A5 &a5 , const A6 &a6 , const A7 &a7 , const A8 &a8)
{																	
	return allocate_shared<T>( allocator<T, MemMgr>() , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 );
}

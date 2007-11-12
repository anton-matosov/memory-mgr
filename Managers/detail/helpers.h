
namespace managers
{
	namespace detail 
	{
		namespace helpers 
		{
			namespace detail 
			{
				template <typename T>
				int integer_log2_impl(T x, int n) 
				{
					int result = 0;
					while (x != 1) 
					{
						const T t = static_cast< const T >( x >> n );
						if (t) {
							result += n;
							x = t;
						}
						n /= 2;
					}
					return result;
				}

				// helper to find the maximum power of two
				// less than p (more involved than necessary,
				// to avoid PTS)
				//
				template <int p, int n>
				struct max_pow2_less 
				{
					enum { c = 2*n < p };
					enum { value = c ? (max_pow2_less< c*p, 2*c*n>::value) : n};
				};

				template <>
				struct max_pow2_less<0, 0> 
				{
					enum { value = 0};
				};


			} //helpers detail

			// ---------
			// integer_log2
			// ---------------
			//
			template <typename T>
			int integer_log2(T x) 
			{
				assert(x > 0); // PRE

				const int n = detail::max_pow2_less<std::numeric_limits<T>::digits, 4>::value;

				return detail::integer_log2_impl(x, n);

			}

			template <typename T>
			int lowest_bit(T x) 
			{
				assert(x >= 1); // PRE

				// clear all bits on except the rightmost one,
				// then calculate the logarithm base 2
				return integer_log2<T>( x - ( x & (x-1) ) );

			}

		}//helpers

	}//detail

}//managers
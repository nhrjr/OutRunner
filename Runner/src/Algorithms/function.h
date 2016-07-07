#pragma once

/*The MIT License (MIT)

Copyright (c) 2012-2015 ApEk, NoAvailableAlias, Nano-signal-slot Contributors

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/
#include <cstdint>
#include <array>

namespace Nano
{

	using DelegateKey = std::array<std::uintptr_t, 2>;

	template <typename RT> class Function;
	template <typename RT, typename... Args>
	class Function<RT(Args...)>
	{
		using Thunk = RT(*)(void*, Args...);

		friend class Observer;

		void* m_this_ptr; // instance pointer
		Thunk m_stub_ptr; // free function pointer

		Function(void* this_ptr, Thunk stub_ptr) :
			m_this_ptr(this_ptr), m_stub_ptr(stub_ptr) {}

		Function(DelegateKey delegate_key) :
			m_this_ptr(reinterpret_cast<void*>(delegate_key[0])),
			m_stub_ptr(reinterpret_cast<Thunk>(delegate_key[1])) {}

	public:

		template <RT(*fun_ptr) (Args...)>
		static inline Function bind()
		{
			return{ nullptr, [](void* /*NULL*/, Args... args)
			{ return (*fun_ptr)(std::forward<Args>(args)...); } };
		}
		template <typename T, RT(T::* mem_ptr) (Args...)>
		static inline Function bind(T* pointer)
		{
			return{ pointer, [](void* this_ptr, Args... args)
			{ return (static_cast<T*>(this_ptr)->*mem_ptr) (std::forward<Args>(args)...); } };
		}
		template <typename T, RT(T::* mem_ptr) (Args...) const>
		static inline Function bind(T* pointer)
		{
			return{ pointer, [](void* this_ptr, Args... args)
			{ return (static_cast<T*>(this_ptr)->*mem_ptr) (std::forward<Args>(args)...); } };
		}
		template <typename L>
		static inline Function bind(L* pointer)
		{
			return{ pointer, [](void *this_ptr, Args... args)
			{ return (static_cast<L*>(this_ptr)->operator()(std::forward<Args>(args)...)); } };
		}
		inline operator DelegateKey() const
		{
			return
			{ {
					reinterpret_cast<std::uintptr_t>(m_this_ptr),
					reinterpret_cast<std::uintptr_t>(m_stub_ptr)
				} };
		}
		template <typename... Uref>
		inline RT operator() (Uref&&... args)
		{
			return (*m_stub_ptr)(m_this_ptr, std::forward<Uref>(args)...);
		}
	};

} // namespace Nano ------------------------------------------------------------

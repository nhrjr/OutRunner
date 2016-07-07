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

#include "function.h"
#include "observer.h"

namespace Nano
{

	template <typename RT> class Signal;
	template <typename RT, typename... Args>
	class Signal<RT(Args...)> : private Observer
	{
		template <typename T>
		void insert_sfinae(DelegateKey const& key, typename T::Observer* instance)
		{
			Observer::insert(key, instance);
			instance->insert(key, this);
		}
		template <typename T>
		void remove_sfinae(DelegateKey const& key, typename T::Observer* instance)
		{
			Observer::remove(key);
			instance->remove(key);
		}
		template <typename T>
		void insert_sfinae(DelegateKey const& key, ...)
		{
			Observer::insert(key);
		}
		template <typename T>
		void remove_sfinae(DelegateKey const& key, ...)
		{
			Observer::remove(key);
		}

	public:

		using Delegate = Function<RT(Args...)>;

		//-------------------------------------------------------------------CONNECT

		template <typename L>
		void connect(L* instance)
		{
			Observer::insert(Delegate::template bind(instance));
		}
		template <typename L>
		void connect(L& instance)
		{
			connect(std::addressof(instance));
		}

		template <RT(*fun_ptr)(Args...)>
		void connect()
		{
			Observer::insert(Delegate::template bind<fun_ptr>());
		}

		template <typename T, RT(T::* mem_ptr)(Args...)>
		void connect(T* instance)
		{
			insert_sfinae<T>(Delegate::template bind<T, mem_ptr>(instance), instance);
		}
		template <typename T, RT(T::* mem_ptr)(Args...) const>
		void connect(T* instance)
		{
			insert_sfinae<T>(Delegate::template bind<T, mem_ptr>(instance), instance);
		}

		template <typename T, RT(T::* mem_ptr)(Args...)>
		void connect(T& instance)
		{
			connect<T, mem_ptr>(std::addressof(instance));
		}
		template <typename T, RT(T::* mem_ptr)(Args...) const>
		void connect(T& instance)
		{
			connect<T, mem_ptr>(std::addressof(instance));
		}

		//----------------------------------------------------------------DISCONNECT

		template <typename L>
		void disconnect(L* instance)
		{
			Observer::remove(Delegate::template bind(instance));
		}
		template <typename L>
		void disconnect(L& instance)
		{
			disconnect(std::addressof(instance));
		}

		template <RT(*fun_ptr)(Args...)>
		void disconnect()
		{
			Observer::remove(Delegate::template bind<fun_ptr>());
		}

		template <typename T, RT(T::* mem_ptr)(Args...)>
		void disconnect(T* instance)
		{
			remove_sfinae<T>(Delegate::template bind<T, mem_ptr>(instance), instance);
		}
		template <typename T, RT(T::* mem_ptr)(Args...) const>
		void disconnect(T* instance)
		{
			remove_sfinae<T>(Delegate::template bind<T, mem_ptr>(instance), instance);
		}

		template <typename T, RT(T::* mem_ptr)(Args...)>
		void disconnect(T& instance)
		{
			disconnect<T, mem_ptr>(std::addressof(instance));
		}
		template <typename T, RT(T::* mem_ptr)(Args...) const>
		void disconnect(T& instance)
		{
			disconnect<T, mem_ptr>(std::addressof(instance));
		}

		//----------------------------------------------------EMIT / EMIT ACCUMULATE

#ifdef NANO_USE_DEPRECATED

		/// Will not benefit from perfect forwarding
		/// TODO [[deprecated]] when c++14 is comfortably supported

		void operator() (Args... args)
		{
			emit(std::forward<Args>(args)...);
		}
		template <typename Accumulate>
		void operator() (Args... args, Accumulate&& accumulate)
		{
			emit_accumulate<Accumulate>
				(std::forward<Accumulate>(accumulate), std::forward<Args>(args)...);
		}

#endif

		template <typename... Uref>
		void emit(Uref&&... args)
		{
			Observer::onEach<Delegate>(std::forward<Uref>(args)...);
		}

		template <typename Accumulate, typename... Uref>
		void emit_accumulate(Accumulate&& accumulate, Uref&&... args)
		{
			Observer::onEach_Accumulate<Delegate, Accumulate>
				(std::forward<Accumulate>(accumulate), std::forward<Uref>(args)...);
		}

		//-------------------------------------------------------------------UTILITY

		bool empty() const
		{
			return Observer::isEmpty();
		}

		void removeAll()
		{
			Observer::removeAll();
		}

	};

} // namespace Nano ------------------------------------------------------------


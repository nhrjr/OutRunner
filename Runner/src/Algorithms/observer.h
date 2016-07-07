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

namespace Nano
{

	class Observer
	{

		template <typename T> friend class Signal;

		struct DelegateKeyObserver { DelegateKey delegate; Observer* observer; };
		struct Node { DelegateKeyObserver data; Node* next; } *head = nullptr;

		//-----------------------------------------------------------PRIVATE METHODS

		void insert(DelegateKey const& key, Observer* ptr)
		{
			head = new Node{ { key, ptr }, head };
		}

		void insert(DelegateKey const& key)
		{
			this->insert(key, this);
		}

		void remove(DelegateKey const& key)
		{
			Node* node = head;
			Node* prev = nullptr;
			// Only delete the first occurrence
			for (; node; prev = node, node = node->next)
			{
				if (node->data.delegate == key)
				{
					if (prev)
					{
						prev->next = node->next;
					}
					else
					{
						head = head->next;
					}
					delete node;
					break;
				}
			}
		}

		void removeAll()
		{
			for (auto node = head; node;)
			{
				auto temp = node;
				// If this is us we only need to delete
				if (this != node->data.observer)
				{
					// Remove this slot from this listening Observer
					node->data.observer->remove(node->data.delegate);
				}
				node = node->next;
				delete temp;
			}
			head = nullptr;
		}

		bool isEmpty() const
		{
			return head == nullptr;
		}

		template <typename Delegate, typename... Uref>
		void onEach(Uref&&... args)
		{
			for (auto node = head, next = head; node; node = next)
			{
				next = node->next;
				// Perfect forward and emit
				Delegate(node->data.delegate)(std::forward<Uref>(args)...);
			}
		}

		template <typename Delegate, typename Accumulate, typename... Uref>
		void onEach_Accumulate(Accumulate&& accumulate, Uref&&... args)
		{
			for (auto node = head, next = head; node; node = next)
			{
				next = node->next;
				// Perfect forward, emit, and accumulate the return value
				accumulate(Delegate(node->data.delegate)(std::forward<Uref>(args)...));
			}
		}

		//-----------------------------------------------------------------PROTECTED

	protected:

		~Observer()
		{
			removeAll();
		}

		//--------------------------------------------------------------------PUBLIC

	public:

		Observer() = default;
		Observer(const Observer& other) = delete; // non construction-copyable
		Observer& operator=(const Observer&) = delete; // non copyable

	};

} // namespace Nano ------------------------------------------------------------

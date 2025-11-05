
#pragma once



#include <array>
#include <stdexcept>
#include <iterator>
#include <cstddef>
#include <utility>



template<typename /*POD*/T, std::size_t Capacity>
class RingQueue
{
	public:
		static constexpr std::size_t capacity_value = Capacity;
		
		RingQueue():Size(0),Head(0){}
		
		void clear() noexcept { Size = 0; Head = 0; }
		bool empty() const noexcept { return Size == 0; }
		bool full()  const noexcept { return Size == Capacity; }
		size_t size() const noexcept { return Size; }
		size_t capacity() const noexcept { return Capacity; }
		
		void push(const T& value)
		{
			if (full()) throw std::overflow_error("RingQueue full");
			Buffer[round_index(Head + Size)] = value;
			++Size;
		}
		void push(T&& value)
		{
			if (full()) throw std::overflow_error("RingQueue full");
			Buffer[round_index(Head + Size)] = std::move(value);
			++Size;
		}
		
		template<class... Args>
		void emplace(Args&&... args)
		{
			if (full()) throw std::overflow_error("RingQueue full");
			Buffer[round_index(Head + Size)] = T(std::forward<Args>(args)...);
			++Size;
		}
		
		void pop()
		{
			if (empty()) throw std::underflow_error("RingQueue empty");
			Head = round_index(Head + 1);
			--Size;
		}
		
		void erase(size_t index)
		{
			if (index >= Size) throw std::out_of_range("erase index out of range");
			
			if (index < (Size / 2)){
				for (size_t i = index; i > 0; --i){
					size_t from = round_index(Head + i - 1);
					size_t to   = round_index(Head + i);
					Buffer[to] = std::move(Buffer[from]);
				}
				Head = round_index(Head + 1);
			} else {
				for (size_t i = index; i < Size - 1; ++i){
					size_t from = round_index(Head + i + 1);
					size_t to   = round_index(Head + i);
					Buffer[to] = std::move(Buffer[from]);
				}
			}
			--Size;
		}
		
		T& front()
		{
			if (empty()) throw std::underflow_error("RingQueue empty");
			return Buffer[Head];
		}
		const T& front() const
		{
			if (empty()) throw std::underflow_error("RingQueue empty");
			return Buffer[Head];
		}
		
		T& back()
		{
			if (empty()) throw std::underflow_error("RingQueue empty");
			return Buffer[round_index(Head + Size - 1)];
		}
		const T& back() const
		{
			if (empty()) throw std::underflow_error("RingQueue empty");
			return Buffer[round_index(Head + Size - 1)];
		}
		
		T& operator[](size_t index)
		{
			if (index >= Size) throw std::out_of_range("index out of range");
			return Buffer[round_index(Head + index)];
		}
		const T& operator[](size_t index) const
		{
			if (index >= Size) throw std::out_of_range("index out of range");
			return Buffer[round_index(Head + index)];
		}
		
		
		
		class iterator
		{
			public:
				using iterator_category = std::random_access_iterator_tag;
				using difference_type   = std::ptrdiff_t;
				using value_type        = T;
				using pointer           = T*;
				using reference         = T&;
				
				iterator(RingQueue* q, size_t pos) : Owner(q), Pos(pos){}
				reference operator*() const { return (*Owner)[Pos]; }
				pointer operator->() const { return &(**this); }
				
				iterator& operator++(){ ++Pos; return *this; }
				iterator operator++(int){ auto tmp = *this; ++(*this); return tmp; }
				iterator& operator--(){ --Pos; return *this; }
				iterator operator--(int){ auto tmp = *this; --(*this); return tmp; }
				
				bool operator==(const iterator& other) const { return Owner == other.Owner && Pos == other.Pos; }
				bool operator!=(const iterator& other) const { return !(*this == other); }
				
				size_t index() const {return Pos; }
			
			private:
				RingQueue* Owner;
				size_t Pos;
		};
		
		class const_iterator
		{
			public:
				using iterator_category = std::random_access_iterator_tag;
				using difference_type   = std::ptrdiff_t;
				using value_type        = const T;
				using pointer           = const T*;
				using reference         = const T&;
				
				const_iterator(const RingQueue* q, size_t pos) : Owner(q), Pos(pos){}
				reference operator*() const { return (*Owner)[Pos]; }
				pointer operator->() const { return &(**this); }
				
				const_iterator& operator++(){ ++Pos; return *this; }
				const_iterator operator++(int){ auto tmp = *this; ++(*this); return tmp; }
				const_iterator& operator--(){ --Pos; return *this; }
				const_iterator operator--(int){ auto tmp = *this; --(*this); return tmp; }
				
				bool operator==(const const_iterator& other) const { return Owner == other.Owner && Pos == other.Pos; }
				bool operator!=(const const_iterator& other) const { return !(*this == other); }
				
				size_t index() const {return Pos; }
			
			private:
				const RingQueue* Owner;
				size_t Pos;
		};
		
		iterator begin() { return iterator(this, 0); }
		iterator end()   { return iterator(this, Size); }
		const_iterator begin() const { return const_iterator(this, 0); }
		const_iterator end()   const { return const_iterator(this, Size); }
		const_iterator cbegin() const { return const_iterator(this, 0); }
		const_iterator cend()   const { return const_iterator(this, Size); }
	
	private:
		size_t round_index(size_t index){ return index % Capacity; }
	
	private:
		std::array<T, Capacity> Buffer;
		size_t Size;
		size_t Head;
};

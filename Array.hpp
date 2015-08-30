#ifndef __SKHOLE_ARRAY__
#define __SKHOLE_ARRAY__

#include <iostream>
#include <assert.h>

namespace cs540 {

	class OutOfRange {

		public:
			OutOfRange() {

			}
	};

	template <typename T, std::size_t dim, std::size_t... dims>
	class Array {

		static_assert(dim != 0, "Dimension can not be zero");

		public:
			std::size_t size;

			Array<T, dims...>array[dim];

			static T ValueType;

			Array(){
				size = dim;
				for(int i = 0; i < size; i++){
					array[i] = Array<T, dims...>();
				}
			}

			Array( const Array& other) {
				size = other.size;
				for(int i = 0; i < other.size; i++) {
					array[i] = other.array[i];
				}
			}

			template <typename U>
			Array(const Array<U, dim, dims...>& other){
				size = other.size;
				for(int i = 0; i < size; i++){
					array[i] = other.array[i];
				}
			}

			Array& operator=(const Array& other) {
				if(this == &other){
					return *this;
				}
				if(size != other.size){
					throw OutOfRange();
				}
				for(int i = 0; i < size; i++){
					array[i] = other.array[i];
				}
				return *this;
			}

			template<typename U>
			Array& operator=(const Array<U, dim, dims...>& other){
				if(this == (Array<T, dim, dims...>*)&other){
					return *this;
				}
				if(size != other.size){
					throw OutOfRange();
				}
				for(int i = 0; i < size; i++){
					array[i] = other.array[i];
				}
				return *this;
			}

			Array<T, dims...>& operator[](std::size_t index) {
				if(index < 0 || index >= dim){
					throw OutOfRange();
				}
				return array[index];
			}

			const Array<T, dims...>& operator[](std::size_t index) const {
				if(index < 0 || index >= dim ){
					throw OutOfRange();
				}
				return array[index];
			}

			class FirstDimensionMajorIterator {

				private:
					size_t getSize(size_t arg) {
                                		dimensions[index] = arg;
                        	        	return arg;
                	        	}

	        	                template <typename... params>
		                        size_t getSize(size_t first, params... args){
                	        	        dimensions[index] = first;
                		                index++;
        	                	        return first * getSize(args...);
	                        	}

                                public:
                                        Array<T, dims...>* data;
                                        std::size_t pos;
					std::size_t dimensions[sizeof...(dims)];
					std::size_t index = 0;
					typename Array<T, dims...>::FirstDimensionMajorIterator it;

                                        FirstDimensionMajorIterator() {
						getSize(dims...);
                                                pos = 0;
                                        }

                                        FirstDimensionMajorIterator(Array<T, dims...>* p_data, std::size_t p_pos) {
						assert(p_data != nullptr);
                                                getSize(dims...);
						assert(p_data);
						pos = p_pos;
                                                data = p_data;
						it = data[pos].fmbegin();
                                        }

                                        FirstDimensionMajorIterator(const FirstDimensionMajorIterator& other) {
                                                data = other.data;
                                                pos = other.pos;
						it = other.it;
						for(int i = 0; i < sizeof...(dims); i++){
							dimensions[i] = other.dimensions[i];
						}
                                        }

                                        FirstDimensionMajorIterator &operator=(const FirstDimensionMajorIterator &other) {
                                                if(this == &other){
                                                        return *this;
                                                }
                                                data = other.data;
                                                pos = other.pos;
						it = other.it;
						for(int i = 0; i < sizeof...(dims); i++){
							dimensions[i] = other.dimensions[i];
						}
                                                return *this;
                                        }

					FirstDimensionMajorIterator &operator++() {
						++it;
						if(it.pos == dimensions[0]){
							++pos;
							it = data[pos].fmbegin();
						}
						return *this;
                                        }

                                        FirstDimensionMajorIterator operator++(int) {
						FirstDimensionMajorIterator temp = *this;
						++(*this);
                				return temp;
                                        }

                                        T &operator*() const {
						return *it;
                                        }

                                        friend bool operator==(const FirstDimensionMajorIterator &it1, const FirstDimensionMajorIterator &it2) {
                                                return (it1.pos == it2.pos && it1.data == it2.data && it1.it == it2.it);
                                        }

                                        friend bool operator!=(const FirstDimensionMajorIterator &it1, const FirstDimensionMajorIterator &it2) {
                                                return !(it1 == it2);
                                        }

                        };


			FirstDimensionMajorIterator fmbegin(std::size_t index) {
                                return FirstDimensionMajorIterator(array, index);
                        }

			FirstDimensionMajorIterator fmbegin() {
				return FirstDimensionMajorIterator(array, 0);
			}

			FirstDimensionMajorIterator fmend() {
				return FirstDimensionMajorIterator(array, dim);
			}

			class LastDimensionMajorIterator {

				public:
					Array<T, dim, dims...>* data;
                                        std::size_t pos;
                                        typename Array<T, dims...>::LastDimensionMajorIterator it;
					bool isEnd;

                                        LastDimensionMajorIterator() {
                                                pos = 0;
						isEnd = false;
                                        }

					LastDimensionMajorIterator(Array<T, dim, dims...>* p_data, std::size_t p_pos) {
                                                assert(p_data != nullptr);
                                                pos = p_pos;
                                                data = p_data;
						isEnd = false;
                                        }

                                        LastDimensionMajorIterator(const LastDimensionMajorIterator& other) {
                                                data = other.data;
						isEnd = other.isEnd;
                                                pos = other.pos;
                                                it = other.it;
                                        }

					LastDimensionMajorIterator &operator=(const LastDimensionMajorIterator &other) {
                                                if(this == &other){
                                                        return *this;
                                                }
						assert(&other != nullptr);
                                                data = other.data;
                                                pos = other.pos;
                                                it = other.it;
						isEnd = other.isEnd;
                                                return *this;
                                        }

					friend bool operator==(const LastDimensionMajorIterator& it1, const LastDimensionMajorIterator &it2) {
						if(it1.data != it2.data) {
							return false;
						}
						if(it1.pos != it2.pos) {
							return false;
						}
						if(it1.isEnd != it2.isEnd) {
							return false;
						}
						return it1.it == it2.it;
                                        }

                                        friend bool operator!=(const LastDimensionMajorIterator &it1, const LastDimensionMajorIterator &it2) {
						if(it1.data != it2.data) {
                                                        return true;
						}
                                                if(it1.pos != it2.pos) {
                                                        return true;
						}
                                                if(it1.isEnd != it2.isEnd) {
                                                        return true;
						}
                                                return it1.it != it2.it;
                                        }

					T &operator*() const {
						return *it;
					}

					void rearrange(){
            					it.data = &(data->array[pos]);
				        	it.rearrange();
						isEnd = it.isEnd;
        				}

					LastDimensionMajorIterator &operator++() {
						pos = (pos + 1) % dim;
						if(pos == 0){
							isEnd = true;
							++it;
						}
						rearrange();
						return *this;
					}

					LastDimensionMajorIterator operator++(int) {
						LastDimensionMajorIterator it = *this;
						++(*this);
						return it;
					}
			};


			LastDimensionMajorIterator lmbegin() {
				LastDimensionMajorIterator fmi;
			        fmi.data = this;
			        fmi.it = array[0].lmbegin();
			        return fmi;
			}

			LastDimensionMajorIterator lmend() {
				LastDimensionMajorIterator fmi;
        			fmi.data = this;
				fmi.isEnd = true;
        			fmi.it = array[0].lmend();
        			return fmi;
			}
	};

	template <typename T, std::size_t dim>
	class Array<T, dim> {

		static_assert( dim != 0, "Dimension should not be zero!!!");

		public:
			T array[dim];

			std::size_t size;

			static T ValueType;

			Array() {
				size = dim;
			}

			Array(const Array& other) {
				size = other.size;
				for(int i = 0; i < size; i++){
					array[i] = other.array[i];
				}
			}

			template<typename U>
			Array(const Array<U, dim>& other){
				size = other.size;
				for(int i = 0; i < size; i++){
					array[i] = other.array[i];
				}
			}

			Array& operator=(const Array& other) {
                                if(this == &other){
                                        return *this;
                                }
                                if(size != other.size){
                                        throw OutOfRange();
                                }
                                for(int i = 0; i < size; i++){
                                        array[i] = other.array[i];
                                }
				return *this;
                        }

			template<typename U>
                        Array& operator=(const Array<U, dim>& other){
                                if(this == (Array<T, dim>*)&other){
                                        return *this;
                                }
                                if(size != other.size){
                                        throw OutOfRange();
                                }
                                for(int i = 0; i < size; i++){
                                        array[i] = other.array[i];
                                }
				return *this;
                        }

			T& operator[](std::size_t index){
				if(index < 0 || index >= dim) {
					throw OutOfRange();
				}
				return array[index];
			}

			const T& operator[](std::size_t index) const {
				if(index < 0 || index >= dim){
					throw OutOfRange();
				}
				return array[index];
			}

			class FirstDimensionMajorIterator {

				public:
					T* data;
					std::size_t pos;

				public:
					FirstDimensionMajorIterator() {
						pos = 0;
					}

					FirstDimensionMajorIterator(T* p_data, std::size_t p_pos) {
						pos = p_pos;
						data = p_data;
					}

					FirstDimensionMajorIterator(const FirstDimensionMajorIterator& other) {
						data = other.data;
						pos = other.pos;
					}

					FirstDimensionMajorIterator &operator=(const FirstDimensionMajorIterator &other) {
						if(this == &other){
							return *this;
						}
						data = other.data;
						pos = other.pos;
						return *this;
					}

					FirstDimensionMajorIterator &operator++() {
						++pos;
						return *this;
					}

					FirstDimensionMajorIterator operator++(int) {
						FirstDimensionMajorIterator it = *this;
						++(*this);
						return it;
					}

					T &operator*() const {
						assert(data != nullptr);
						return data[pos];
					}

					friend bool operator==(const FirstDimensionMajorIterator &it1, const FirstDimensionMajorIterator &it2) {
						return (it1.pos == it2.pos && it1.data == it2.data);
					}

					friend bool operator!=(const FirstDimensionMajorIterator &it1, const FirstDimensionMajorIterator &it2) {
						return !(it1 == it2);
					}

			};

			class LastDimensionMajorIterator {

				public:
					Array<T, dim>* data;
					T* elem;
					std::size_t pos;
					bool isEnd;

				public:
					LastDimensionMajorIterator() {
						pos = 0;
						isEnd = false;
					}

					LastDimensionMajorIterator(Array<T, dim>* p_data, std::size_t p_pos) {
						data = &(*p_data);
						pos = p_pos;
						isEnd = false;
						elem = &(data->array[pos]);
					}

					LastDimensionMajorIterator(const LastDimensionMajorIterator &other) {
						pos = other.pos;
						data = other.data;
						elem = other.elem;
						isEnd = other.isEnd;
					}

					LastDimensionMajorIterator &operator=(const LastDimensionMajorIterator &other) {
						if(this == &other) {
							return *this;
						}
						pos = other.pos;
						data = other.data;
						elem = other.elem;
						isEnd = other.isEnd;
						return *this;
					}

					void rearrange() {
						elem = &(data->array[pos]);

					}

					LastDimensionMajorIterator &operator++() {
						pos = pos + 1;
            					if(pos == dim){
							pos = 0;
							isEnd = true;
            					} else {
							isEnd = false;
						}
            					elem = &(data->array[pos]);
						return *this;
					}

					LastDimensionMajorIterator operator++(int) {
						LastDimensionMajorIterator temp = *this;
						++(*this);
						return temp;
					}

					T &operator*() const {
						return *elem;
					}

					friend bool operator==(const LastDimensionMajorIterator &it1, const LastDimensionMajorIterator &it2) {
						if(it1.data != it2.data)
							return false;
						if(it1.pos != it2.pos)
							return false;
						if(it1.isEnd != it2.isEnd)
							return false;
						if(it1.isEnd)
							return true;
						return it1.elem == it2.elem;
					}

					friend bool operator!=(const LastDimensionMajorIterator &it1, const LastDimensionMajorIterator &it2) {
						if(it1.data != it2.data) {
                                                        return true;
						}
                                                if(it1.pos != it2.pos) {
                                                        return true;
						}
                                                if(it1.isEnd != it2.isEnd) {
                                                        return true;
						}
                                                if(it1.isEnd) {
                                                        return false;
						}
                                                return it1.elem != it2.elem;
					}
			};


			FirstDimensionMajorIterator fmbegin() {
				assert(array != nullptr);
                                return FirstDimensionMajorIterator(array, 0);
                        }

                        FirstDimensionMajorIterator fmend() {
                                return FirstDimensionMajorIterator(array, dim);
                        }

                        LastDimensionMajorIterator lmbegin() {
				LastDimensionMajorIterator it;
				it.data = &(*this);
				it.elem = &array[0];
				return it;
                        }

                        LastDimensionMajorIterator lmend() {
				LastDimensionMajorIterator it;
				it.data = this;
				it.elem = &array[0];
				it.isEnd = true;
				return it;
                        }
	};

}

#endif // #define __SKHOLE_ARRAY__

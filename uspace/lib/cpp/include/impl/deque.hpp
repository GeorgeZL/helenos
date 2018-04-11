/*
 * Copyright (c) 2018 Jaroslav Jindrak
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LIBCPP_DEQUE
#define LIBCPP_DEQUE

#include <initializer_list>
#include <iterator>
#include <memory>

namespace std
{
    template<class T, class Allocator = allocator<T>>
    class deque;

    namespace aux
    {
        /**
         * Note: We decided that these iterators contain a
         *       reference to the container and an index, which
         *       allows us to use the already implemented operator[]
         *       on deque and also allows us to conform to the requirement
         *       of the standard that functions such as push_back
         *       invalidate the .end() iterator.
         */

        template<class T, class Allocator>
        class deque_const_iterator
        {
            public:
                using size_type         = typename deque<T, Allocator>::size_type;
                using value_type        = typename deque<T, Allocator>::value_type;
                using reference         = typename deque<T, Allocator>::const_reference;
                using difference_type   = size_type;
                using pointer           = const value_type*;
                using iterator_category = random_access_iterator_tag;

                deque_const_iterator(deque<T, Allocator>& deq, size_type idx)
                    : deq_{deq}, idx_{idx}
                { /* DUMMY BODY */ }

                deque_const_iterator& operator=(const deque_const_iterator& other)
                {
                    deq_ = other.deq_;
                    idx_ = other.idx_;

                    return *this;
                }

                reference operator*() const
                {
                    return deq_[idx_];
                }

                pointer operator->() const
                {
                    return addressof(deq_[idx_]);
                }

                deque_const_iterator& operator++()
                {
                    ++idx_;

                    return *this;
                }

                deque_const_iterator operator++(int)
                {
                    return deque_const_iterator{deq_, idx_++};
                }

                deque_const_iterator& operator--()
                {
                    --idx_;

                    return *this;
                }

                deque_const_iterator operator--(int)
                {
                    return deque_const_iterator{deq_, idx_--};
                }

                deque_const_iterator operator+(difference_type n)
                {
                    return deque_const_iterator{deq_, idx_ + n};
                }

                deque_const_iterator& operator+=(difference_type n)
                {
                    idx_ += n;

                    return *this;
                }

                deque_const_iterator operator-(difference_type n)
                {
                    return deque_const_iterator{deq_, idx_ - n};
                }

                deque_const_iterator& operator-=(difference_type n)
                {
                    idx_ -= n;

                    return *this;
                }

                reference operator[](difference_type n) const
                {
                    return deq_[idx_ + n];
                }

                size_type idx() const
                {
                    return idx_;
                }

            private:
                deque<T, Allocator>& deq_;
                size_type idx_;
        };

        template<class T, class Allocator>
        bool operator==(const deque_const_iterator<T, Allocator>& lhs,
                        const deque_const_iterator<T, Allocator>& rhs)
        {
            return lhs.idx() == rhs.idx();
        }

        template<class T, class Allocator>
        bool operator!=(const deque_const_iterator<T, Allocator>& lhs,
                        const deque_const_iterator<T, Allocator>& rhs)
        {
            return !(lhs == rhs);
        }

        template<class T, class Allocator>
        class deque_iterator
        {
            public:
                using size_type         = typename deque<T, Allocator>::size_type;
                using value_type        = typename deque<T, Allocator>::value_type;
                using reference         = typename deque<T, Allocator>::reference;
                using difference_type   = size_type;
                using pointer           = value_type*;
                using iterator_category = random_access_iterator_tag;

                deque_iterator(deque<T, Allocator>& deq, size_type idx)
                    : deq_{deq}, idx_{idx}
                { /* DUMMY BODY */ }

                deque_iterator(const deque_const_iterator<T, Allocator>& other)
                    : deq_{other.deq_}, idx_{other.idx_}
                { /* DUMMY BODY */ }

                deque_iterator& operator=(const deque_iterator& other)
                {
                    deq_ = other.deq_;
                    idx_ = other.idx_;

                    return *this;
                }

                deque_iterator& operator=(const deque_const_iterator<T, Allocator>& other)
                {
                    deq_ = other.deq_;
                    idx_ = other.idx_;

                    return *this;
                }

                reference operator*()
                {
                    return deq_[idx_];
                }

                pointer operator->()
                {
                    return addressof(deq_[idx_]);
                }

                deque_iterator& operator++()
                {
                    ++idx_;

                    return *this;
                }

                deque_iterator operator++(int)
                {
                    return deque_iterator{deq_, idx_++};
                }

                deque_iterator& operator--()
                {
                    --idx_;

                    return *this;
                }

                deque_iterator operator--(int)
                {
                    return deque_iterator{deq_, idx_--};
                }

                deque_iterator operator+(difference_type n)
                {
                    return deque_iterator{deq_, idx_ + n};
                }

                deque_iterator& operator+=(difference_type n)
                {
                    idx_ += n;

                    return *this;
                }

                deque_iterator operator-(difference_type n)
                {
                    return deque_iterator{deq_, idx_ - n};
                }

                deque_iterator& operator-=(difference_type n)
                {
                    idx_ -= n;

                    return *this;
                }

                reference operator[](difference_type n) const
                {
                    return deq_[idx_ + n];
                }

                size_type idx() const
                {
                    return idx_;
                }

            private:
                deque<T, Allocator>& deq_;
                size_type idx_;
        };

        template<class T, class Allocator>
        bool operator==(const deque_iterator<T, Allocator>& lhs,
                        const deque_iterator<T, Allocator>& rhs)
        {
            return lhs.idx() == rhs.idx();
        }

        template<class T, class Allocator>
        bool operator!=(const deque_iterator<T, Allocator>& lhs,
                        const deque_iterator<T, Allocator>& rhs)
        {
            return !(lhs == rhs);
        }
    }

    /**
     * 23.3.3 deque:
     */

    template<class T, class Allocator>
    class deque
    {
        public:
            using allocator_type  = Allocator;
            using value_type      = T;
            using reference       = value_type&;
            using const_reference = const value_type&;

            using iterator               = aux::deque_iterator<T, Allocator>;
            using const_iterator         = aux::deque_const_iterator<T, Allocator>;
            using reverse_iterator       = std::reverse_iterator<iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;

            using size_type       = typename allocator_traits<allocator_type>::size_type;
            using difference_type = typename allocator_traits<allocator_type>::difference_type;
            using pointer         = typename allocator_traits<allocator_type>::pointer;
            using const_pointer   = typename allocator_traits<allocator_type>::const_pointer;

            /**
             * 23.3.3.2, construct/copy/destroy:
             */

            deque()
                : deque{allocator_type{}}
            { /* DUMMY BODY */ }

            explicit deque(const allocator_type& alloc)
                : allocator_{alloc}, front_bucket_idx_{bucket_size_},
                  back_bucket_idx_{0}, front_bucket_{default_front_},
                  back_bucket_{default_back_}, bucket_count_{default_bucket_count_},
                  bucket_capacity_{default_bucket_capacity_}, size_{}, data_{}
            {
                init_();
            }

            explicit deque(size_type n, const allocator_type& alloc = allocator_type{})
            {
                // TODO: implement
            }

            deque(size_type n, const value_type& value, const allocator_type& alloc = allocator_type{})
            {
                // TODO: implement
            }

            template<class InputIterator>
            deque(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type{})
            {
                // TODO: implement
            }

            deque(const deque& other)
            {
                // TODO: implement
            }

            deque(deque&& other)
            {
                // TODO: implement
            }

            deque(const deque& other, const allocator_type& alloc)
            {
                // TODO: implement
            }

            deque(deque&& other, const allocator_type& alloc)
            {
                // TODO: implement
            }

            deque(initializer_list<T> init, const allocator_type& alloc = allocator_type{})
            {
                // TODO: implement
            }

            ~deque()
            {
                fini_();
            }

            deque& operator=(const deque& other)
            {
                // TODO: implement
            }

            deque& operator=(deque&& other)
                noexcept(allocator_traits<allocator_type>::is_always_equal::value)
            {
                // TODO: implement
            }

            deque& operator=(initializer_list<T> init)
            {
                // TODO: implement
            }

            template<class InputIterator>
            void assign(InputIterator first, InputIterator last)
            {
                // TODO: implement
            }

            void assign(size_type n, const T& value)
            {
                // TODO: implement
            }

            void assign(initializer_list<T> init)
            {
                // TODO: implement
            }

            allocator_type get_allocator() const noexcept
            {
                return allocator_;
            }

            iterator begin() noexcept
            {
                return aux::deque_iterator{*this, 0};
            }

            const_iterator begin() const noexcept
            {
                return aux::deque_const_iterator{*this, 0};
            }

            iterator end() noexcept
            {
                return aux::deque_iterator{*this, size_};
            }

            const_iterator end() const noexcept
            {
                return aux::deque_const_iterator{*this, size_};
            }

            reverse_iterator rbegin() noexcept
            {
                return make_reverse_iterator(end());
            }

            const_reverse_iterator rbegin() const noexcept
            {
                return make_reverse_iterator(cend());
            }

            reverse_iterator rend() noexcept
            {
                return make_reverse_iterator(begin());
            }

            const_reverse_iterator rend() const noexcept
            {
                return make_reverse_iterator(cbegin());
            }

            const_iterator cbegin() const noexcept
            {
                return aux::deque_const_iterator{*this, 0};
            }

            const_iterator cend() const noexcept
            {
                return aux::deque_const_iterator{*this, size_};
            }

            const_reverse_iterator crbegin() const noexcept
            {
                return make_reverse_iterator(cend());
            }

            const_reverse_iterator crend() const noexcept
            {
                return make_reverse_iterator(cbegin());
            }

            /**
             * 23.3.3.3, capacity:
             */

            size_type size() const noexcept
            {
                return size_;
            }

            size_type max_size() const noexcept
            {
                // TODO: implement
            }

            void resize(size_type sz)
            {
                // TODO: implement
            }

            void resize(size_type sz, const value_type& value)
            {
                // TODO: implement
            }

            void shrink_to_fit()
            {
                // TODO: implement
            }

            bool empty() const noexcept
            {
                return size_ == size_type{};
            }

            reference operator[](size_type idx)
            {
                return data_[get_bucket_index_(idx)][get_element_index_(idx)];
            }

            const_reference operator[](size_type idx) const
            {
                return data_[get_bucket_index_(idx)][get_element_index_(idx)];
            }

            reference at(size_type idx)
            {
                // TODO: implement
            }

            const_reference at(size_type idx) const
            {
                // TODO: implement
            }

            reference front()
            {
                // TODO: implement
            }

            const_reference front() const
            {
                // TODO: implement
            }

            reference back()
            {
                // TODO: implement
            }

            const_reference back() const
            {
                // TODO: implement
            }

            /**
             * 23.3.3.4, modifiers:
             */

            template<class... Args>
            void emplace_front(Args&&... args)
            {
                // TODO: implement
            }

            template<class... Args>
            void emplace_back(Args&&... args)
            {
                // TODO: implement
            }

            template<class... Args>
            iterator emplace(const_iterator position, Args&&... args)
            {
                // TODO: implement
            }

            void push_front(const value_type& value)
            {
                if (front_bucket_idx_ == 0)
                    add_new_bucket_front_();

                data_[front_bucket_][--front_bucket_idx_] = value;
                ++size_;
            }

            void push_front(value_type&& value)
            {
                if (front_bucket_idx_ == 0)
                    add_new_bucket_front_();

                data_[front_bucket_][--front_bucket_idx_] = forward<value_type>(value);
                ++size_;
            }

            void push_back(const value_type& value)
            {
                data_[back_bucket_][back_bucket_idx_++] = value;
                ++size_;

                if (back_bucket_idx_ >= bucket_size_)
                    add_new_bucket_back_();
            }

            void push_back(value_type&& value)
            {
                data_[back_bucket_][back_bucket_idx_++] = forward<value_type>(value);
                ++size_;

                if (back_bucket_idx_ >= bucket_size_)
                    add_new_bucket_back_();
            }

            iterator insert(const_iterator position, const value_type& value)
            {
                // TODO: implement
            }

            iterator insert(const_iterator position, value_type&& value)
            {
                // TODO: implement
            }

            iterator insert(const_iterator position, size_type n, const value_type& value)
            {
                // TODO: implement
            }

            template<class InputIterator>
            iterator insert(const_iterator position, InputIterator first, InputIterator last)
            {
                // TODO: implement
            }

            iterator insert(const_iterator position, initializer_list<value_type> init)
            {
                // TODO: implement
            }

            void pop_back()
            {
                if (empty())
                    return;

                if (back_bucket_idx_ == 0)
                { // Means we gotta pop data_[front_bucket_][bucket_size_ - 1]!
                    if (data_[back_bucket_])
                        allocator_.deallocate(data_[back_bucket_], bucket_size_);

                    --back_bucket_;
                    back_bucket_idx_ = bucket_size_ - 1;
                }
                else
                    --back_bucket_idx_;

                allocator_.destroy(&data_[back_bucket_][back_bucket_idx_]);
                --size_;
            }

            void pop_front()
            {
                if (empty())
                    return;

                if (front_bucket_idx_ >= bucket_size_)
                { // Means we gotta pop data_[front_bucket_][0]!
                    if (data_[front_bucket_])
                        allocator_.deallocate(data_[front_bucket_], bucket_size_);

                    ++front_bucket_;
                    front_bucket_idx_ = 1;

                    allocator_.destroy(&data_[front_bucket_][0]);
                }
                else
                {
                    allocator_.destroy(&data_[front_bucket_][front_bucket_idx_]);

                    ++front_bucket_idx_;
                }

                --size_;
            }

            iterator erase(const_iterator position)
            {
                // TODO: implement
            }

            iterator erase(const_iterator first, const_iterator last)
            {
                // TODO: implement
            }

            void swap(deque& other)
                noexcept(allocator_traits<allocator_type>::is_always_equal::value)
            {
                // TODO: implement
            }

            void clear() noexcept
            {
                fini_();

                front_bucket_ = default_front_;
                back_bucket_ = default_back_;
                bucket_count_ = default_bucket_count_;
                bucket_capacity_ = default_bucket_capacity_;
                size_ = size_type{};

                init_();
            }

        /* private: */
            allocator_type allocator_;

            /**
             * Note: In our implementation, front_bucket_idx_
             *       points at the first element and back_bucket_idx_
             *       points at the one past last element. Because of this,
             *       some operations may be done in inverse order
             *       depending on the side they are applied to.
             */
            size_type front_bucket_idx_;
            size_type back_bucket_idx_;
            size_type front_bucket_;
            size_type back_bucket_;

            static constexpr size_type bucket_size_{16};
            static constexpr size_type default_bucket_count_{2};
            static constexpr size_type default_bucket_capacity_{4};
            static constexpr size_type default_front_{1};
            static constexpr size_type default_back_{2};

            size_type bucket_count_;
            size_type bucket_capacity_;
            size_type size_{};

            value_type** data_;

            void init_()
            {
                data_ = new value_type*[bucket_capacity_];

                for (size_type i = front_bucket_; i <= back_bucket_; ++i)
                    data_[i] = allocator_.allocate(bucket_size_);
            }

            void fini_()
            {
                for (size_type i = front_bucket_; i <= back_bucket_; ++i)
                    allocator_.deallocate(data_[i], bucket_size_);

                delete[] data_;
                data_ = nullptr;
            }

            bool has_bucket_space_back_() const
            {
                return back_bucket_ < bucket_capacity_ - 1;
            }

            bool has_bucket_space_front_() const
            {
                return front_bucket_ > 0;
            }

            void add_new_bucket_back_()
            {
                if (!has_bucket_space_back_())
                    expand_();

                ++back_bucket_;
                data_[back_bucket_] = allocator_.allocate(bucket_size_);
                back_bucket_idx_ = size_type{};
            }

            void add_new_bucket_front_()
            {
                if (!has_bucket_space_front_())
                    expand_();

                --front_bucket_;
                data_[front_bucket_] = allocator_.allocate(bucket_size_);
                front_bucket_idx_ = bucket_size_;
            }

            void expand_()
            {
                bucket_capacity_ *= 2;
                value_type** new_data = new value_type*[bucket_capacity_];

                size_type new_front = bucket_capacity_ / 4;
                size_type new_back = bucket_capacity_ - bucket_capacity_ / 4 - 1;

                for (size_type i = new_front, j = front_bucket_; i <= new_back; ++i, ++j)
                    new_data[i] = move(data_[j]);
                std::swap(data_, new_data);

                delete[] new_data;
                front_bucket_ = new_front;
                back_bucket_ = new_back;
            }

            size_type get_bucket_index_(size_type idx) const
            {
                if (idx < elements_in_front_bucket_())
                    return front_bucket_;

                idx -= elements_in_front_bucket_();
                return idx / bucket_size_ + front_bucket_ + 1;
            }

            size_type get_element_index_(size_type idx) const
            {
                if (idx < elements_in_front_bucket_())
                    return bucket_size_ - elements_in_front_bucket_() + idx;

                idx -= elements_in_front_bucket_();
                return idx % bucket_size_;
            }

            size_type elements_in_front_bucket_() const
            {
                return bucket_size_ - front_bucket_idx_;
            }
    };

    template<class T, class Allocator>
    bool operator==(const deque<T, Allocator>& lhs, const deque<T, Allocator>& rhs)
    {
        // TODO: implement
        return false;
    }

    template<class T, class Allocator>
    bool operator<(const deque<T, Allocator>& lhs, const deque<T, Allocator>& rhs)
    {
        // TODO: implement
        return false;
    }

    template<class T, class Allocator>
    bool operator!=(const deque<T, Allocator>& lhs, const deque<T, Allocator>& rhs)
    {
        // TODO: implement
        return false;
    }

    template<class T, class Allocator>
    bool operator>(const deque<T, Allocator>& lhs, const deque<T, Allocator>& rhs)
    {
        // TODO: implement
        return false;
    }

    template<class T, class Allocator>
    bool operator<=(const deque<T, Allocator>& lhs, const deque<T, Allocator>& rhs)
    {
        // TODO: implement
        return false;
    }

    template<class T, class Allocator>
    bool operator>=(const deque<T, Allocator>& lhs, const deque<T, Allocator>& rhs)
    {
        // TODO: implement
        return false;
    }

    /**
     * 23.3.3.5, deque specialized algorithms:
     */

    template<class T, class Allocator>
    void swap(deque<T, Allocator>& lhs, deque<T, Allocator>& rhs)
        noexcept(noexcept(lhs.swap(rhs)))
    {
        lhs.swap(rhs);
    }
}

#endif

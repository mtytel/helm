/* Copyright 2013-2017 Matt Tytel
 *
 * mopo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mopo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mopo.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include "processor.h"
#include "utils.h"

#include <cstddef>

namespace mopo {

  template <class T>
  class CircularQueue {
    public:

      class iterator {
        public:
          iterator(T* pointer, T* front, T* end) : pointer_(pointer), front_(front), end_(end) { }

          inline void increment() {
            if (pointer_ == end_)
              pointer_ = front_;
            else
              pointer_++;
          }

          iterator operator++() {
            iterator iter = *this;
            increment();
            return iter;
          }

          iterator operator++(int i) {
            iterator iter = *this;
            increment();
            return iter;
          }

          T& operator*() {
            return *pointer_;
          }

          T* operator->() {
            return pointer_;
          }

          T* get() {
            return pointer_;
          }

          bool operator==(const iterator& rhs) {
            return pointer_ == rhs.pointer_;
          }

          bool operator!=(const iterator& rhs) {
            return pointer_ != rhs.pointer_;
          }

        protected:
          T* pointer_;
          T* front_;
          T* end_;
      };

      CircularQueue(int capacity) : capacity_(capacity + 1), start_(0), end_(0) {
        data_ = new T[capacity_];
      }

      CircularQueue() : data_(nullptr), capacity_(0), start_(0), end_(0) { }

      void reserve(int capcity) {
        if (data_)
          delete data_;

        capacity_ = capcity + 1;
        data_ = new T[capacity_];
        start_ = 0;
        end_ = 0;
      }

      T& operator[](std::size_t index) {
        return data_[(start_ + static_cast<int>(index)) % capacity_];
      }

      const T& operator[](std::size_t index) const {
        return data_[(start_ + static_cast<int>(index)) % capacity_];
      }

      void push_back(T entry) {
        data_[end_] = entry;
        end_ = (end_ + 1) % capacity_;
      }

      T pop_back() {
        end_ = (end_ - 1 + capacity_) % capacity_;
        return data_[end_];
      }

      void push_front(T entry) {
        start_ = (start_ - 1 + capacity_) % capacity_;
        data_[start_] = entry;
      }

      T pop_front() {
        int last = start_;
        start_ = (start_ + 1) % capacity_;
        return data_[last];
      }

      void removeAt(int index) {
        int i = (index + start_) % capacity_;
        end_ = (end_ - 1 + capacity_) % capacity_;
        while (i != end_) {
          int next = (i + 1) % capacity_;
          data_[i] = data_[next];
          i = next;
        }
      }

      void remove(T entry) {
        for (int i = start_; i != end_; i = (i + 1) % capacity_) {
          if (data_[i] == entry) {
            removeAt((i - start_ + capacity_) % capacity_);
            return;
          }
        }
      }

      iterator erase(iterator& iter) {
        int index = iter.get() - data_;
        removeAt((index - start_ + capacity_) % capacity_);
        return iter;
      }

      int count(T entry) const {
        int number = 0;
        for (int i = start_; i != end_; i = (i + 1) % capacity_) {
          if (data_[i] == entry)
            number++;
        }
        return number;
      }

      void clear() {
        start_ = 0;
        end_ = 0;
      }

      T front() const {
        return data_[start_];
      }

      T back() const {
        return data_[(end_ - 1 + capacity_) % capacity_];
      }

      int size() const {
        return (end_ - start_ + capacity_) % capacity_;
      }

      iterator begin() const {
        return iterator(data_ + start_, data_, data_ + (capacity_ - 1));
      }

      iterator end() const {
        return iterator(data_ + end_, data_, data_ + (capacity_ - 1));
      }

    private:
      T* data_;
      int capacity_;
      int start_;
      int end_;
  };
} // namespace mopo

#endif // CIRCULAR_QUEUE_H

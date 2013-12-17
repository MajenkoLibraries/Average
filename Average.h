/*
 * Copyright (c) 2013, Majenko Technologies
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 *  1. Redistributions of source code must retain the above copyright notice, 
 *     this list of conditions and the following disclaimer.
 * 
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 * 
 *  3. Neither the name of Majenko Technologies nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without 
 *     specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef _AVERAGE_H
#define _AVERAGE_H

#if (ARDUINO >= 100) 
# include <Arduino.h>
#else
# include <WProgram.h>
#endif

template <class T> class Average {
    private:
        // Private functions and variables here.  They can only be accessed
        // by functions within the class.
        T *_store;
        uint32_t _count;
        uint32_t _size;

    public:
        // Public functions and variables.  These can be accessed from
        // outside the class.
        Average(uint32_t size);
        float rolling(T entry);
        void push(T entry);
        float mean();
        T mode();
        T minimum();
        T maximum();
        float stddev();
        T get(uint32_t);
};


template <class T> Average<T>::Average(uint32_t size) {
    _size = size;
    _count = 0;
    _store = (T *)malloc(sizeof(T) * size);
    for (uint32_t i = 0; i < size; i++) {
        _store[i] = 0;
    }
}

template <class T> void Average<T>::push(T entry) {
	for(uint32_t i = _size - 1; i > 0; i--) {
		_store[i] = _store[i-1];
	}
	_store[0] = entry;

    if (_count < _size) {
        _count++;
    }
}


template <class T> float Average<T>::rolling(T entry) {
    push(entry);
    return mean();
}

template <class T> float Average<T>::mean() {
	float total = 0;

    if (_count == 0) {
        return 0;
    }

	for(uint32_t i = 0; i < _count; i++) {
		total = total + _store[i];
	}
	return total / (float)_count;
}

template <class T> T Average<T>::mode() {
	uint32_t pos;
	uint32_t inner;
	T most;
	uint32_t mostcount;
	T current;
	uint32_t currentcount;

    if (_count == 0) {
        return 0;
    }

	most = _store[0];
	mostcount = 1;
	for(pos = 0; pos < _count; pos++) {
		current = _store[pos];
		currentcount = 0;
		for(inner = pos + 1; inner < _count; inner++) {
			if(_store[inner] == current) {
				currentcount++;
			}
		}
		if(currentcount > mostcount) {
			most = current;
			mostcount = currentcount;
		}
		// If we have less array slices left than the current
		// maximum count, then there is no room left to find
		// a bigger count.  We have finished early and we can
		// go home.
		if(_count - pos < mostcount) {
			break;
		}
	}
	return most;
}

template <class T> T Average<T>::minimum() {
	T minval;

    if (_count == 0) {
        return 0;
    }

	minval = _store[0];

	for(uint32_t i = 0; i < _count; i++) {
		if(_store[i] < minval) {
			minval = _store[i];
		}
	}
	return minval;
}

template <class T> T Average<T>::maximum() {
	T maxval;

    if (_count == 0) {
        return 0;
    }

	maxval = _store[0];

	for(uint32_t i = 0; i < _count; i++) {
		if(_store[i] > maxval) {
			maxval = _store[i];
		}
	}
	return maxval;
}

template <class T> float Average<T>::stddev() {
	float square;
	float sum;
	float mu;
	float theta;
	int i;

    if (_count == 0) {
        return 0;
    }

	mu = mean();

	sum = 0;
	for(uint32_t i = 0; i < _count; i++) {
		theta = mu - (float)_store[i];
		square = theta * theta;
		sum += square;
	}
	return sqrt(sum/(float)_count);
}

template <class T> T Average<T>::get(uint32_t index) {
    if (index >= _count) {
        return 0;
    }
    return _store[index];
}
 
#endif

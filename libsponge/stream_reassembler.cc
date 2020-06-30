#include "stream_reassembler.hh"

#include <iostream>

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    : _output(capacity), _capacity(capacity), _begin(0), _end(0), _buf(capacity, 0), _writen(capacity, false) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    if (eof) {
        _eof = true;
    }

    std::copy(data.begin(), data.end(), _buf.begin() + index);
    std::fill_n(_writen.begin() + index, data.size(), true);
    _end = std::max(_end, index + data.size());

    auto it = _writen.begin() + _begin;
    size_t count = 0;
    while (*it) {
        *it = false;
        it++;
        count++;
    }

    _output.write(_buf.substr(_begin, count));
    _begin += count;

    if (_eof && empty()) {
        _output.end_input();
    }
}

size_t StreamReassembler::unassembled_bytes() const { return std::accumulate(_writen.begin(), _writen.end(), 0); }

bool StreamReassembler::empty() const { return _end == _begin; }

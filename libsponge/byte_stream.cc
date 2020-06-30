#include "byte_stream.hh"

#include <algorithm>
#include <ios>
#include <iostream>
#include <iterator>
#include <stdexcept>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity)
    : _buf(std::ios_base::app | std::ios_base::in | std::ios_base::out), _cap(capacity) {}

size_t ByteStream::write(const string &data) {
    if (_inputEnded) {
        return {};
    }
    auto remain = remaining_capacity();
    if (remain > data.size()) {
        _buf << data;
        _buf_size += data.size();
        return data.size();
    }
    _buf << data.substr(0, remain);
    _buf_size += remain;
    return remain;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    if (len == 0) {
        return {};
    }
    const auto &buf = _buf.str();
    return {buf.begin() + bytes_read(), buf.begin() + len + bytes_read()};
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    if (len == 0) {
        return;
    }
    if (len >= buffer_size()) {
        _buf_size = 0;
        _buf.seekg(ios_base::end);
        if (_inputEnded) {
            _buf.setstate(ios_base::eofbit);
        }
        return;
    }

    _buf.seekg(len, ios_base::cur);
    _buf_size -= len;
}

void ByteStream::end_input() {
    _inputEnded = true;
    if (buffer_empty()) {
        _buf.setstate(ios_base::eofbit);
    }
}

bool ByteStream::input_ended() const { return _inputEnded; }

size_t ByteStream::buffer_size() const { return _buf_size; }

bool ByteStream::buffer_empty() const { return buffer_size() == 0; }

bool ByteStream::eof() const { return _buf.eof(); }

size_t ByteStream::bytes_written() const { return _buf.str().size(); }

size_t ByteStream::bytes_read() const { return bytes_written() - buffer_size(); }

size_t ByteStream::remaining_capacity() const { return _cap - buffer_size(); }

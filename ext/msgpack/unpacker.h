/*
 * MessagePack for Ruby
 *
 * Copyright (C) 2008-2012 FURUHASHI Sadayuki
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
#ifndef MSGPACK_RUBY_UNPACKER_H__
#define MSGPACK_RUBY_UNPACKER_H__

#include "buffer.h"

#ifndef MSGPACK_UNPACKER_STACK_CAPACITY
#define MSGPACK_UNPACKER_STACK_CAPACITY 128
#endif

struct msgpack_unpacker_t;
typedef struct msgpack_unpacker_t msgpack_unpacker_t;

enum stack_type_t {
    STACK_TYPE_ARRAY,
    STACK_TYPE_MAP,
};

typedef struct {
    size_t count;
    enum stack_type_t type;
    VALUE object;
    VALUE key;
} msgpack_unpacker_stack_t;

struct msgpack_unpacker_t {
    msgpack_buffer_t buffer;

    unsigned int head_byte;

    msgpack_unpacker_stack_t* stack;
    size_t stack_depth;
    size_t stack_capacity;

    VALUE last_object;

    VALUE reading_raw;
    size_t reading_raw_remaining;

    VALUE io;
    VALUE io_buffer;
    ID io_partial_read_method;
};

#define UNPACKER_BUFFER_(uk) (&(uk)->buffer)

enum msgpack_unpacker_object_type {
    NIL,
    BOOLEAN,
    INTEGER,
    FLOAT,
    ARRAY,
    MAP,
};

void msgpack_unpacker_init(msgpack_unpacker_t* uk);

void msgpack_unpacker_destroy(msgpack_unpacker_t* uk);

void msgpack_unpacker_mark(msgpack_unpacker_t* uk);

static inline void msgpack_unpacker_set_io(msgpack_unpacker_t* uk, VALUE io, ID io_partial_read_method)
{
    uk->io = io;
    uk->io_partial_read_method = io_partial_read_method;
}



/* error codes */
#define PRIMITIVE_CONTAINER_START 1
#define PRIMITIVE_OBJECT_COMPLETE 0
#define PRIMITIVE_EOF -1
#define PRIMITIVE_INVALID_BYTE -2
#define PRIMITIVE_STACK_TOO_DEEP -3
#define PRIMITIVE_UNEXPECTED_TYPE -4

int msgpack_unpacker_read(msgpack_unpacker_t* uk, size_t target_stack_depth);

int msgpack_unpacker_skip(msgpack_unpacker_t* uk, size_t target_stack_depth);

static inline VALUE msgpack_unpacker_get_last_object(msgpack_unpacker_t* uk)
{
    return uk->last_object;
}


int get_next_object_type(msgpack_unpacker_t* uk);

int msgpack_unpacker_skip_nil(msgpack_unpacker_t* uk);

long msgpack_unpacker_read_array_header(msgpack_unpacker_t* uk);

long msgpack_unpacker_read_map_header(msgpack_unpacker_t* uk);

#endif

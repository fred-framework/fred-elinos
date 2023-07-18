#ifndef FRED_LIB_CPP_H
#define FRED_LIB_CPP_H

#include <array>
#include <cassert>
#include <memory>
#include <stdexcept>

// Angle brackets?
#include "fred_lib.h"

// class fredcpp_hw_task;

class fredcpp {
    // FIXME: make this live for as long as it takes!!

    using fred_ptr = std::unique_ptr<struct fred_data, decltype(&fred_free)>;
    fred_ptr data;

    friend class fredcpp_hw_task;

private:
    static inline struct fred_data *fred_init_throw() {
        struct fred_data *temp;
        int res = fred_init(&temp);
        if (res) {
            throw std::runtime_error("Could not initialize FRED!\n");
        }
        return temp;
    }

    struct fred_data *operator*() {
        return data.get();
    };

public:
    // unique_ptr will invoke fred_free for us automatically in its
    // destructor.
    //
    // NOTE: since we have a unique_ptr, copy is deleted by default!!
    fredcpp() : data(fred_init_throw(), fred_free) {}
};

class fredcpp_hw_task {
    // Implementation assumes that fredcpp_hw_task has at
    // least the same lifetime as the referenced handle!!
    fredcpp &handle;

    // No need for unique_ptr because it doesn't have any stuff to do on
    // delete
    using fred_hw_task_ptr = struct fred_hw_task *;
    fred_hw_task_ptr task;

private:
    static inline struct fred_hw_task *fred_bind_throw(fredcpp &handle,
                                                       int hw_task_id) {
        struct fred_hw_task *temp;
        int res = fred_bind(*handle, &temp, hw_task_id);
        if (res) {
            // TODO: print fred task info as well to string
            throw std::runtime_error("Could not bind to FRED HW task!\n");
        }
        return temp;
    }

public:
    fredcpp_hw_task(fredcpp &handle_, int hw_task_id) :
        handle(handle_), task(fred_bind_throw(handle, hw_task_id)) {}

    void *map_buffer(int index) {
        void *retval = fred_map_buff(*handle, task, index);
        if (retval == nullptr) {
            throw std::runtime_error("Could not map FRED buffer!\n");
        }
        return retval;
    }

    void accel() {
        int retval = fred_accel(handle.data.get(), task);
        if (retval) {
            throw std::runtime_error("Fred acceleration call failed!");
        }
    }
};

// NOTE: there is no distinction between input and output arguments;
// functor behaves like void() when called
template <int Id, typename... Args>
class fredcpp_function {
private:
    static constexpr int hw_task_id = Id;
    // Use pointer to args
    using args_tuple = std::tuple<Args *...>;
    fredcpp_hw_task task;
    args_tuple arguments;

public:
    static constexpr size_t num_args = std::tuple_size<args_tuple>::value;
    using args_ref_type = std::tuple<Args &...>;

private:
    // Returns mapped pointer to i-th argument
    template <typename T, std::size_t index>
    T *get_argument() {
        return static_cast<T *>(task.map_buffer(index));
    }

    // Returns a tuple of the argument pointers (must be called by
    // get_arguments())
    template <std::size_t... Is>
    auto get_arguments(std::index_sequence<Is...>) {
        return std::make_tuple<Args *...>({get_argument<Args, Is>()}...);
    }

    // Returns a tuple of the argument pointers
    auto get_arguments() {
        return get_arguments(std::make_index_sequence<num_args>{});
    }

public:
    fredcpp_function(fredcpp &handle) :
        task(handle, hw_task_id), arguments(get_arguments()) {}

    template <int i>
    auto &get() {
        return *std::get<i>(arguments);
    }

    void operator()() {
        return task.accel();
    }
};

#endif // FRED_LIB_CPP_H

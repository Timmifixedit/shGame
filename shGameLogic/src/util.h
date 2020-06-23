//
// Created by tim on 13.06.20.
//
#ifndef SHGAME_UTIL_H
#define SHGAME_UTIL_H
#include <array>

namespace sh::util {

    namespace customTypeTraits {
        /**
         * SFINAE hacks...
         */
        template<typename T>
        std::true_type test(T *t, typename std::remove_reference<decltype(t->begin(), t->end(), t)>::type);

        template<typename T>
        std::false_type test(...);

        template<typename T>
        struct is_iterable : decltype(test<T>(nullptr, nullptr)) {};
    }


/**
 * Returns a random integer number between min and max
 * @param min lower bound (inclusive)
 * @param max upper bound (inclusive)
 * @return
 */
    int rng(int min, int max);

    /**
     * Selects a random element from a container
     * @tparam T container type (not value type of container)
     * @param container container where the element is drawn from
     * @return iterator pointing to elected element
     */
    template<typename T>
    auto selectRandom(T &container) -> decltype(container.size(), std::begin(container) + int()) {
        return std::begin(container) + rng(0, static_cast<int>(container.size() - 1));
    }

    /**
     * Overload of function above for standard c arrays
     * @tparam T type of arry
     * @tparam N array size
     * @param array array where the element is drawn from
     * @return pointer to selected element
     */
    template<typename T, std::size_t N>
    auto selectRandom(T (&array)[N]) -> T* {
        return array + rng(0, static_cast<int>(N - 1));
    }
}

#endif //SHGAME_UTIL_H

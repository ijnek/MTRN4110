#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include <stdint.h>    // NOLINT(modernize-deprecated-headers, hicpp-deprecated-headers)
// ReSharper disable once CppUnusedIncludeDirective
#include <stdlib.h>    // NOLINT(modernize-deprecated-headers, hicpp-deprecated-headers)

namespace example {
	/**
	 * \brief hello_world class will print "hello world\n" n number a times specified at compile time.
	 * \tparam times is the number of times hello world will be printed. times can be 1 to 5.
	 */
	template <int times>
	class hello_world {
		static_assert(times >= 1, "n must be at least 1");
		static_assert (times <= 5, "n must be less or equal to 5 times.");

	public:
		/**
		 * \brief enable function initialise the serial bus ready for print.
		 */
		static auto enable () -> void;

		/**
		 * \brief Print "Hello world\n" n number of times specified by template argument constant times.
		 * \return the total number of characters printed.
		 */
		static auto print ()->size_t;
	};
}

#ifndef HYPER_NON_COPYABLE_HPP
#define HYPER_NON_COPYABLE_HPP

#include <system/export.hpp>

namespace hyp {
	class HYPER_API NonCopyable {
	protected:
		NonCopyable() {}

	private:
		NonCopyable(const NonCopyable&) = delete;
		NonCopyable& operator=(const NonCopyable&) = delete;
	};
}

#endif
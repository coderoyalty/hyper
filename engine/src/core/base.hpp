#ifndef HYPER_CORE_HPP
#define HYPER_CORE_HPP

#include <memory>

namespace hyp {
	template <typename T>
	using Ref = std::shared_ptr<T>;

	template <typename T>
	using Shared = Ref<T>;

	template <typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args&&... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Scope = std::unique_ptr<T>;

	template <typename T>
	using Unique = Scope<T>;

	template <typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args&&... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

};

#endif
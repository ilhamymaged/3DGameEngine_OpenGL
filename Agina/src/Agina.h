#pragma once

#include <memory>

namespace std {
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ...Args>
	constexpr Ref<T> make_Ref(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}
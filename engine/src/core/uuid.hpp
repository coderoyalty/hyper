#pragma once
#ifndef HYP_UUID_HPP

	#include <random>

namespace hyp {
	class UUID {
	public:
		UUID();

		UUID(uint64_t uuid);

		UUID(const UUID&) = default;

		operator uint64_t() const { return m_uuid; }

	private:
		uint64_t m_uuid;
	};
}

namespace std {
	template <typename T>
	struct hash;

	template <>
	struct hash<hyp::UUID>
	{
		std::size_t operator()(const hyp::UUID& uuid) const {
			return (uint64_t)uuid;
		}
	};

}

#endif
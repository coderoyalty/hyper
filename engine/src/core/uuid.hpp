#pragma once
#ifndef HYP_UUID_HPP

	#include <random>

namespace hyp {
	class UUID {
	public:
		UUID();

		UUID(uint64_t uuid);

		UUID(const UUID&) = default;

		uint64_t operator()() const {
			return m_uuid;
		}

	private:
		uint64_t m_uuid;
	};
}

#endif
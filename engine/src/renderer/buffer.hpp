#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <string>
#include <cstdint>
#include <vector>

#include "utils/logger.hpp"
#include "core/base.hpp"

namespace hyp {

	enum class ShaderDataType
	{
		None = 0,
		Float,
		Vec2,
		Vec3,
		Vec4,
		Mat3,
		Mat4,
		Int,
		IVec2,
		IVec3,
		IVec4,
		Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type) {
		switch (type)
		{
		case ShaderDataType::Float:
			return 4;
		case ShaderDataType::Vec2:
			return 4 * 2;
		case ShaderDataType::Vec3:
			return 4 * 3;
		case ShaderDataType::Vec4:
			return 4 * 4;
		case ShaderDataType::Mat3:
			return 4 * 3 * 3;
		case ShaderDataType::Mat4:
			return 4 * 4 * 4;
		case ShaderDataType::Int:
			return 4;
		case ShaderDataType::IVec2:
			return 4 * 2;
		case ShaderDataType::IVec3:
			return 4 * 3;
		case ShaderDataType::IVec4:
			return 4 * 4;
		case ShaderDataType::Bool:
			return 1;
		default:
			HYP_WARN("invalid ShaderDataType!");
		};

		return 0;
	};

	struct VertexAttribDescriptor
	{
		ShaderDataType type;
		uint32_t size;
		size_t offset;
		std::string name;
		bool normalized;

		VertexAttribDescriptor() = default;

		VertexAttribDescriptor(ShaderDataType type, const std::string& name, bool normalized = false)
		    : type(type), name(name), offset(0), size(ShaderDataTypeSize(type)), normalized(false) {
		}

		uint32_t getComponentCount() const {
			switch (type)
			{
			case ShaderDataType::Float:
				return 1;
			case ShaderDataType::Vec2:
				return 2;
			case ShaderDataType::Vec3:
				return 3;
			case ShaderDataType::Vec4:
				return 4;
			case ShaderDataType::Mat3:
				return 3; // 3* float3
			case ShaderDataType::Mat4:
				return 4; // 4* float4
			case ShaderDataType::Int:
				return 1;
			case ShaderDataType::IVec2:
				return 2;
			case ShaderDataType::IVec3:
				return 3;
			case ShaderDataType::IVec4:
				return 4;
			case ShaderDataType::Bool:
				return 1;
			default:
				HYP_WARN("invalid ShaderDataType!");
			}

			return 0;
		}
	};

	class BufferLayout {
	public:
		BufferLayout() {
		}

		BufferLayout(std::initializer_list<VertexAttribDescriptor> attributesDescriptor)
		    : m_attribDescriptors(attributesDescriptor) {
			computeLayout();
		}

		uint32_t getStride() const {
			return m_stride;
		}

		const std::vector<VertexAttribDescriptor>& getAttributes() const {
			return m_attribDescriptors;
		};

		std::vector<VertexAttribDescriptor>::iterator begin() {
			return m_attribDescriptors.begin();
		};

		std::vector<VertexAttribDescriptor>::iterator end() {
			return m_attribDescriptors.end();
		};

		std::vector<VertexAttribDescriptor>::const_iterator begin() const {
			return m_attribDescriptors.begin();
		};

		std::vector<VertexAttribDescriptor>::const_iterator end() const {
			return m_attribDescriptors.end();
		};

	private:
		void computeLayout() {
			uint32_t offset = 0;
			m_stride = 0;
			for (auto& attribute : m_attribDescriptors)
			{
				attribute.offset = offset;
				offset += attribute.size;
				m_stride += attribute.size;
			}
		}

	private:
		uint32_t m_stride;
		std::vector<VertexAttribDescriptor> m_attribDescriptors;
	};

};

#endif

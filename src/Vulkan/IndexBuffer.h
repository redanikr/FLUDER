#pragma once
#include "Buffer.h"

namespace fldr::vk {
	class IndexBuffer : public Buffer<uint32_t> {
	public:
		IndexBuffer(const Device & device);
	};
}
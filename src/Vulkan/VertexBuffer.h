#pragma once
#include "Device.h"
#include "Buffer.h"

namespace fldr::vk {
	class VertexBuffer : public Buffer<float> {
	public:
		VertexBuffer(const Device& device);
	};
}
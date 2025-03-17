#include "VertexBuffer.h"


fldr::vk::VertexBuffer::VertexBuffer(const Device& device) :
	Buffer(device,VK_BUFFER_USAGE_VERTEX_BUFFER_BIT) {
}

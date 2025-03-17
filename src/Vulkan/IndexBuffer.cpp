#include "IndexBuffer.h"

fldr::vk::IndexBuffer::IndexBuffer(const Device& device) :
	Buffer(device,VK_BUFFER_USAGE_INDEX_BUFFER_BIT){
}

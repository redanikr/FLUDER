#include "Vulkan/Device.h"
#include "Vulkan/Instance.h"
int main(){
    FLUDER::Vulkan::Instance instance(true);
    FLUDER::Vulkan::Device device(instance);
    return 0;
}
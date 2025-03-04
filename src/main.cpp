#include "Window.h"
#include "Vulkan/Config/VulkanManager.h"
int main(){
    FLUDER::Window window;
    FLUDER::Vulkan::VulkanManager configuration(window,true);
    while(!window.shouldClose())
     	configuration.draw();
    return 0;
}

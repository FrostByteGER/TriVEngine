# TriVEngine
Simple Vulkan engine written in C++ with VulkanSDK, GLFW and GLM.

## Visual Studio & C++
TriVEngine is a Visual Studio 2017 project and does not support older VS version. You may try to run it on an older version, but I will not provide any support.
Additionally, all code is written in C++17 standard. Thus the code cannot be compiled on any standard below C++17.

## Libraries
This project uses several libraries that are included in this repository.
Currently, the project uses the following libraries in the specified version. Using a newer version may or may not break the project:
 - GLFW 3.2.1
 - GLM 0.9.8.5
 
The VulkanSDK however is currently not included in the repository. To compile the project, you need the following version of the VulkanSDK. Using an older or newer version may result in compile errors:
 - VulkanSDK 1.0.68.0
 
When installing the SDK, create an environment variable "VK_HOME" and set it to the root directory of your VulkanSDK installation.
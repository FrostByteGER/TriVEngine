#pragma once
#include <vulkan/vulkan.h>
#include <functional>

namespace TriV
{
	template <typename T>
	class VDeleter {
	public:
		VDeleter() : VDeleter([](T, VkAllocationCallbacks*) {}) {}

		VDeleter(std::function<void(T, VkAllocationCallbacks*)> deletef) {
			this->_deleter = [=](T obj) { deletef(obj, nullptr); };
		}

		VDeleter(const VDeleter<VkInstance>& instance, std::function<void(VkInstance, T, VkAllocationCallbacks*)> deletef) {
			this->_deleter = [&instance, deletef](T obj) { deletef(instance, obj, nullptr); };
		}

		VDeleter(const VDeleter<VkDevice>& device, std::function<void(VkDevice, T, VkAllocationCallbacks*)> deletef) {
			this->_deleter = [&device, deletef](T obj) { deletef(device, obj, nullptr); };
		}

		~VDeleter() {
			Cleanup();
		}

		const T* operator &() const {
			return &_object;
		}

		T* Replace() {
			Cleanup();
			return &_object;
		}

		operator T() const {
			return _object;
		}

		void operator=(T rhs) {
			if (rhs != _object) {
				Cleanup();
				_object = rhs;
			}
		}

		template<typename V>
		bool operator==(V rhs) {
			return _object == T(rhs);
		}

	private:
		T _object{ VK_NULL_HANDLE };
		std::function<void(T)> _deleter;

		void Cleanup() {
			if (_object != VK_NULL_HANDLE) {
				_deleter(_object);
			}
			_object = VK_NULL_HANDLE;
		}
	};
}

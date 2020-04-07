#pragma once

#include "foundation/PxAllocatorCallback.h"
#include "foundation/PxAssert.h"

#if PX_WINDOWS || PX_LINUX_FAMILY || PX_SWITCH
#include <malloc.h>
#endif

namespace TriV::Engine::Physics {

	class PhysXDefaultAllocator : public physx::PxAllocatorCallback
	{
	public:
		void* allocate(const size_t size, const char*, const char*, int) override
		{
			void* ptr = platformAlignedAlloc(size);
			PX_ASSERT((reinterpret_cast<size_t>(ptr) & 15) == 0);
			return ptr;
		}

		void deallocate(void* ptr) override
		{
			platformAlignedFree(ptr);
		}

	private:
#if PX_WINDOWS
		// on win32 we only have 8-byte alignment guaranteed, but the CRT provides special aligned allocation fns
		static PX_FORCE_INLINE void* platformAlignedAlloc(size_t size)
		{
			return _aligned_malloc(size, 16);
		}

		static PX_FORCE_INLINE void platformAlignedFree(void* ptr)
		{
			_aligned_free(ptr);
		}
#elif PX_LINUX_FAMILY || PX_SWITCH
		static PX_FORCE_INLINE void* platformAlignedAlloc(size_t size)
		{
			return ::memalign(16, size);
		}

		static PX_FORCE_INLINE void platformAlignedFree(void* ptr)
		{
			::free(ptr);
		}
#else
		// on all other platforms we get 16-byte alignment by default
		static PX_FORCE_INLINE void* platformAlignedAlloc(size_t size)
		{
			return ::malloc(size);
		}

		static PX_FORCE_INLINE void platformAlignedFree(void* ptr)
		{
			::free(ptr);
		}
#endif
	};
}
#pragma once
#include <cassert>
#include <concepts>
#include <atomic>
#include <limits>

#include <Unknwn.h>

// TODO: actually support multiple interfaces
template<typename... InterfaceT>
	requires(std::derived_from<InterfaceT, IUnknown> && ...)
class Base : public InterfaceT...
{
	std::atomic<ULONG> m_RefCount;

public:
	Base()
		: m_RefCount{ 0 }
	{
	}

	~Base()
	{
		assert(m_RefCount == 0 && "Object was deleted, although there are still references!");
	}

	Base(const Base&) noexcept = delete;
	Base& operator=(const Base&) noexcept = delete;

	Base(Base&&) noexcept = default;
	Base& operator=(Base&&) noexcept = default;

public: // RefCountedObject
	virtual ULONG STDMETHODCALLTYPE AddRef() override
	{
		assert(m_RefCount < std::numeric_limits<ULONG>::max());

		return ++m_RefCount;
	}

	virtual ULONG STDMETHODCALLTYPE Release() override
	{
		assert(m_RefCount > 0);

		if (--m_RefCount == 0)
		{
			delete this;
		}

		return m_RefCount;
	}

public: // COM
	STDMETHOD(QueryInterface)(const IID& riid, _COM_Outptr_ void** ppvObject) override
	{
		*ppvObject = ([&] {
			void* inner = nullptr;

			(([&]<typename T>() {
				if (__uuidof(T) == riid)
					inner = this;
			}).operator() < InterfaceT > (), ...);

			if (riid == __uuidof(IUnknown))
			{
				inner = this;
			}

			return inner;
			})();

			if (*ppvObject == nullptr)
			{
				return CLASS_E_CLASSNOTAVAILABLE;
			}

			AddRef();

			return S_OK;
	}
};
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <combaseapi.h>

#include "Base.hpp"

// {2A82E6CD-6E11-45F0-B61C-D77592F776B5}
static const GUID IID_ITarget = { 0x2a82e6cd, 0x6e11, 0x45f0, { 0xb6, 0x1c, 0xd7, 0x75, 0x92, 0xf7, 0x76, 0xb5 } };

struct __declspec(uuid("2A82E6CD-6E11-45F0-B61C-D77592F776B5")) ITarget : IUnknown
{
};

struct Implementation : Base<ITarget>
{
};

int main()
{
    void* object;
    Implementation{}.QueryInterface(IID_ITarget, &object);
    delete object;
}
#include <combaseapi.h>

// {2A82E6CD-6E11-45F0-B61C-D77592F776B5}
static const GUID IID_ITarget = { 0x2a82e6cd, 0x6e11, 0x45f0, { 0xb6, 0x1c, 0xd7, 0x75, 0x92, 0xf7, 0x76, 0xb5 } };

struct __declspec(uuid("2A82E6CD-6E11-45F0-B61C-D77592F776B5")) ITarget
{
};

template<typename... Ts>
struct Factory : Ts...
{
    virtual bool process(const IID& filter, void** ppvObject)
    {
        *ppvObject = ([&] {
            void* inner{ nullptr };

            (([&]<typename X> {
                if (__uuidof(X) == filter)
                    inner = new X;
            }).template operator()<Ts>(), ...);

            return inner;
        })();

        if (*ppvObject == nullptr)
        {
            return false;
        }

        return true;
    }
};

struct Implementation : Factory<ITarget>
{
};

int main()
{
    void* object;
    Implementation{}.process(IID_ITarget, &object);
    delete object;

    Factory<ITarget>{}.process(IID_ITarget, &object);
    delete object;
}
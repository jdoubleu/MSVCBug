struct ITarget
{
    static constexpr int ID = 42;
};

template<typename... Ts>
struct Factory
{
    void process(int filter)
    {
        auto ptr = ([&] {
            void* inner{ nullptr };

            (([&]<typename X> {
                if (X::ID == filter)
                    inner = new X;
            }).template operator()<Ts>(), ...);

            return inner;
        })();

        if (ptr != nullptr)
        {
            delete ptr;
        }
    }
};

struct Implementation : Factory<Implementation, ITarget>
{
    static constexpr int ID = 10;
};

int main()
{
    Implementation{}.process(42);
    Factory<ITarget>{}.process(42);
}
struct S
{
    static constexpr int ID = 42;
};

template<typename... Ts>
struct Processor
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

struct T : Processor<T, S>
{
    static constexpr int ID = 10;
};

int main()
{
    T{}.process(42);
    Processor<S>{}.process(42);
}
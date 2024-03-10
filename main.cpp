struct ITarget
{
    static constexpr int ID = 42;
};

template<typename... InterfaceT>
struct Factory : public InterfaceT...
{
	void create(int id, void** ppvObject)
	{
		*ppvObject = ([&] {
			void* inner = nullptr;

			(([&]<typename T>() {
				if (T::ID == id)
					inner = this;
			}).template operator()<InterfaceT>(), ...);

			return inner;
		})();
	}
};

struct Implementation : Factory<ITarget>
{
};

int main()
{
    void* object;
    Implementation{}.create(42, &object);
    delete object;
}
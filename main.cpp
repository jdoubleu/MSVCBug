struct ITarget
{
    static constexpr int ID = 42;
};

// A function template doesn't have the issue. 
// On the other hand, there wouldn't be a `this` (see commends below).
template<typename... InterfaceT>
struct Factory : public InterfaceT...
{
	void create(int id, void** ppvObject)
	{
		// Eliminating the outer IIFE, fixes the issue.
		*ppvObject = ([&] {
			void* inner = nullptr;

			(([&]<typename T>() {
				if (T::ID == id)
					// Changing the assignment e.g. to `new T` doesn't crash.
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
#ifndef DIE_TK_SINGLETON_H_11_28_2015
#define DIE_TK_SINGLETON_H_11_28_2015

template<typename T>
struct singleton_policy_freestore {
	T * instance() const
	{
		static T * instance_ = new T;
		return instance_;
	}
};

template<typename T>
struct singleton_policy_static {
	T * instance() const
	{
		static T instance_;
		return &instance_;
	}
};

template<typename T, template<typename> class P = singleton_policy_static>
class singleton {
public:
	typedef P<T> storage_type;
private:
	storage_type storage;
public:
	explicit singleton(storage_type const & storage = storage_type()): storage(storage) {}
	T & instance() const { return *storage.instance(); }
	T * operator->() { return storage.instance(); }
};

#endif //DIE_TK_SINGLETON_H

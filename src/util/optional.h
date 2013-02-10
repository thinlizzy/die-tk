#ifndef OPTIONAL_H_jjj54444409vgfdgkj234
#define OPTIONAL_H_jjj54444409vgfdgkj234

#include <memory>

template<typename T>
class FreeStorePolicy {
	std::unique_ptr<T> data;
public:
	FreeStorePolicy() {}
	explicit FreeStorePolicy(T const & value): data(new T(value)) {}
	FreeStorePolicy(FreeStorePolicy const & policy): data( policy.data ? new T(*policy.data) : 0 ) {}
	FreeStorePolicy & operator=(FreeStorePolicy const & policy) {
		swap(FreeStorePolicy(policy));
		return *this;
	}
	void swap(FreeStorePolicy && other) throw() { data.swap(other.data); }
	void setValue(T const & value) {
		if( data ) {
			*data = value;
		} else {
			data.reset(new T(value));
		}
	}
	T & operator*() { return *data; }
	T const & operator*() const { return *data; }
	T * operator->() { return data.get(); }
	T const * operator->() const { return data.get(); }
};

template<typename T>
class AutomaticStoragePolicy {
	bool hasValue;
	T data;
public:
	AutomaticStoragePolicy(): hasValue(false) {}
	explicit AutomaticStoragePolicy(T const & value): hasValue(true), data(value) {}
	void swap(AutomaticStoragePolicy && other) throw() {
		std::swap(hasValue,other.hasValue);
		std::swap(data,std::move(other.data));
	}
	void setValue(T const & value) {
		hasValue = true;
		data = value;
	}
	T & operator*() { return data; }
	T const & operator*() const { return data; }
	T * operator->() { return hasValue ? &data : 0; }
	T const * operator->() const { return hasValue ? &data : 0; }
};

// TODO make conversion operators

/*

template<typename T, bool freestore>
struct StoragePolicyTraits {
	template <typename U = T> struct type : FreeStorePolicy<U> { type() {}; type(U const & value): FreeStorePolicy<U>(value) {} };
};

template<typename T>
struct StoragePolicyTraits<T,false> {
	template <typename U = T> struct type : AutomaticStoragePolicy<U> { type() {}; type(U const & value): AutomaticStoragePolicy<U>(value) {} };
};


template<typename T, template<typename> class StoragePolicy>
class basic_optional {
	StoragePolicy<T> storage;
public:
	basic_optional() {}

	basic_optional(T const & value):
		storage(value)
	{}

	void swap(basic_optional && opt) throw()
	{
		storage.swap(std::move(opt.storage));
	}

	basic_optional & operator=(T const & value)
	{
		storage.setValue(value);
		return *this;
	}

	T & operator*() { return *storage; }
	T const & operator*() const { return *storage; }
	T * operator->() { return storage.operator->(); }
	T const * operator->() const { return storage.operator->(); }

	operator void const * () const { return operator->(); }
	//operator bool const () { return operator->() != 0; }
	bool operator!() const { return operator->() == 0; }

	bool operator==(basic_optional const & opt) const
	{
		auto hasValue = bool(operator const void *());
		if( hasValue != bool(opt.operator const void *()) ) return false;
		if( ! hasValue ) return true;

		return operator*() == opt.operator*();
	}

	bool operator!=(basic_optional const & opt) const { return ! (*this == opt); }
};

template<typename T>
class optional: public basic_optional<T, StoragePolicyTraits<T,(sizeof(T) > sizeof(T*))>::template type>  {
public:
	optional() {}

	optional(T const & value):
		basic_optional<T, StoragePolicyTraits<T,(sizeof(T) > sizeof(T*))>::template type>(value)
	{}

	optional & operator=(T const & value)
	{
		basic_optional<T, StoragePolicyTraits<T,(sizeof(T) > sizeof(T*))>::template type>::operator=(value);
		return *this;
	}

	bool operator==(optional const & opt) const
	{
		return basic_optional<T, StoragePolicyTraits<T,(sizeof(T) > sizeof(T*))>::template type>::operator==(opt);
	}

	bool operator!=(optional const & opt) const
	{
		return basic_optional<T, StoragePolicyTraits<T,(sizeof(T) > sizeof(T*))>::template type>::operator!=(opt);
	}
};

*/


template<bool big>
struct StoragePolicyTraits {
	template <class T> using type = FreeStorePolicy<T>;
};

template<>
struct StoragePolicyTraits<false> {
	template <class T> using type = AutomaticStoragePolicy<T>;
};

template<typename T, template<typename> class StoragePolicy = StoragePolicyTraits<(sizeof(T) > sizeof(T*))>::template type>
class optional {
	StoragePolicy<T> storage;
public:
	optional() {}

	optional(T const & value):
		storage(value)
	{}

	void swap(optional && opt) throw()
	{
		storage.swap(std::move(opt.storage));
	}

	optional & operator=(T const & value)
	{
		storage.setValue(value);
		return *this;
	}

	T & operator*() { return *storage; }
	T const & operator*() const { return *storage; }
	T * operator->() { return storage.operator->(); }
	T const * operator->() const { return storage.operator->(); }

	operator void const * () const { return operator->(); }
	bool operator!() const { return operator->() == 0; }

	bool operator==(optional const & opt) const
	{
		auto hasValue = bool(operator const void *());
		if( hasValue != bool(opt.operator const void *()) ) return false;
		if( ! hasValue ) return true;

		return operator*() == opt.operator*();
	}
};


#endif

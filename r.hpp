#ifndef __FF__R_HPP__
#define __FF__R_HPP__

#include <memory>
#include <map>
#include <list>
#include <functional>

namespace ff { namespace r {

template <typename T> class rrr;

template <typename T>
rrr<T>& R() {
	static rrr<T> r;
	return r;
}

template <typename T>
struct behavior {
	explicit behavior(const T& data)
		: data_(data) { }

	~behavior() {
		auto& r = R<T>();
		r.forget(this);
	}

	void lift(const T& data) const {
		data_ = data;
	}

	const T& value() const {
		return data_;
	}

private:
	mutable T data_;
};

template <typename T>
struct rrr {
	void insert(std::shared_ptr<behavior<T>> p, std::function<void(const T&)> func) {
		d_[p.get()].push_front(func);
	}

	void lift(std::shared_ptr<behavior<T>> p, const T& t) {
		auto it = d_.find(p.get());
		if (d_.end() == it) return;
		it->first->lift(t);
		for (auto& fn : it->second)
			fn(t);
	}

	void forget(behavior<T>* p) {
		d_.erase(p);
	}

private:
	typedef std::map<behavior<T>*, std::list<std::function<void(const T&)> > > map_t;
	map_t d_;
};

template <typename T>
std::shared_ptr<behavior<T>> make(const T& data) {
	auto p = std::shared_ptr<behavior<T> >(new behavior<T>(data));
	return p;
}

template <typename T>
void subscribe(std::shared_ptr<behavior<T>> p, std::function<void (const T&)> func) {
	auto& r = R<T>();
	r.insert(p, func);
}

template <typename T>
void next(std::shared_ptr<behavior<T>> p, const T& t) {
	auto& r = R<T>();
	r.lift(p, t);
}
}} // ff::r
#endif // __FF__R_HPP__

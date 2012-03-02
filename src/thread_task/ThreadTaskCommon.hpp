#pragma once

#include <utility>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/cstdint.hpp>
#include <boost/thread.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>

namespace ktl {
	class ThreadTaskImpl {
	public:
		typedef boost::int64_t time_count_type;
		typedef boost::mutex mutex_type;
		typedef mutex_type::scoped_lock scoped_lock_type;
	private:
		sprig::krkr::tjs::object_type control_;
		sprig::krkr::tjs::object_type obj_;
		sprig::krkr::tjs::object_type obj_this_;
		tTJSVariant result_;
		bool is_processing_;
		bool cancelled_;
		boost::shared_ptr<boost::thread> thread_;
		mutable mutex_type mutex_;
		mutable mutex_type access_mutex_;
	private:
		sprig::krkr::tjs::object_type get_obj() const {
			scoped_lock_type lock(access_mutex_);
			return obj_;
		}
		void set_obj(sprig::krkr::tjs::object_type const& v) {
			scoped_lock_type lock(access_mutex_);
			obj_ = v;
		}
		sprig::krkr::tjs::object_type get_obj_this() const {
			scoped_lock_type lock(access_mutex_);
			return obj_this_;
		}
		void set_obj_this(sprig::krkr::tjs::object_type const& v) {
			scoped_lock_type lock(access_mutex_);
			obj_this_ = v;
		}
		tTJSVariant get_result() const {
			scoped_lock_type lock(access_mutex_);
			return result_;
		}
		void set_result(tTJSVariant const& v) {
			scoped_lock_type lock(access_mutex_);
			result_ = v;
		}
		bool get_processing() const {
			scoped_lock_type lock(access_mutex_);
			return is_processing_;
		}
		void set_processing(bool v) {
			scoped_lock_type lock(access_mutex_);
			is_processing_ = v;
		}
	private:
		void callback() {
			set_processing(true);
			set_result(tTJSVariant());
			{
				tTJSVariant result;
				tTJSVariant param[] = {
					tTJSVariant(control_.get(), control_.get())
				};
				tTJSVariant* p_param[] = {
					param + 0
				};
				sprig::krkr::tjs::FuncObjectCall(
					get_obj().get(),
					&result,
					1,
					p_param,
					get_obj_this().get()
					);
				set_result(result);
			}
			set_processing(false);
		}
	public:
		ThreadTaskImpl()
			: cancelled_(false)
			, is_processing_(false)
		{}
		void init_control(sprig::krkr::tjs::object_type control) {
			control_ = control;
		}
		bool start(tTJSVariantClosure const& closure) {
			scoped_lock_type lock(mutex_);
			if (get_processing()) {
				return false;
			}
			set_obj(closure.Object);
			set_obj_this(closure.ObjThis);
			cancelled_ = false;
			thread_ = boost::make_shared<boost::thread>(
				boost::lambda::bind(&ThreadTaskImpl::callback, this)
				);
			return true;
		}
		bool cancel() {
			scoped_lock_type lock(mutex_);
			if (!get_processing()) {
				return false;
			}
			cancelled_ = true;
			return true;
		}
		tTJSVariant result() const {
			scoped_lock_type lock(mutex_);
			return get_result();
		}
		bool is_processing() const {
			scoped_lock_type lock(mutex_);
			return get_processing();
		}
		bool cancelled() const {
			scoped_lock_type lock(mutex_);
			return cancelled_;
		}
		void yield() {
			scoped_lock_type lock(mutex_);
			thread_->yield();
		}
		void sleep(time_count_type milliseconds) {
			scoped_lock_type lock(mutex_);
			thread_->sleep(boost::get_system_time() + boost::posix_time::milliseconds(milliseconds));
		}
	};
}	// namespace ktl

#pragma once

#include "movie_item_base.h"
#include "util/atomic.hpp"
#include "Utilities/mutex.h"

#include <QMovie>
#include <QThread>

#include <memory>
#include <functional>

using icon_callback_t = std::function<void(int)>;
using icon_load_callback_t = std::function<void(int)>;
using size_calc_callback_t = std::function<void()>;

class movie_item_base
{
public:
	movie_item_base();
	virtual ~movie_item_base();

	void init_pointers();

	void set_active(bool active);

	[[nodiscard]] bool get_active() const
	{
		return m_active;
	}

	[[nodiscard]] std::shared_ptr<QMovie> movie() const
	{
		return m_movie;
	}

	void init_movie(const QString& path);

	void call_icon_func() const;
	void set_icon_func(const icon_callback_t& func);

	void call_icon_load_func(int index);
	void set_icon_load_func(const icon_load_callback_t& func);

	void call_size_calc_func();
	void set_size_calc_func(const size_calc_callback_t& func);

	void wait_for_icon_loading(bool abort);
	void wait_for_size_on_disk_loading(bool abort);

	bool icon_loading() const
	{
		return m_icon_loading;
	}

	bool size_on_disk_loading() const
	{
		return m_size_on_disk_loading;
	}

	[[nodiscard]] std::shared_ptr<atomic_t<bool>> icon_loading_aborted() const
	{
		return m_icon_loading_aborted;
	}

	[[nodiscard]] std::shared_ptr<atomic_t<bool>> size_on_disk_loading_aborted() const
	{
		return m_size_on_disk_loading_aborted;
	}

	shared_mutex pixmap_mutex;

protected:
	std::shared_ptr<QMovie> m_movie;

private:
	std::unique_ptr<QThread> m_icon_load_thread;
	std::unique_ptr<QThread> m_size_calc_thread;
	bool m_active = false;
	atomic_t<bool> m_size_on_disk_loading = false;
	atomic_t<bool> m_icon_loading = false;
	size_calc_callback_t m_size_calc_callback = nullptr;
	icon_load_callback_t m_icon_load_callback = nullptr;
	icon_callback_t m_icon_callback = nullptr;

	std::shared_ptr<atomic_t<bool>> m_icon_loading_aborted;
	std::shared_ptr<atomic_t<bool>> m_size_on_disk_loading_aborted;
};

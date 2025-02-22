#pragma once

#include <chrono>

namespace Sen::Kernel {

	class Clock {

		private:

			std::chrono::time_point<std::chrono::steady_clock> start_time_;

			int64_t duration_;

			bool running_;
		
		public:

			explicit Clock(
			) : start_time_{}, duration_{ 0 }, running_{ false } {
			}

			~Clock(

			) = default;

			inline auto start_safe(

			) -> void
			{
				if (!running_) {
					start_time_ = std::chrono::steady_clock::now();
					running_ = true;
				}
			}

			inline auto stop_safe(

			) -> void
			{
				if (running_) {
					const auto end_time = std::chrono::steady_clock::now();
					duration_ += std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time_).count();
					running_ = false;
				}
			}

			inline auto reset(

			) -> void
			{
				duration_ = 0;
				running_ = false;
			}

			inline auto duration(

			) const -> int64_t
			{
				return duration_;
			}

			inline auto duration_as_seconds(

			) const -> double
			{
				return static_cast<double>(duration_)  / 1000.0;
			}

			inline auto is_started(

			) const -> bool
			{
				return running_;
			}

			inline auto is_stopped(

			) const -> bool
			{
				return !running_;
			}

			friend auto operator << (
				std::ostream& os,
				const Clock& other
			) -> std::ostream& {
				os << "Clock(" << other.start_time_.time_since_epoch().count() << ", " << other.duration_ << ", " << other.running_ << ")";
				return os;
			}
	};

}
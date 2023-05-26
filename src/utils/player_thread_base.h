#ifndef SRC_UTILS_PLAYER_THREAD_HOLDER_H_
#define SRC_UTILS_PLAYER_THREAD_HOLDER_H_

#include "declarations.hpp"
#include <vector>

template <typename Derived>
class PlayerThreadHolder {
	public:
		PlayerThreadHolder() { }

		void start(int numThreads) {
			setState(THREAD_STATE_RUNNING);
			threads.resize(numThreads);

			for (size_t i = 0; i < threads.size(); ++i) {
				threads[i] = std::thread(&Derived::threadMain, static_cast<Derived*>(this), i);
			}
		}

		void stop() {
			setState(THREAD_STATE_CLOSING);
		}

		void join() {
			for (auto &thread : threads) {
				if (thread.joinable()) {
					thread.join();
				}
			}
		}

	protected:
		void setState(ThreadState newState) {
			threadState.store(newState, std::memory_order_relaxed);
		}

		ThreadState getState() const {
			return threadState.load(std::memory_order_relaxed);
		}

	private:
		std::atomic<ThreadState> threadState { THREAD_STATE_TERMINATED };
		std::vector<std::thread> threads;
};

#endif // SRC_UTILS_PLAYER_THREAD_HOLDER_H_
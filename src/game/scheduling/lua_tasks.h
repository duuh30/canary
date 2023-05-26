/**
 * Canary - A free and open-source MMORPG server emulator
 * Copyright (©) 2019-2022 OpenTibiaBR <opentibiabr@outlook.com>
 * Repository: https://github.com/opentibiabr/canary
 * License: https://github.com/opentibiabr/canary/blob/main/LICENSE
 * Contributors: https://github.com/opentibiabr/canary/graphs/contributors
 * Website: https://docs.opentibiabr.com/
 */

#ifndef SRC_GAME_SCHEDULING_LUA_TASKS_H_
#define SRC_GAME_SCHEDULING_LUA_TASKS_H_

#include "tasks.h"

class LuaTask : public Task {
	public:
		explicit LuaTask(std::function<void(void)> &&f) :
            Task(std::move(f)) { }

        LuaTask(uint32_t ms, std::function<void(void)> &&f) :
            Task(ms, std::move(f)) { }
	protected:
		std::chrono::system_clock::time_point expiration = SYSTEM_TIME_ZERO;
	private:
		// Expiration has another meaning for scheduler tasks,
		// then it is the time the task should be added to the
		// dispatcher
		std::function<void(void)> func;
};

class LuaDispatcher : public ThreadHolder<LuaDispatcher> {
	public:
		LuaDispatcher() = default;

		LuaDispatcher(const LuaDispatcher &) = delete;
		void operator=(const LuaDispatcher &) = delete;

		static LuaDispatcher &getInstance() {
			// Guaranteed to be destroyed
			static LuaDispatcher instance;
			// Instantiated on first use
			return instance;
		}

		void addTask(Task* task, bool push_front = false);

		void shutdown();

		uint64_t getDispatcherCycle() const {
			return dispatcherCycle;
		}

		void threadMain();

	private:
		std::mutex taskLock;
		std::condition_variable taskSignal;

		std::list<Task*> taskList;
		uint64_t dispatcherCycle = 0;
};

constexpr auto g_luaDispatcher = &LuaDispatcher::getInstance;

#endif // SRC_GAME_SCHEDULING_LUA_TASKS_H_

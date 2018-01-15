#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include "Task.h"
#include <queue>
#include <pthread.h>

using namespace std;
class ThreadPool {
	public:
		/**
		 * C'tor of thread pool, accepting number of threads in pool
		 */
		ThreadPool(int threadsNum);

		/**
		 * Adds a new task to queue
		 */
		void addTask(Task *task);

		/**
		 * Closes all threads in pool
		 */
		void terminate();

		/**
		 * D'tor
		 */
		virtual ~ThreadPool();

	private:
		//queue of thread pool
		queue<Task *> tasksQueue;

		//pointer to dynamic thread array
		pthread_t* threads;

		//flag to mark if pool stopped working
		bool stopped;

		//lock for queue
		pthread_mutex_t lock;

		/**
		 * A static wrapper that calls executeTasks()
		 */
		static void *execute(void *arg);

		/**
		 * The function that each thread runs
		 */
		void executeTasks();
};


#endif /* THREADPOOL_H_ */

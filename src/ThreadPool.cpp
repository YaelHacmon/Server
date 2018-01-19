#include "ThreadPool.h"
#include <unistd.h>

ThreadPool::ThreadPool(int threadsNum): stopped(false), num_threads(threadsNum) {
	threads = new pthread_t[threadsNum];
	for (int i = 0; i < threadsNum; i++) {
		pthread_create(threads + i, NULL, execute,this);
	}
	pthread_mutex_init(&lock, NULL);
	pthread_mutex_init(&lockPush, NULL);
}


void* ThreadPool::execute(void *arg) {
	ThreadPool *pool = (ThreadPool *)arg;
	pool->executeTasks();

	//must return something - non-void
	return NULL;
}


void ThreadPool::addTask(Task *task) {
	pthread_mutex_lock(&lockPush);
	tasksQueue.push(task);
	pthread_mutex_unlock(&lockPush);
}


void ThreadPool::executeTasks() {
	//infinite while loop for checking task queue
	while (!stopped) {
		//lock and check if a task waits
		pthread_mutex_lock(&lock);
		if (!tasksQueue.empty()) {

			//if so - remove task from queue
			Task* task = tasksQueue.front();
			tasksQueue.pop();

			pthread_mutex_unlock(&lock);

			//execute task
			task->execute();

			//make sure to release memory allocated dynamically
			delete task;
		}
		else {
			pthread_mutex_unlock(&lock);
			sleep(1);
		}
	}
}


void ThreadPool::terminate() {
	pthread_mutex_destroy(&lock);

	//stop idle threads
	stopped = true;

	//kill all threads - release from memory and stop active ones
	for (int i = 0; i < num_threads; i++) {
		pthread_cancel(*(threads + i));
	}
}


ThreadPool::~ThreadPool() {
	//delete existing tasks in queue
	while (!tasksQueue.empty()) {
		//if so - remove task from queue
		Task* t = tasksQueue.front();
		tasksQueue.pop();
		delete t;
	}

	//delete thread array
	delete[] threads;
}

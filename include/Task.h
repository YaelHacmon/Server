#ifndef TASK_H_
#define TASK_H_

/**
 * General implementation of the task interface for the Threadpool pattern
 */
class Task {
	public:
		/**
		 * C'tor accepting pointer to function and arguments for function
		 */
		Task(void * (*func)(void *arg), void* arg);

		/**
		 * Executes the function
		 */
		virtual void execute();

		/**
		 * D'tor
		 */
		virtual ~Task() {}

	protected:
		//pointer to function to execute
		void * (*func)(void *arg);
		//arguments for function
		void *arg;
};


#endif /* TASK_H_ */

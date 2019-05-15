#include <dsps/Utils.h>

#include <algorithm>

#include <dsps/Channel.h>
#include <dsps/Task.h>

void DSP::processing(std::list<Task*> sourceTask, std::list<Task*> outputChannel, const std::uint64_t N) {
    // Linearisation of DAG
    bool finished = false;
    auto linearDAG = dagLinearisation(sourceTask);

    do {
        for (auto it = linearDAG.begin(); it != linearDAG.end(); ++it) {
            Task *task = *it;
            // If the task is a source task, we compute only once
            if (sourceTask.end() != std::find(sourceTask.begin(), sourceTask.end(), task)) {
                task->compute(N);
            }
            // Else we compute the task until it wasn't ready
            else {
                while (task->isReady(N)) {
                    task->compute(N);
                }
            }
        }

        // Check if the DAG was completed
        finished = true;
        for (auto it = outputChannel.begin(); it != outputChannel.end() && finished; ++it) {
            auto task = *it;
            if (!task->hasFinished(N)) {
                finished = false;
            }
        }
    } while (!finished);
}

std::list<Task*> DSP::dagLinearisation(std::list<Task*> sourceTask) {
    std::list<Task*> linearisation;

    // For all source task we add the next task to lineatisation
    do {
        // Add the front element to linearisation
        Task *currentTask = sourceTask.front();
        sourceTask.pop_front();
        linearisation.push_back(currentTask);

        // Add the next task to source list
        for (std::size_t i = 0; i < currentTask->countNextTask(); ++i) {
            Task *nextTask = currentTask->getNextTask(i);

            // If the task isn't in the linearisation
            if (nextTask != nullptr && sourceTask.end() == std::find_if(sourceTask.begin(), sourceTask.end(), [nextTask](Task *task){
                return task == nextTask;
            }) ) {
                sourceTask.push_back(nextTask);
            }
        }
    } while(sourceTask.size() != 0);

    return linearisation;
}

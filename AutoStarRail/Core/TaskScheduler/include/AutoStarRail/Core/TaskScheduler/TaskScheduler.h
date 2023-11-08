#ifndef ASR_CORE_TASKSCHEDULER_H
#define ASR_CORE_TASKSCHEDULER_H

#include "AutoStarRail/IAsrBase.h"
#include <chrono>
#include <unistd.h>
#include <vector>

#include <AutoStarRail/AsrPtr.hpp>
#include <AutoStarRail/PluginInterface/IAsrTask.h>

ASR_NS_BEGIN

namespace Core
{
    class TaskScheduler
    {
    private:
        struct SchedulingUnit
        {
            std::chrono::time_point<std::chrono::system_clock> next_run_time;
            AsrPtr<IAsrTask>                                   p_task;

            bool operator==(const SchedulingUnit& t);
        };

        SchedulingUnit              current_task;
        std::vector<SchedulingUnit> task_queue;

        void AddTask(SchedulingUnit Task);
        void DelTask(SchedulingUnit Task);

        void RunTaskQueue();

    public:
        // for UI
        IAsrReadOnlyString GetStatusJSON();
        // for proxy
        AsrPtr<IAsrTask> GetCurrentTask();
    };
} // namespace Core

ASR_NS_END

#endif // ASR_CORE_TASKSCHEDULER_H
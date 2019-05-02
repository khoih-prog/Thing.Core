#include "AppTaskScheduler.h"
#include "IoC.h"

namespace Thing
{
	namespace Core
	{
		AppTaskScheduler::AppTaskScheduler(IAppContainer& AppContainer) : app(&AppContainer)
		{
			AppContainer.AddListener(this);
		}

		AppTaskScheduler::~AppTaskScheduler()
		{
		}

		bool AppTaskScheduler::OnLoop()
		{
			unsigned long currentTimestamp = Hardware->Millis();
			for (std::list<ScheduledTask>::iterator i = tasks.begin(); i != tasks.end();)
			{
				if ((bool)(i->status & AppTaskStatus::ShouldDelete))
				{
					i = tasks.erase(i);
					continue;
				}

				if (currentTimestamp - i->scheduledTimestamp >= i->interval)
				{
					i->runnable->Run();
					if ((bool)(i->status & AppTaskStatus::Once))
						i = tasks.erase(i);
					else
					{
						i->scheduledTimestamp = currentTimestamp;
						++i;
					}
					continue;
				}
				++i;
			}
			return false;
		}

		void AppTaskScheduler::AttachOnce(unsigned long milli, Thing::Core::IRunnable* runnable)
		{
			ScheduledTask task;
			task.status = AppTaskStatus::Once;
			task.scheduledTimestamp = Hardware->Millis();
			task.interval = milli;
			task.runnable = runnable;
			tasks.push_back(task);
		}

		void AppTaskScheduler::AttachOnce(unsigned long milli, Thing::Core::IRunnable& runnable)
		{
			AttachOnce(milli, &runnable);
		}

		void AppTaskScheduler::AttachPeriodic(unsigned long milli, Thing::Core::IRunnable* runnable)
		{
			ScheduledTask task;
			task.status = AppTaskStatus::Periodic;
			task.scheduledTimestamp = Hardware->Millis();
			task.interval = milli;
			task.runnable = runnable;
			tasks.push_back(task);
		}

		void AppTaskScheduler::AttachPeriodic(unsigned long milli, Thing::Core::IRunnable& runnable)
		{
			AttachPeriodic(milli, &runnable);
		}

		void AppTaskScheduler::Detach(Thing::Core::IRunnable* runnable)
		{
			for (std::list<ScheduledTask>::iterator i = tasks.begin(); i != tasks.end(); ++i)
				if (i->runnable == runnable)
				{
					i->status = i->status | AppTaskStatus::ShouldDelete;
					return;
				}
		}

		void AppTaskScheduler::Detach(Thing::Core::IRunnable& runnable)
		{
			Detach(&runnable);
		}
	}
}
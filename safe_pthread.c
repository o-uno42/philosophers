#include "philo.h"

void	s_p_create(pthread_t *thread, void *(*foo)(void *), void *data)
{
	if (pthread_create(thread, NULL, foo, data) == -1)
		ft_error("Pthread create error.");
}

void	safe_pthread_join(pthread_t *thread)
{
	if (pthread_join(*thread, NULL) == -1)
		ft_error("Pthread join error.");
}

void	safe_pthread_detach(pthread_t *thread)
{
	if (pthread_detach(*thread) == -1)
		ft_error("Pthread detach error.");
}

void	wait_all_threads(t_rules *rules)
{
	safe_mutex_lock(&rules->rules_mutex);
	while (!rules->threads_ready)
	{
		safe_mutex_unlock(&rules->rules_mutex);
		usleep(100);
		safe_mutex_lock(&rules->rules_mutex);
	}
	safe_mutex_unlock(&rules->rules_mutex);
}

void	desync_philos(t_philo *philo)
{
	if (philo->rules->nb_philos % 2 == 0)
	{
		if (philo->philo_id % 2 == 0)
			new_usleep(30000, philo->rules);
	}
	else
	{
		if (philo->philo_id % 2 == 0)
			thinking(philo, true);
	}
}

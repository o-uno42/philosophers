#include "philo.h"

void	assign_forks(t_philo *philo, t_fork *forks, int i)
{
	if (philo->philo_id % 2 == 0)
	{
		philo->fork_1 = &forks[i];
		philo->fork_2 = &forks[(i + 1) % philo->rules->nb_philos];
	}
	else
	{
		philo->fork_1 = &forks[(i + 1) % philo->rules->nb_philos];
		philo->fork_2 = &forks[i];
	}
}

bool	philo_died(t_philo *philo)
{
	long	time;
	long	philo_full;
	long	time_to_die;
	long	last_meal_time;

	safe_mutex_lock(&philo->race_mutex);
	philo_full = philo->full;
	safe_mutex_unlock(&philo->race_mutex);
	if (philo_full)
		return (false);
	safe_mutex_lock(&philo->race_mutex);
	last_meal_time = philo->last_meal_time;
	safe_mutex_unlock(&philo->race_mutex);
	time = gettime(1000) - last_meal_time;
	time_to_die = philo->rules->time_to_die / 1000;
	if (time > time_to_die)
		return (true);
	return (false);
}

bool	threads_running(pthread_mutex_t *rules, long *nb_threads, long nb_philo)
{
	bool	all;

	safe_mutex_lock(rules);
	if (*nb_threads == nb_philo)
		all = true;
	safe_mutex_unlock(rules);
	return (all);
}

void	*monitoring(void *data)
{
	int		i;
	t_rules	*rules;

	i = 0;
	rules = (t_rules *)data;
	while (!threads_running(&rules->rules_mutex, \
		&rules->nb_threads, rules->nb_philos))
		;
	while (!end_simulation(rules))
	{
		i = 0;
		while (i < rules->nb_philos && !end_simulation(rules))
		{
			if (philo_died(rules->philos + i))
			{
				safe_mutex_lock(&rules->rules_mutex);
				rules->end_simulation = true;
				safe_mutex_unlock(&rules->rules_mutex);
				print_action(DEAD, rules->philos + i);
			}
			i++;
		}
	}
	return (NULL);
}

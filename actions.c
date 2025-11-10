#include "philo.h"

void	thinking(t_philo *philo, bool pre_simulation)
{
	long	t_eat;
	long	t_sleep;
	long	t_think;

	if (!pre_simulation)
		print_action(THINKING, philo);
	if (philo->rules->nb_philos % 2 == 0)
		return ;
	t_eat = philo->rules->time_to_eat;
	t_sleep = philo->rules->time_to_sleep;
	t_think = t_eat * 2 - t_sleep;
	if (t_think < 0)
		t_think = 0;
	new_usleep(t_think * 0.42, philo->rules);
}

void	eating(t_philo *philo)
{
	safe_mutex_lock(&philo->fork_1->fork);
	print_action(TAKES_FORK_1, philo);
	safe_mutex_lock(&philo->fork_2->fork);
	print_action(TAKES_FORK_2, philo);
	safe_mutex_lock(&philo->race_mutex);
	philo->last_meal_time = gettime(1000);
	safe_mutex_unlock(&philo->race_mutex);
	philo->n_meals++;
	print_action(EATING, philo);
	new_usleep(philo->rules->time_to_eat, philo->rules);
	if (philo->rules->max_meals > 0 \
		&& (philo->n_meals == philo->rules->max_meals))
	{
		safe_mutex_lock(&philo->race_mutex);
		philo->full = true;
		safe_mutex_unlock(&philo->race_mutex);
	}
	safe_mutex_unlock(&philo->fork_1->fork);
	safe_mutex_unlock(&philo->fork_2->fork);
}

void	*simulation(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	wait_all_threads(philo->rules);
	safe_mutex_lock(&philo->race_mutex);
	philo->last_meal_time = gettime(1000);
	safe_mutex_unlock(&philo->race_mutex);
	safe_mutex_lock(&philo->rules->rules_mutex);
	philo->rules->nb_threads++;
	safe_mutex_unlock(&philo->rules->rules_mutex);
	desync_philos(philo);
	while (!end_simulation(philo->rules))
	{
		if (philo->full)
			break ;
		eating(philo);
		print_action(SLEEPING, philo);
		new_usleep(philo->rules->time_to_sleep, philo->rules);
		thinking(philo, false);
	}
	return (NULL);
}

void	start_simulation(t_rules *rules)
{
	int		i;

	i = -1;
	if (rules->max_meals == 0)
		return ;
	else if (rules->nb_philos == 1)
		s_p_create(&rules->philos[0].thread_id, one_philo, &rules->philos[0]);
	else
	{
		while (++i < rules->nb_philos)
			s_p_create(&rules->philos[i].thread_id, simulation, \
				&rules->philos[i]);
	}
	s_p_create(&rules->monitor, monitoring, rules);
	rules->start_simulation = gettime(1000);
	safe_mutex_lock(&rules->rules_mutex);
	rules->threads_ready = true;
	safe_mutex_unlock(&rules->rules_mutex);
	i = -1;
	while (++i < rules->nb_philos)
		safe_pthread_join(&rules->philos[i].thread_id);
	safe_mutex_lock(&rules->rules_mutex);
	rules->end_simulation = true;
	safe_mutex_unlock(&rules->rules_mutex);
	safe_pthread_join(&rules->monitor);
}

bool	end_simulation(t_rules *rules)
{
	bool	i;

	safe_mutex_lock(&rules->rules_mutex);
	i = rules->end_simulation;
	safe_mutex_unlock(&rules->rules_mutex);
	return (i);
}

#include "philo.h"

void	print_action(t_action action, t_philo *philo)
{
	long	time;

	if (philo->full)
		return ;
	time = gettime(1000) - philo->rules->start_simulation;
	safe_mutex_lock(&philo->rules->print_mutex);
	if ((action == TAKES_FORK_1 || action == TAKES_FORK_2) \
		&& !end_simulation(philo->rules))
		printf(CYN "%ld  %d has taken a fork\n" RST, time, philo->philo_id);
	else if (action == EATING && !end_simulation(philo->rules))
		printf(YEL "%ld  %d is eating\n" RST, time, philo->philo_id);
	else if (action == SLEEPING && !end_simulation(philo->rules))
		printf(BLU "%ld  %d is sleeping\n" RST, time, philo->philo_id);
	else if (action == THINKING && !end_simulation(philo->rules))
		printf("%ld  %d is thinking\n", time, philo->philo_id);
	else if (action == DEAD)
		printf(RED "%ld  %d died\n" RST, time, philo->philo_id);
	safe_mutex_unlock(&philo->rules->print_mutex);
}

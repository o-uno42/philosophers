#include "philo.h"

void	ft_parsing(t_rules *rules, char **argv)
{
	rules->nb_philos = new_atol(argv[1]);
	rules->time_to_die = new_atol(argv[2]) * 1e3;
	rules->time_to_eat = new_atol(argv[3]) * 1e3;
	rules->time_to_sleep = new_atol(argv[4]) * 1e3;
	if (rules->time_to_die < 6e4 || rules->time_to_eat < 6e4 \
		|| rules->time_to_sleep < 6e4)
		ft_error("Timestamps need to be major than 60 milliseconds.");
	if (argv[5])
		rules->max_meals = new_atol(argv[5]);
	else
		rules->max_meals = -1;
}

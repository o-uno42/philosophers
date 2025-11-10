#include "philo.h"

void	*one_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_all_threads(philo->rules);
	safe_mutex_lock(&philo->race_mutex);
	philo->last_meal_time = gettime(1000);
	safe_mutex_unlock(&philo->race_mutex);
	safe_mutex_lock(&philo->rules->rules_mutex);
	philo->rules->nb_threads++;
	safe_mutex_unlock(&philo->rules->rules_mutex);
	print_action(TAKES_FORK_1, philo);
	while (!end_simulation(philo->rules))
		usleep(200);
	return (NULL);
}

static void	clean(t_rules *rules)
{
	t_philo	*philo;
	int		i;

	i = 0;
	while (i < rules->nb_philos)
	{
		philo = rules->philos + i;
		safe_mutex_destroy(&philo->race_mutex);
		i++;
	}
	safe_mutex_destroy(&rules->print_mutex);
	safe_mutex_destroy(&rules->rules_mutex);
	free(rules->forks);
	free(rules->philos);
	return ;
}

static void	philo_init(t_rules *rules)
{
	int		i;
	t_philo	*philo;

	i = 0;
	while (i < rules->nb_philos)
	{
		philo = rules->philos + i;
		philo->philo_id = i + 1;
		philo->full = false;
		philo->n_meals = 0;
		philo->rules = rules;
		safe_mutex_init(&philo->race_mutex);
		assign_forks(philo, rules->forks, i);
		i++;
	}
	return ;
}

static void	rules_init(t_rules *rules)
{
	int	i;

	i = 0;
	rules->end_simulation = false;
	rules->threads_ready = false;
	rules->nb_threads = 0;
	rules->philos = safe_malloc(sizeof(t_philo) * rules->nb_philos);
	rules->forks = safe_malloc(sizeof(t_fork) * rules->nb_philos);
	safe_mutex_init(&rules->rules_mutex);
	safe_mutex_init(&rules->print_mutex);
	while (i < rules->nb_philos)
	{
		safe_mutex_init(&rules->forks[i].fork);
		rules->forks[i].fork_id = i;
		i++;
	}
	philo_init(rules);
	return ;
}

int	main(int argc, char **argv)
{
	t_rules	rules;

	if (argc != 5 && argc != 6)
	{
		printf(RED "Wrong input.\n");
		printf("(1) nbr philos\n(2) time to die\n(3) time to eat\n");
		printf("(4) time to sleep\n");
		ft_error("(5) (optional) nbr of times each philo must eat\n" RST);
	}
	ft_parsing(&rules, argv);
	rules_init(&rules);
	start_simulation(&rules);
	clean(&rules);
	return (0);
}

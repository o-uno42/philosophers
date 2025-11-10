#include "philo.h"

void	ft_error(char *str)
{
	printf("%s\n", str);
	exit(EXIT_FAILURE);
}

long long	gettime(int sec)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
	{
		ft_error("gettimeofthedayfailed");
		return (0);
	}
	if (sec == 1)
		return (tv.tv_sec + (tv.tv_usec / 1000000));
	else if (sec == 1000)
		return (tv.tv_sec * 1000 + (tv.tv_usec / 1000));
	else if (sec == 1000000)
		return ((tv.tv_sec * 1000000) + tv.tv_usec);
	else
	{
		ft_error("gettime failed");
		return (0);
	}
}

void	new_usleep(long usec, t_rules *rules)
{
	long	start;
	long	time;
	long	remaining;

	rules = rules;
	start = gettime(1000000);
	while ((gettime(1000000) - start) < usec)
	{
		if (end_simulation(rules))
			break ;
		time = gettime(1000000) - start;
		remaining = usec - time;
		if (remaining > 1000)
			usleep(remaining / 2);
		else
		{
			while ((gettime(1000000) - start) < usec)
				;
		}
	}
}

static char	*valid_input(const char *str)
{
	int		i;
	char	*nb;

	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
		ft_error("Negative numbers are not valid.");
	if (!(str[i] >= '0' && str[i] <= '9'))
		ft_error("Your input has a non valid number.");
	nb = strdup(str);
	if (!nb)
		ft_error("Memory allocation failed.");
	return (nb);
}

long	new_atol(const char *str)
{
	long	res;
	int		i;
	char	*tmp;

	res = 0;
	i = 0;
	tmp = valid_input(str);
	while (tmp[i] >= '0' && tmp[i] <= '9')
	{
		res = res * 10 + (tmp[i] - '0');
		i++;
	}
	if (res > INT_MAX)
	{
		free(tmp);
		ft_error("INT_MAX is the limit.");
	}
	free(tmp);
	return (res);
}

#include "philo.h"

void	*safe_malloc(size_t size)
{
	void	*res;

	res = malloc(size);
	if (!res)
		ft_error("Malloc error.");
	return (res);
}

void	safe_mutex_lock(pthread_mutex_t *mutex)
{
	if (pthread_mutex_lock(mutex) == -1)
		ft_error("Pthread mutex lock error.");
}

void	safe_mutex_unlock(pthread_mutex_t *mutex)
{
	if (pthread_mutex_unlock(mutex) == -1)
		ft_error("Pthread mutex unlock error.");
}

void	safe_mutex_init(pthread_mutex_t *mutex)
{
	if (pthread_mutex_init(mutex, NULL) == -1)
		ft_error("Pthread mutex init error.");
}

void	safe_mutex_destroy(pthread_mutex_t *mutex)
{
	if (pthread_mutex_destroy(mutex) == -1)
		ft_error("Pthread mutex destroy error.");
}

#include <sys/time.h>
#include <pthread.h> //pthread and mutex
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define RST  "\x1B[0m"
#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"
#define CYN  "\x1B[36m"
#define WHT  "\x1B[37m"

typedef enum e_action
{
	EATING,
	SLEEPING,
	THINKING,
	TAKES_FORK_1,
	TAKES_FORK_2,
	DEAD,
}			t_action;

typedef struct s_forks	t_forks;
typedef struct s_philo	t_philo;
typedef struct s_rules	t_rules;

typedef struct s_fork
{
	int				fork_id;
	pthread_mutex_t	fork;
	t_rules			*rules;
}					t_fork;

typedef struct s_philo
{
	pthread_t		thread_id;
	int				philo_id;
	long			n_meals;
	bool			full;
	long			last_meal_time;
	int				is_dead;
	t_fork			*fork_1;
	t_fork			*fork_2;
	t_rules			*rules;
	t_action		action;
	pthread_mutex_t	race_mutex;
}					t_philo;

typedef struct s_rules
{
	pthread_t		monitor;
	long			nb_philos;
	long			nb_threads;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			max_meals;
	long			start_simulation;
	bool			end_simulation;
	bool			threads_ready;
	pthread_mutex_t	rules_mutex;
	pthread_mutex_t	print_mutex;
	t_philo			*philos;
	t_fork			*forks;
}					t_rules;

void		assign_forks(t_philo *philo, t_fork *forks, int i);
void		start_simulation(t_rules *rules);
bool		end_simulation(t_rules *rules);
void		*one_philo(void *arg);
void		thinking(t_philo *philo, bool pre_simulation);
bool		threads_running(pthread_mutex_t *rules, \
	long *nb_threads, long nb_philo);
void		wait_all_threads(t_rules *rules);
void		*monitoring(void *data);
void		safe_mutex_lock(pthread_mutex_t *mutex);
void		safe_mutex_unlock(pthread_mutex_t *mutex);
void		safe_mutex_init(pthread_mutex_t *mutex);
void		safe_mutex_destroy(pthread_mutex_t *mutex);
void		s_p_create(pthread_t *thread, void *(*foo)(void *), void *data);
void		safe_pthread_join(pthread_t *thread);
void		safe_pthread_detach(pthread_t *thread);
long long	gettime(int sec);
void		new_usleep(long sec, t_rules *rules);
void		desync_philos(t_philo *philo);
void		ft_parsing(t_rules *rules, char **argv);
void		ft_error(char *str);
void		*safe_malloc(size_t size);
long		new_atol(const char *str);
void		print_action(t_action action, t_philo *philo);

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 19:16:29 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/03 15:35:51 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PHILOSOPHERS_H
# define FT_PHILOSOPHERS_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>
# include <string.h>

# define UNLIMITED_LOGS -1

# define RED "\001\e[31m\002"
# define BLUE "\001\e[94m\002"
# define GREEN "\001\e[32m\002"
# define GRAY "\001\e[90m\002"
# define YELLOW "\001\e[93m\002"
# define PURPLE "\001\e[35m\002"
# define ORANGE "\001\e[38;5;202m\002"

enum e_error
{
	E_SUCCESS = 0,
	E_MALLOC = 1,
	E_INIT_MUTEX = 1 << 1,
	E_INIT_THREADS = 1 << 2,
	E_JOIN_THREADS = 1 << 3,
	E_DESTROY_MUTEX = 1 << 4
};

typedef struct s_protected_int
{
	int				content;
	pthread_mutex_t	mutex;
}	t_protected_int;

typedef struct s_protected_uint
{
	unsigned int	content;
	pthread_mutex_t	mutex;
}	t_protected_uint;

typedef struct s_protected_size_t
{
	size_t			content;
	pthread_mutex_t	mutex;
}	t_protected_size_t;

typedef struct s_global_data
{
	unsigned int		number_of_philosophers;
	useconds_t			time_to_die;
	useconds_t			time_to_eat;
	useconds_t			time_to_sleep;
	long				number_of_times_each_philosopher_must_eat;
	t_protected_int		*forks;
	t_protected_int		any_death;
	t_protected_size_t	start_time;
	t_protected_uint	finished;
	pthread_mutex_t		write_mutex;
	t_protected_size_t	*last_eats;
}	t_global_data;

typedef struct s_philo_data
{
	unsigned int		number;
	useconds_t			time_to_die;
	useconds_t			time_to_eat;
	useconds_t			time_to_sleep;
	long				number_of_times_each_philosopher_must_eat;
	size_t				initial_time;
	t_protected_size_t	*last_eat;
	t_protected_int		*first_fork;
	t_protected_int		*second_fork;
	pthread_mutex_t		*write_mutex;
	t_protected_int		*any_death;
	t_protected_size_t	*start_time;
	t_protected_uint	*finished;
}	t_philo_data;

int				check_input(int argc, char **argv);
void			save_data(int argc, char **argv, t_global_data *data);
size_t			get_current_time_ms(void);
int				wait_ms(t_philo_data *data, useconds_t msec);
int				init_all(t_global_data *g_data, pthread_t **philosophers,
					t_philo_data **philos_data, pthread_mutex_t ***all_mutex);
void			fill_philo_data(t_philo_data *p_data, int i,
					t_global_data *g_data);
int				manage_philosophers(t_global_data *g_data);
void			*philo_routine(void *arg);
void			*monitorize(void *arg);
int				set_int(t_protected_int *p_var, int new_value);
unsigned int	set_uint(t_protected_uint *p_var, unsigned int new_value);
void			add_one_uint(t_protected_uint *p_var);
size_t			set_size_t(t_protected_size_t *p_var, size_t new_value);
int				get_int(t_protected_int *p_var);
unsigned int	get_uint(t_protected_uint *p_var);
size_t			get_size_t(t_protected_size_t *p_var);

#endif

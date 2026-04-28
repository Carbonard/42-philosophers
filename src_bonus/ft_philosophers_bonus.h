/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers_bonus.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 19:16:29 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/28 17:36:56 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PHILOSOPHERS_BONUS_H
# define FT_PHILOSOPHERS_BONUS_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <semaphore.h>
# include <fcntl.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <limits.h>

# define UNLIMITED_LOGS -1
# define MAX_SEM_NAME 100

# define RED "\001\e[31m\002"
# define BLUE "\001\e[94m\002"
# define GREEN "\001\e[32m\002"
# define GRAY "\001\e[90m\002"
# define YELLOW "\001\e[93m\002"
# define PURPLE "\001\e[35m\002"
# define ORANGE "\001\e[38;5;202m\002"

enum e_error
{
	SUCCESS,
	ERROR_MALLOC,
	ERROR_SEM,
	ERROR_INIT_THREADS,
	ERROR_JOIN_THREADS,
};

typedef struct s_global_data
{
	unsigned int	number_of_philosophers;
	unsigned int	philo_number;
	useconds_t		time_to_die;
	useconds_t		time_to_eat;
	useconds_t		time_to_sleep;
	long			number_of_times_each_philosopher_must_eat;
	long			meals;
	size_t			initial_time;
	int				is_dead;
	size_t			last_eat;
	sem_t			*forks_sem;
	sem_t			*write_sem;
	sem_t			*death_sem;
	sem_t			*data_sem;
	sem_t			*control_sem;
	sem_t			*iteration_sem;
}	t_global_data;

int		check_input(int argc, char **argv);
void	save_data(int argc, char **argv, t_global_data *data);
size_t	get_current_time_ms(void);
int		manage_philosophers(t_global_data *g_data);
void	create_death_monitor(
			t_global_data *data, pthread_t *monitor, pthread_t *sim_monitor);
void	philo_routine(t_global_data *data);
sem_t	*create_sem(char *name, int num, int id);
int		display_msg(t_global_data *data, const char *msg, char *color);
void	display_death(t_global_data *data);
void	close_semaphores(t_global_data *data);
void	*monitorize_iterations(void *arg);
void	*stop_simulation(void *args);

#endif

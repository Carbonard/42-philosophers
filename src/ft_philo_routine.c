/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_routine.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 23:00:14 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/04 03:13:56 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

static int	display_msg(const t_philo_data *data, const char *msg, char *color)
{
	size_t	timestamp_in_ms;

	pthread_mutex_lock(data->write_mutex);
	if (get_int(data->any_death) != 0)
	{
		pthread_mutex_unlock(data->write_mutex);
		return (1);
	}
	timestamp_in_ms = get_current_time_ms() - data->initial_time;
	printf("%s%-5lu %i %s\001\e[0m\002\n",
		color, timestamp_in_ms, data->number, msg);
	pthread_mutex_unlock(data->write_mutex);
	return (0);
}

static void	take_fork(t_protected_int *fork, t_protected_int *any_death)
{
	int	previous_value;

	previous_value = set_int(fork, 0);
	while (previous_value == 0 && get_int(any_death) == 0)
	{
		usleep(100);
		previous_value = set_int(fork, 0);
	}
}

static int	eat(t_philo_data *data)
{
	take_fork(data->first_fork, data->any_death);
	if (display_msg(data, "has taken a fork", PURPLE))
		return (1);
	take_fork(data->second_fork, data->any_death);
	if (display_msg(data, "has taken a fork", PURPLE))
		return (1);
	if (display_msg(data, "is eating", YELLOW))
		return (1);
	set_size_t(data->last_eat, get_current_time_ms());
	if (wait_ms(data, data->time_to_eat))
		return (1);
	set_int(data->first_fork, 1);
	set_int(data->second_fork, 1);
	return (0);
}

void	start_setup(t_philo_data *data)
{
	while (data->initial_time == 0)
	{
		usleep(10);
		data->initial_time = get_size_t(data->start_time);
	}
	if (data->number % 2 == 0)
		wait_ms(data, data->time_to_eat / 2);
	set_size_t(data->last_eat, data->initial_time);
}

void	*philo_routine(void *arg)
{
	t_philo_data	*data;
	long			iterations;

	data = (t_philo_data *)arg;
	iterations = 0;
	start_setup(data);
	while (get_int(data->any_death) == 0)
	{
		if (eat(data))
			break ;
		iterations++;
		if (iterations == data->number_of_times_each_philosopher_must_eat)
			add_one_uint(data->finished);
		if (display_msg(data, "is sleeping", BLUE))
			break ;
		if (wait_ms(data, data->time_to_sleep))
			break ;
		if (display_msg(data, "is thinking", GREEN))
			break ;
		if (wait_ms(data, 1))
			break ;
	}
	return (NULL);
}

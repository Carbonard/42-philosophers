/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_routine_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 23:00:14 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/04 02:14:15 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

int	wait_ms(t_global_data *data, useconds_t msec)
{
	size_t	init_time;

	init_time = get_current_time_ms();
	while (get_current_time_ms() < init_time + msec)
	{
		if (get_int(&data->finished) != 0)
			return (1);
		usleep(100);
	}
	return (0);
}

static void	close_eat_action(t_global_data *data)
{
	sem_post(data->forks_sem);
	sem_post(data->forks_sem);
	usleep(100);
	sem_post(data->control_sem);
}

static int	eat_action(t_global_data *data, char *msg, char *color)
{
	if (display_msg(data, msg, color))
	{
		close_eat_action(data);
		return (1);
	}
	return (0);
}

static int	eat(t_global_data *data)
{
	sem_wait(data->control_sem);
	usleep(100);
	sem_wait(data->forks_sem);
	if (eat_action(data, "has taken a fork", PURPLE))
		return (1);
	if (sem_wait(data->forks_sem))
		return (1);
	if (eat_action(data, "has taken a fork", PURPLE))
		return (1);
	set_size_t(&data->last_eat, get_current_time_ms());
	if (eat_action(data, "is eating", YELLOW))
		return (1);
	if (wait_ms(data, data->time_to_eat))
	{
		close_eat_action(data);
		return (1);
	}
	close_eat_action(data);
	add_one_uint(&data->meals);
	if (get_uint(&data->meals)
		== data->number_of_times_each_philosopher_must_eat)
		sem_post(data->iteration_sem);
	return (0);
}

void	philo_routine(t_global_data *data)
{
	pthread_t	death_monitor;
	pthread_t	simulation_monitor;

	create_death_monitor(data, &death_monitor, &simulation_monitor);
	if (data->philo_number % 2 == 0)
		wait_ms(data, data->time_to_eat / 2);
	while (get_int(&data->finished) == 0)
	{
		if (eat(data))
			break ;
		if (display_msg(data, "is sleeping", BLUE))
			break ;
		if (wait_ms(data, data->time_to_sleep))
			break ;
		if (display_msg(data, "is thinking", GREEN))
			break ;
		if (wait_ms(data, 1))
			break ;
	}
	pthread_join(death_monitor, NULL);
	pthread_join(simulation_monitor, NULL);
	close_semaphores(data);
	exit (0);
}

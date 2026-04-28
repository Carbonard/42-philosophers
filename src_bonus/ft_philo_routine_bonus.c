/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_routine_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 23:00:14 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/28 20:09:17 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

static int	eat_action(t_global_data *data, char *msg, char *color)
{
	if (display_msg(data, msg, color))
	{
		sem_post(data->control_sem);
		sem_post(data->forks_sem);
		sem_wait(data->forks_sem);
		return (1);
	}
	return (0);
}

static void	add_meal(t_global_data *data)
{
	sem_wait(data->data_sem);
	data->meals++;
	if (data->meals == data->number_of_times_each_philosopher_must_eat)
		sem_post(data->iteration_sem);
	sem_post(data->data_sem);
}

static int	eat(t_global_data *data)
{
	sem_wait(data->control_sem);
	usleep(500);
	sem_wait(data->forks_sem);
	if (eat_action(data, "has taken a fork", PURPLE))
		return (1);
	sem_wait(data->forks_sem);
	if (eat_action(data, "has taken a fork", PURPLE))
		return (1);
	sem_wait(data->data_sem);
	data->last_eat = get_current_time_ms();
	sem_post(data->data_sem);
	if (eat_action(data, "is eating", YELLOW))
		return (1);
	usleep(data->time_to_eat * 1000);
	sem_post(data->forks_sem);
	sem_post(data->forks_sem);
	sem_post(data->control_sem);
	add_meal(data);
	return (0);
}

void	philo_routine(t_global_data *data)
{
	pthread_t	death_monitor;
	pthread_t	simulation_monitor;

	data->meals = 0;
	create_death_monitor(data, &death_monitor, &simulation_monitor);
	while (!data->is_dead)
	{
		sem_post(data->data_sem);
		if (eat(data))
			break ;
		if (display_msg(data, "is sleeping", BLUE))
			break ;
		usleep(data->time_to_sleep * 1000);
		if (display_msg(data, "is thinking", ORANGE))
			break ;
		sem_wait(data->data_sem);
	}
	sem_post(data->data_sem);
	pthread_join(death_monitor, NULL);
	pthread_join(simulation_monitor, NULL);
	close_semaphores(data);
	exit (0);
}

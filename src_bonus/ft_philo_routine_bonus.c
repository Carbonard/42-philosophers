/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_routine_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 23:00:14 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/23 18:09:47 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

int	eat(t_global_data *data)
{
	sem_wait(data->control_sem);
	sem_wait(data->forks_sem);
	if (display_msg(data, "has taken a fork", PURPLE))
		return (1);
	sem_wait(data->forks_sem);
	if (display_msg(data, "has taken a fork", PURPLE))
		return (1);
	sem_wait(data->death_sem);
	if (display_msg(data, "is eating", YELLOW))
		return (1);
	data->last_eat = get_current_time_ms();
	sem_post(data->death_sem);
	usleep(data->time_to_eat * 1000);
	sem_post(data->forks_sem);
	sem_post(data->forks_sem);
	sem_post(data->control_sem);
	return (0);
}

void	philo_routine(t_global_data *data)
{
	pthread_t	monitor;

	create_death_monitor(data, &monitor);
	data->meals = 0;
	while (data->number_of_times_each_philosopher_must_eat == UNLIMITED_LOGS
		|| data->meals < data->number_of_times_each_philosopher_must_eat)
	{
		if (eat(data))
			break ;
		if (display_msg(data, "is sleeping", BLUE))
			break ;
		usleep(data->time_to_sleep * 1000);
		if (display_msg(data, "is thinking", ORANGE))
			break ;
		sem_wait(data->death_sem);
		data->meals++;
		sem_post(data->death_sem);
	}
	pthread_join(monitor, NULL);
	sem_close(data->forks_sem);
	sem_close(data->death_sem);
	sem_close(data->write_sem);
	sem_close(data->control_sem);
	exit (0);
}

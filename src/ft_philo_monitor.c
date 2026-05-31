/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_monitor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 15:14:01 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/28 16:36:47 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

static void	display_death(t_global_data *data, unsigned int id)
{
	long	timestamp_in_ms;

	pthread_mutex_lock(&(data->write_mutex));
	timestamp_in_ms = get_current_time_ms() - get_size_t(&(data->start_time));
	printf("%s%-5li %u %s\001\e[0m\002\n", RED, timestamp_in_ms, id, "died");
	pthread_mutex_unlock(&(data->write_mutex));
}

void	*monitorize(void *arg)
{
	t_global_data	*data;
	unsigned int	i;

	data = (t_global_data *)arg;
	usleep(data->time_to_die * 1000);
	while (get_int(&(data->any_death)) == 0)
	{
		i = 0;
		while (i < data->number_of_philosophers)
		{
			if (get_size_t(data->last_eats + i)
				+ (size_t)data->time_to_die <= get_current_time_ms()
				&& get_size_t(data->last_eats + i) != 0)
			{
				set_int(&(data->any_death), 1);
				display_death(data, i + 1);
				break ;
			}
			i++;
		}
		if (get_uint(&(data->finished)) == data->number_of_philosophers)
			set_int(&(data->any_death), 1);
		usleep(700);
	}
	return (NULL);
}

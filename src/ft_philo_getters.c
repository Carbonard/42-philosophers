/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_getters.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 12:59:59 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/02 13:00:08 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

int	get_int(t_protected_int *p_var)
{
	int	old_value;

	pthread_mutex_lock(&p_var->mutex);
	old_value = p_var->content;
	pthread_mutex_unlock(&p_var->mutex);
	return (old_value);
}

unsigned int	get_uint(t_protected_uint *p_var)
{
	unsigned int	old_value;

	pthread_mutex_lock(&p_var->mutex);
	old_value = p_var->content;
	pthread_mutex_unlock(&p_var->mutex);
	return (old_value);
}

size_t	get_size_t(t_protected_size_t *p_var)
{
	size_t	old_value;

	pthread_mutex_lock(&p_var->mutex);
	old_value = p_var->content;
	pthread_mutex_unlock(&p_var->mutex);
	return (old_value);
}

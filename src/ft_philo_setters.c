/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_setters.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 18:50:37 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/02 12:59:47 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

int	set_int(t_protected_int *p_var, int new_value)
{
	int	old_value;

	pthread_mutex_lock(&p_var->mutex);
	old_value = p_var->content;
	p_var->content = new_value;
	pthread_mutex_unlock(&p_var->mutex);
	return (old_value);
}

unsigned int	set_uint(t_protected_uint *p_var, unsigned int new_value)
{
	unsigned int	old_value;

	pthread_mutex_lock(&p_var->mutex);
	old_value = p_var->content;
	p_var->content = new_value;
	pthread_mutex_unlock(&p_var->mutex);
	return (old_value);
}

void	add_one_uint(t_protected_uint *p_var)
{
	pthread_mutex_lock(&(p_var->mutex));
	p_var->content++;
	pthread_mutex_unlock(&(p_var->mutex));
}

size_t	set_size_t(t_protected_size_t *p_var, size_t new_value)
{
	size_t	old_value;

	pthread_mutex_lock(&p_var->mutex);
	old_value = p_var->content;
	p_var->content = new_value;
	pthread_mutex_unlock(&p_var->mutex);
	return (old_value);
}

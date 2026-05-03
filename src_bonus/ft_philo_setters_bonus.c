/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_setters_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 18:50:37 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/03 21:36:17 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

int	set_int(t_protected_int *p_var, int new_value)
{
	int	old_value;

	sem_wait(p_var->sem);
	old_value = p_var->content;
	p_var->content = new_value;
	sem_post(p_var->sem);
	return (old_value);
}

unsigned int	set_uint(t_protected_uint *p_var, unsigned int new_value)
{
	unsigned int	old_value;

	sem_wait(p_var->sem);
	old_value = p_var->content;
	p_var->content = new_value;
	sem_post(p_var->sem);
	return (old_value);
}

void	add_one_uint(t_protected_uint *p_var)
{
	sem_wait(p_var->sem);
	p_var->content++;
	sem_post(p_var->sem);
}

size_t	set_size_t(t_protected_size_t *p_var, size_t new_value)
{
	size_t	old_value;

	sem_wait(p_var->sem);
	old_value = p_var->content;
	p_var->content = new_value;
	sem_post(p_var->sem);
	return (old_value);
}

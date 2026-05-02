/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 18:30:16 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/02 13:04:54 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

int	main(int argc, char **argv)
{
	t_global_data	g_data;

	if (check_input(argc, argv))
		save_data(argc, argv, &g_data);
	else
		return (1);
	if (manage_philosophers(&g_data))
		return (1);
	return (0);
}

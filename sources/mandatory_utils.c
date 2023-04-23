/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandatory_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 23:11:07 by Helene            #+#    #+#             */
/*   Updated: 2023/04/24 01:01:30 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	initialise_data(t_fork_data *data, char **argv, int last)
{
	if (access(argv[1], F_OK | R_OK) == -1)
	{
		perror("access");
		if (access(argv[last], F_OK) == 0 && access(argv[last], W_OK) == -1)
			perror("access");
		else if (access(argv[last], F_OK) == -1)
			close(open(argv[last], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU));
		exit(5);
	}
	if (access(argv[last], F_OK) == 0 && access(argv[last], W_OK) == -1)
	{
		perror("access");
		//exit(5);
	}
	data->in_out[0] = open(argv[1], O_RDONLY);
	if (data->in_out[0] == -1)
	{
		perror("open ");
		exit(4);
	}
	data->in_out[1] = open(argv[last], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
	if (data->in_out[1] == -1)
	{
		perror("open ");
		//f2_close(data->in_out[0]);
		//exit(4);
	}
}

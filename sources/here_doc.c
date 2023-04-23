/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 17:07:03 by hlesny            #+#    #+#             */
/*   Updated: 2023/04/24 01:10:19 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	contains_limiter(char *line, char *limiter) // returns
												//-1 if line doesn't contain it,
												//and the index of the limiter in line if it does
{
	int	i;
	int	j;
	int	k;

	i = 0;
	while (line[i])
	{
		j = 0;
		while (line[i] && line[i] != limiter[j])
			i++;
		k = i;
		while (line[k] && line[k] == limiter[j])
		{
			k++;
			j++;
		}
		if (!limiter[j])
			// ie si est sorti du while en ayant entièrement parcouru limiter
			return (i);
		if (line[i])
			i++;
	}
	return (-1);
}

int	get_input(char *limiter, int *in, int infile)
{
	char	*line;
	int		i;
	int		j;

	line = get_next_line(STDIN_FILENO);
	while (line && contains_limiter(line, limiter) == -1)
	{
		ft_putstr_fd(line, infile);
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	i = -1;
	j = contains_limiter(line, limiter);
	if (line && j >= 0)
	{
		free(line);
		close(infile);
		*in = open("here_doc", O_CREAT | O_RDONLY, S_IRUSR | S_IWUSR);
		return (infile);
	}
	close(infile);
	return (-1);
}

int	test_here_doc(char *arg1, char *limiter, int *in)
{
	int		infile;

	if (!strsearch(arg1, "here_doc"))
		return (-1);
	infile = open("here_doc", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
	if (infile == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	return (get_input(limiter, in, infile));
}

void	here_doc_case(t_fork_data *data, char **argv, int last)
{
	if (access(argv[last], F_OK) == 0 && access(argv[last], W_OK) == -1)
		perror("access");
	data->in_out[1] = open(argv[last], O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
	if (data->in_out[1] == -1)
	{
		perror("open ");
		f2_close(data->in_out[0]);
		unlink("here_doc");
		exit(4);
	}
}

void	no_here_doc_case(t_fork_data *data, char **argv, int last)
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
		perror("access");
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
		f2_close(data->in_out[0]);
	}
}

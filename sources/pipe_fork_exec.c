/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_fork_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 02:31:34 by Helene            #+#    #+#             */
/*   Updated: 2023/04/24 02:32:41 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	open_pipe(t_fork_data *data, int i)
{
	if (i < data->processes_nb - 1)
	{
		if (i)
		{
			f_dup2(data->pipefd[0], STDIN_FILENO);
			f_close(data->pipefd[0]);
			f_close(data->pipefd[1]);
		}
		if (pipe(data->pipefd) == -1)
		{
			perror("pipe");
			exit(3);
		}
		if (data->pipefd[0] < 0 || data->pipefd[1] < 0)
		{
			write(2, "Error : open_pipe() : invalid file\n", 35);
			exit(EXIT_FAILURE);
		}
	}
}

void	child_process(char **envp, t_fork_data *data, int i, int *pid)
{
	if (i == data->processes_nb - 1)
	{
		f_close(data->pipefd[1]);
		f_dup2(data->pipefd[0], STDIN_FILENO);
		f_close(data->pipefd[0]);
	}
	else
	{
		f_dup2(data->pipefd[1], STDOUT_FILENO);
		f_close(data->pipefd[1]);
		f_close(data->pipefd[0]);
	}
	if (execve(data->commands[i][0], data->commands[i], envp) == -1)
	{
		ft_putstr_fd(data->commands[i][0], STDERR_FILENO);
		ft_putstr_fd(" : ", STDERR_FILENO);
		perror("execve ");
		free_commands(data->commands);
		free(pid);
		exit(3);
	}
	exit(0);
}

void	pipe_fork_exec(t_fork_data *data, char **envp, int pids_nb)
{
	pid_t	*pid;
	int		i;

	i = -1;
	pid = malloc(sizeof(int) * (pids_nb));
	if (!pid)
		return ;
	while (++i < data->processes_nb)
	{
		open_pipe(data, i);
		pid[i] = fork();
		if (pid[i] == -1)
			perror("fork ");
		if (pid[i] == 0)
			child_process(envp, data, i, pid);
	}
	f_close(data->pipefd[0]);
	f_close(data->pipefd[1]);
	i = -1;
	while (++i < data->processes_nb)
	{
		if (waitpid(pid[i], &data->wstatus, 0) == -1)
			perror("waitpid ");
	}
	free(pid);
}

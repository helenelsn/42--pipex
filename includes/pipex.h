/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 20:06:49 by hlesny            #+#    #+#             */
/*   Updated: 2023/04/24 02:25:13 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "get_next_line.h"
# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>     // for perror()
# include <stdlib.h>    // defines EXIT_FAILURE and EXIT_SUCCESS
# include <sys/types.h> // for pid_t data type
# include <sys/wait.h>  // for wait()
# include <unistd.h>    // for fork()

typedef struct s_fork_data
{
	char	***commands;
	int		pipefd[2];
	int		in_out[2];
	int		wstatus;
	int		processes_nb;
}			t_fork_data;

char		***set_commands(int cmds_count, char **cmds, char **envp);
int			strsearch(char *string, char *to_find);
void		initialise_data(t_fork_data *data, char **argv, int last);
void		open_pipe(t_fork_data *data, int i);
void		pipe_fork_exec(t_fork_data *data, char **envp, int pids_nb);
void		child_process(char **envp, t_fork_data *data, int i, int *pid);
void		f_close(int fd);
int			f2_close(int fd);
void		f_dup2(int fd1, int fd2);
void		free_commands(char ***commands);
void		free_tab(char **tab);

#endif
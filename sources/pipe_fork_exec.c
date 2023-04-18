/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_fork_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 02:31:34 by Helene            #+#    #+#             */
/*   Updated: 2023/04/19 01:46:05 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void    open_pipe(t_fork_data *data, int i)
{
    if (i < data->processes_nb - 1)
    {
        if (i)
        {
            dup2(data->pipefd[0], STDIN_FILENO);
            close(data->pipefd[0]);
            close(data->pipefd[1]);
        }
        if (pipe(data->pipefd) == -1) // pipefd[0] -- read // pipefd[1] : write
        {
            perror("pipe");   
            exit(3);
        }     
    }
}

void    child_process(char **envp, t_fork_data *data, int i)
{
    if (i == data->processes_nb - 1) // we want to write in the outfile, rather than in pipefd[1]
    {
        close(data->pipefd[1]);
        dup2(data->pipefd[0], STDIN_FILENO);
        close(data->pipefd[0]);
    }
    else
    {
        dup2(data->pipefd[1], STDOUT_FILENO);
        close(data->pipefd[1]);
        close(data->pipefd[0]); //?
    }
    if (execve(data->commands[i][0], data->commands[i], envp) == -1)
    {
        fprintf(stderr, "%s : ", data->commands[i][0]); // le remplacer par mon printf a moi
        perror("execve ");
        free_commands(data->commands);
        exit(3);
    }
    exit(0); // we only want to fork in the parent process ; we therefore terminate each child process after they executed their assignated command, 
    //or else they would stay in the while loop and create child processes of their own as well
}

void    pipe_fork_exec(t_fork_data *data, char **envp, int pids_nb)
{
    int pid[pids_nb];
    int i;

    i = -1;
    while (++i < data->processes_nb)
    {
        open_pipe(data, i);
        pid[i] = fork();
        if (pid[i] == -1)
            perror("fork ");
        if (pid[i] == 0)
            child_process(envp, data, i);
    }
    f_close(data->pipefd[0]);
    f_close(data->pipefd[1]);
    // if (close(data->pipefd[0]) == -1)
    //     perror("close");
    // if (close(data->pipefd[1]) == -1)
    //     perror("close");
    i = -1;
    while (++i < data->processes_nb)
    {
        if (waitpid(pid[i], &data->wstatus, 0) == -1)
            perror("waitpid ");
    }
}

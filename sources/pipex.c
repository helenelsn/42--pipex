/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 02:31:34 by Helene            #+#    #+#             */
/*   Updated: 2023/04/01 03:15:48 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"


int    child_exec(t_command *cmd)
{
    
}

void    set_commands_list(t_command *cmd, int argc, char **argv)
{
    
}

int main(int argc, char **argv, char **envp)
{
    t_command *current;
    
    int pipes_nb;
    int processes_nb;
    int pipefd[argc - 2][2];
    int pid[argc - 3];

    int in_out[2];
    int i;
    int j;

    current = NULL;
    set_commands_list(&current, argc, argv);
    
    pipes_nb = argc - 2;
    processes_nb = argc - 3; 
    i = 0;
    in_out[0] = open(argv[1], O_RDONLY);
    in_out[1] = open(argv[argc - 1], O_WRONLY);
    while (i < pipes_nb)
    {
        if (pipe(pipefd[i]) == -1)
            return (perror("pipe : "), 1);
        i++;
    }
    i = 0;
    while (i < processes_nb && current)
    {
        pid[i] = fork();
        if (pid[i] == -1)
            return (perror("fork : "), 2);
        if (pid[i] == 0) // child process
        {
            j = 0;
            while (j < pipes_nb)
            {
                if (j == i)
                {
                    if (j == 0) // doit alors lire depuis d'infile donné en argument via le terminal
                        dup2(pipefd[j][0], in_out[0]);
                    else
                        dup2(pipefd[j][0], STDIN_FILENO);
                }
                if (j == i + 1)
                {
                    if (j == pipes_nb - 1) // doit alors écrire sur l'outfile donné en argument via le terminal
                        dup2(pipefd[j][1], in_out[1]);
                    else
                        dup2(pipefd[j][1], STDOUT_FILENO);
                }
                close(pipefd[j][0]);
                close(pipefd[j][1]);
                if (execve(current->path, current->args, envp) == -1)
                    return (perror("execve : "), 3);
                j++;
            }
            return (0); // we only want to fork in the parent process
        }
        current = current->next;
        i++;
    }
    
    
    return (0);
}
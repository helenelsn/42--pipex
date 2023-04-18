/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 15:56:15 by Helene            #+#    #+#             */
/*   Updated: 2023/04/18 23:28:46 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int main(int argc, char **argv, char **envp)
{   
    t_fork_data data;
    int i;

    if (argc < 5)
    {
        ft_putstr_fd("Not enough arguments in command line!\n", STDERR_FILENO);
        return (5);
    }
    initialise_data(&data, argv, argc - 1);
    dup2(data.in_out[0], STDIN_FILENO);
    close(data.in_out[0]);
    dup2(data.in_out[1], STDOUT_FILENO);
    close(data.in_out[1]);
    data.commands = set_commands(argc - 3, argv + 2, envp);
    data.processes_nb = argc - 3; 
    pipe_fork_exec(&data, envp, argc - 3);
    free_commands(data.commands);
    return (WIFEXITED(data.wstatus) & WEXITSTATUS(data.wstatus)); 
}

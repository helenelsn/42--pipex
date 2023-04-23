/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 15:56:24 by Helene            #+#    #+#             */
/*   Updated: 2023/04/23 17:17:49 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "pipex_bonus.h"

int main(int argc, char **argv, char **envp)
{   
    t_fork_data data;
    int here_doc;

    if (argc < 5)
        return (ft_putstr_fd("Not enough arguments in command line!\n", STDERR_FILENO), 5);
    if (!envp)
        return (6);
    here_doc = test_here_doc(argv[1], argv[2], &data.in_out[0]); // gerer le cas ou ecrit uniquement le limiter sur stdin (ie l'infile est vide)
    if (here_doc == -1)
        no_here_doc_case(&data, argv, argc - 1);
    else
        here_doc_case(&data, argv, argc - 1);
    dup2(data.in_out[0], STDIN_FILENO);
    dup2(data.in_out[1], STDOUT_FILENO);
    close(data.in_out[0]);
    close(data.in_out[1]);
    data.commands = set_commands(argc - 3 - (here_doc > -1), argv + 2 + (here_doc > -1), envp);
    data.processes_nb = argc - 3 - (here_doc > -1); 
    pipe_fork_exec(&data, envp, argc - 3 - (here_doc > -1));
    free_commands(data.commands);
    return (WIFEXITED(data.wstatus) & WEXITSTATUS(data.wstatus)); 
}

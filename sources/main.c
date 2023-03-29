/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 20:06:11 by hlesny            #+#    #+#             */
/*   Updated: 2023/03/30 00:16:03 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"



void    set_av(char **argv)
{
    int ac;

    ac = 1;
    
}

// faire autant de fork() (ie creer autant de child processes)
// qu'il y a de commandes passees en ligne de commande
// pourquoi ne pas executer la derniere dans le processus parent ?

int main(int argc, char **argv)
{
    int pipefd[2]; // pipefd[0] : read end of the pipe. pipefd[1] : write end of the pipe
    int child_pid;
    char **av;
    
    if (pipe(pipefd) == -1) // returns 0 if pipe is successfully initialised
    {
        perror("pipe : ");
        exit(1); // ou juste return(1) ?
    }
    child_pid = fork();
    if (child_pid == 0)
    {
        set_av(argv);
        // find the path of the unix command
    }
}
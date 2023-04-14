/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 20:06:49 by hlesny            #+#    #+#             */
/*   Updated: 2023/04/14 21:22:32 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

#include <unistd.h> // for fork()
#include <sys/types.h> // for pid_t data type
#include <sys/wait.h> // for wait()
#include <fcntl.h>
#include <errno.h>
#include <stdio.h> // for perror()

#include "libft.h"
#include "get_next_line.h"

// typedef struct  s_command
// {
//     char            *path;
//     char            **args; // ne pass oublier de NULL-terminate le tableau de pointeurq
//     struct s_cmd   *next;
// }               t_command;

int     test_here_doc(char *arg1, char *limiter);
char    ***set_commands(int argc, char **argv, char **envp);
void    free_commands(char ***commands);
void    free_tab(char **tab);
int     strsearch(char *string, char *to_find);

/* pipe : chainer des processus de maniere a ce que la sortie d'un processus 
alimente directement l'entree du suivant 
The second process is started as the first one is stille running, 
and they are executed concurrently (fork)

Each process takes input from the previous process and produces output
for the next process via standard streams
Pipes are unidirectional : data flows through the pipeline from left to right

Each process must close the end(s) of the pipe that it will not be using
before producing or consuming any data
*/



#endif
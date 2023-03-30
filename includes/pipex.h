/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 20:06:49 by hlesny            #+#    #+#             */
/*   Updated: 2023/03/30 20:18:13 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

#include <unistd.h> // for fork()
#include <sys/types.h> // for pid_t data type
#include <sys/wait.h> // for wait()
#include <fcntl.h>

typedef struct  s_cmd
{
    char            *path;
    char            *args[]; // ne pass oublier de NULL-terminate le tableau de pointeurq
    int             pid; // initialisé à -1. Sert à savoir à quel process est assigné chaque commande (?) 
    struct s_elem   *next:
}               t_cmd;


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
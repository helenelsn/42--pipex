/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 02:31:34 by Helene            #+#    #+#             */
/*   Updated: 2023/04/14 17:10:57 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_next_line(int fd);

void    free_tab(char **tab)
{
    int i;

    i = 0;
    while (tab[i])
    {
        free(tab[i]);
        i++;
    }
    free(tab);
}

/*
{ {"/usr/bin/grep", "a", NULL},
    {"/usr/bin/ls", "-la", NULL},
    {NULL} };
*/
void    free_commands(char ***commands)
{
    int i;

    i = 0;
    while (commands[i])
    {
        free_tab(commands[i]);
        i++;
    }
    free(commands);
}

/*

"<" est utilisé pour rediriger l'entrée standard à partir d'un fichier ou d'un autre flux, 
tandis que "<<" est utilisé pour rediriger l'entrée standard à partir d'une chaîne de caractères 
délimitée par une autre chaîne.

----------------------------------------------------------------------------

cmd > file : écrase le contenu existant
cmd >> file : écrit à la suite du fichier

">" est utilisé pour écrire la sortie standard d'une commande dans un fichier, 
en remplaçant son contenu existant s'il y en a, 
tandis que ">>" est utilisé pour ajouter la sortie standard d'une commande à la fin d'un fichier, 
sans effacer son contenu existant.

*/

int main(int argc, char **argv, char **envp)
{
    char ***commands;
    
    int processes_nb;
    int pipefd[2];
    int pid[argc - 3];
    int wstatus;

    int i;
    int j;
    int in_out[2];
    int here_doc;

    if (argc < 5)
    {
        ft_putstr_fd("Not enough arguments in command line!\n", STDERR_FILENO);
        return (5);
    }
    
    here_doc = test_here_doc(argv[1], argv[2]); // gerer le cas ou ecrit uniquement le limiter sur stdin (ie l'infile est vide)
    
    if (here_doc == -1)
    {
        if (access(argv[1], F_OK | R_OK) == -1) // l'infile n'existe pas, ou ne peut pas etre lu
            return(perror("access"), 4);
        in_out[0] = open(argv[1], O_RDONLY);
        if (in_out[0] == -1)
            return(perror("open "), 4);   
        in_out[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
        if (in_out[1] == -1)
        {
            close(in_out[0]);
            return(perror("open "), 4);
        }
        /* dup2(in_out[0], STDIN_FILENO);
        close(in_out[0]); */
    }
    else
    {
        in_out[0] = here_doc; // re vérifier si est bien >= 0 ? 
        in_out[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
        if (in_out[1] == -1)
        {
            close(in_out[0]);
            return(perror("open "), 4);
        }
    }
    dup2(in_out[0], STDIN_FILENO);
    close(in_out[0]);
    dup2(in_out[1], STDOUT_FILENO);
    close(in_out[1]);
    fprintf(stderr, "here doc = %d\n", here_doc);
    commands = set_commands(argc - 3 - (here_doc > -1), argv + 2 + (here_doc > -1), envp);
    if (!commands)
        return (perror("set_commands "), 4);
    processes_nb = argc - 3 - (here_doc > -1); 
    i = 0;
    
    while (i < processes_nb && commands[i])
    {
        if (i < processes_nb - 1)
        {
            if (i)
            {
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]);
                close(pipefd[1]); // ?
            }
            if (pipe(pipefd) == -1) // pipefd[0] -- read // pipefd[1] : write
                return (perror("pipe"), 2);   
        }
        //printf("ok, i = %d\n", i);
        pid[i] = fork();
        if (pid[i] == -1)
            return (perror("fork "), 2);
        if (pid[i] == 0) // child process
        {
            //sleep(1);
            //fprintf(stderr, "ok, child %d\n", i);
            if (i == 0) // in that case, we want to read from the infile rather than from pipefd[0]
            {
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }
            else if (i == processes_nb - 1) // we want to write in the outfile, rather than in pipefd[1]
            {
                close(pipefd[1]);
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]);
            }
            else
            {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
                close(pipefd[0]); //?
            }
            fprintf(stderr, "i = %d, my pid is %d, my parent is %d\n", i, getpid(), getppid());
            if (execve(commands[i][0], commands[i], envp) == -1)
                return (perror("execve "), 3);
            exit(0); // we only want to fork in the parent process ; we therefore terminate each child process after they executed their assignated command, or else they would stay in the while loop and create child processes of their own as well
            
        }
        i++;
    }
    i = 0;
   if (close(pipefd[0]) == -1)
        perror("close");
    if (close(pipefd[1]) == -1)
        perror("close");
    i = 0;
    while (i < processes_nb)
    {
        //waitpid(pid[i], &wstatus, 0);
        
        if (waitpid(pid[i], &wstatus, 0) == -1)
            perror("waitpid ");
        //if (!WIFEXITED(wstatus))
        //    perror("wifexited ");
        fprintf(stderr, "done waiting for child %d\n", i);
        i++;
    }
    sleep(1);
    free_commands(commands);
    return (WIFEXITED(wstatus) & WEXITSTATUS(wstatus)); 
}

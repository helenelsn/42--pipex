/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 20:06:11 by hlesny            #+#    #+#             */
/*   Updated: 2023/04/01 03:10:00 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* 
Structure de données pour ce projet :
Liste chainee avec, pour chaque element de la liste, les arguments a envoyer a execv 
(ie le path vers le binaire de la commande, et les arguments a passer pour l'execution de la commande) ? 
*/

char **str_append(char *s, char **tab)
{
    
}

t_command *new_elem(char *path, char **argv)
{
    t_command *new;

    new = malloc(sizeof(t_command)); // ft_calloc plutot ?
    if (!new)
        return (perror("malloc : "), NULL);
    new->path = path;
    new->args = str_append(path, argv);
    new->next = NULL;
}

char *get_path(char *command)
{
    
}

void    add_back(t_command **current, t_command *new)
{
    (*current)->next = new;
    current = &new; // ?
}

int     command_check(char **command) // verifies whether the command given as input is valid, ie exists 
{
    
}

int set_command(char *command, t_command **current)
{
    t_command *new;
    char *path;
    char **argv;

    argv = ft_split(command, " "); // " " ou ' ' ? me souviens plus
    new = new_elem(get_path(command), argv);
    if (!current)
        return (-1);
    if (!*current) // on traite donc la premiere string (ie commande) de la ligne de commande 
        *current = new;
    else
        add_back(current, new);
    return (0);
}

int get_commands(int cmdc, char **cmdv, t_command **cmd)
{
    int i;
    t_command *current;
    
    i = 1;
    if (!set_command(cmdv[0], cmd))
        return (-1);
    current = *cmd;
    while (i < cmdc) // les premier et dernier arguments passés en command line a l'executable pipex sont des fichiers, les ignore ici
    {
        if (!set_command(cmdv[i] , &current))
            return (-1);    
        i++;
    }
    // est-ce que cmd pointe encore bien vers le premier element de la liste chainée ? 
    return (0);
}

int     child_process(t_command *cmd, char **envp, int pipefd[2])
{
    dup2(pipefd[0], STDIN_FILENO);
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[0]);
    close(pipefd[1]);
    if (execve(cmd->path, cmd->args, envp) == -1)
        return (perror("execve : "), 0);
}

int main(int argc, char **argv, char **envp)
{
    int fd_in;
    int fd_out;
    int wtstatus; 
    int err;
    
    pid_t pid;
    int pipefd_1[2]; // pipefd[0] : read end of the pipe. pipefd[1] : write end of the pipe
    t_command *cmd;
    
    fd_in = open(argv[0], O_RDONLY);
    fd_out = open(argv[argc - 1], O_WRONLY); //pour la derniere commande executée
    cmd = NULL;
    if (!get_commands(argc - 1, argv + 2, cmd))
        return (strerror("input error : at least one of the commands does not exist\n"), 1);
    
    // on linux, pipe() does not modify pipefd on failure.
    if (pipe(pipefd_1) == -1) // returns 0 if pipe is successfully initialised. 
        return (perror("pipe : "), 2);
    
    while(cmd->next) // traite la derniere commande à part car doit ecrire dans outfile, et plus dans pipefd[1]
    {
        pid = fork(); // duplique le processus courant
        if (pid == -1)
            return (perror("fork : "), 3);
        if (pid == 0) // is in the child's process
        {
            // !!!!! à coder : gerer la premiere commande à part, car celle-ci lit dans le fd_in, et pas pipefd[0]
            if (!child_process(cmd, envp, pipefd_1))
                return (strerror("Error in child process\n"), 4);
        }
        /* while (waitpid(&wtstatus) != -1 || errno != ECHILD) // waits for a child process to finish (waits for all of them ?)
            // je fais quoi la ?
        // on success, wait returns the pid of the terminated child. returns -1 on failure
        if (wait(&wtstatus) == -1)
        {
            perror("wait : ");
            exit(1);
        } */
        // tous les children processes sont finis : execute la dernière commande
        cmd = cmd->next;
    }
    
    waitpid(pid, NULL, 0); // ?
    
    close(pipefd_1[1]);
    dup2(pipefd_1[0], STDIN_FILENO); // ?
    dup2(fd_out, STDOUT_FILENO);
    if (execve(cmd->path, cmd->args, envp) == -1)
        return (perror("execve : "), 5);
    close(pipefd_1[0]);
    if (unlink(fd_in) == -1 || unlink(fd_out) == -1)
        return (perror("unlink : "), 6);
     return (0);
}

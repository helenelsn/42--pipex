/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 20:06:11 by hlesny            #+#    #+#             */
/*   Updated: 2023/03/30 21:30:25 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* 
Structure de données pour ce projet :
Liste chainee avec, pour chaque element de la liste, les arguments a envoyer a execv 
(ie le path vers le binaire de la commande, et les arguments a passer pour l'execution de la commande) ? 
*/

int set_command(char *command, t_cmd *current)
{
    
}

int get_commands(int cmdc, char **cmdv, t_cmd *cmd)
{
    int i;
    t_cmd *current;

    if (cmdc == 1)
        return(set_command(cmdv[0] , cmd));
    if (!set_command(cmdv[0] , cmd)) // crée le premier element de la liste de commandes
        return (-1);
    i = 1;
    current = NULL;
    while (i < cmdc) // les premier et dernier arguments passés en command line a l'executable pipex sont des fichiers, les ignore ici
    {
        
        
        i++;
    }
    
}

int     child_process(t_cmd *cmd)
{
    
}

int main(int argc, char **argv, char **env)
{
    int fd_in;
    int fd_out;
    int status; // à quoi l'initialiser ?
    pid_t pid;
    int pipefd[2]; // pipefd[0] : read end of the pipe. pipefd[1] : write end of the pipe
    t_cmd *cmd;
    
    fd_in = open(argv[0], O_RDONLY);
    fd_out = open(argv[argc - 1], O_WRONLY); //pour la derniere commande executée
    cmd = NULL;
    if (!get_commands(argc - 1, argv + 2, cmd))
        return (strerror("input error : at least one of the commands does not exist\n"), 1); // ou juste exit(1) ? sert a r car est deja a la racine du programme ici 
    
    // on linux, pipe() does not modify pipefd on failure.
    if (pipe(pipefd) == -1) // returns 0 if pipe is successfully initialised. 
        return (perror("pipe : "), 1);
    
    while(cmd->next) // traite la derniere commande à part car doit ecrire dans outfile, et plus dans pipefd[1]
    {
        pid = fork(); // duplique le processus courant
        if (pid == -1)
            return (perror("fork : "), 1);
        if (pid == 0) // is in the child's process
        {
            // !!!!! à coder : gerer la premiere commande à part, car celle-ci lit dans le fd_in, et pas pipefd[0]
            if (!child_process(cmd))
                return (strerror("Error in child process\n"), 1);
        }
        else // is in the parent's process
        {
            if (wait(&status) == -1)
            {
                perror("wait : ");
                exit(1);
            }
        }
        cmd = cmd->next;
    }
    close(pipefd[1]);
    dup2(pipefd[0], 0); // ?
    dup2(fd_out, 1);
    execve(cmd->path, cmd->args, env); // NULL pour l'env ?
    close(pipefd[0]);
    if (unlink(fd_in) == -1)
        perror("unlink : "); // pas de return ici car veut aussi unlink fd_out
    if (unlink(fd_out) == -1)
        return (perror("unlink : "), 1); // return ici ou inutile ? est a la fin du programme dans tous les cas....
     return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 02:31:34 by Helene            #+#    #+#             */
/*   Updated: 2023/04/08 23:15:22 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void    free_tab(char ***command)
{
    int i;

    i = 0;
    while (*command[i])
    {
        free(*command[i]);
        i++;
    }
}

void    free_commands(char ****commands) // char *** ou char **** ?
{
    int i;

    i = 0;
    while (*commands[i])
    {
        free_tab(commands[i]);
        i++;
    }
}

// add the command string to the path string
char	*add_command(char *path, char *command)
{
	int		len1;
    int     len2;
	char	*tmp;
	int		i;

	i = -1;
    len1 = ft_strlen(path);
	len2 = ft_strlen(command);
	tmp = malloc(sizeof(char) * (len1 + len2 + 2));
	if (!tmp)
		return (NULL);
	while (++i < len1)
		tmp[i] = path[i];
    tmp[i] = '/';
	i = -1;
	while (++i < len2)
		tmp[len1 + i + 1] = command[i];
	tmp[len1 + i + 1] = '\0';
	return (tmp);
}

int strsearch(char *string, char *to_find) // détermine si string commence par to_find
{
    int i;

    i = 0;
    while (string[i] && to_find[i])
    {
        if (string[i] != to_find[i])
            return (0);
        i++;    
    }
    if (to_find[i]) // n'est pas sorti du while parce qu'est arrivé au bout de to_find
        return (0);
    return (1);
}

char *get_path(char *command, char **envp)
{
    int i;
    char *path;
    char **paths;

    i = 0;
    while (envp[i] && !strsearch(envp[i], "PATH="))
        i++;
    path = ft_strjoin(".:", envp[i] + 5); // ./ est un chemin relatif : est-ce qu'access() gere ce genre de chemin ?
    if (!path)
        return (NULL);
    paths = ft_split(path, ':');
    
    i = 0;
    while (paths[i])
    {
        free(path);
        path = add_command(paths[i], command);
        //printf("path = %s\n", path);
        if (!access(path, F_OK) && !access(path, X_OK)) // on success, 0 is returned
        {
            return (path);
        }
        i++;
    }
    return (free_tab(&paths), free(path), NULL); // si aucun path n'est valide
}

// get the commands' binary's absolute paths (env | grep PATH)
char ***set_commands(int argc, char **argv, char **envp) // chaque argv[i] correspond a une commande
{
    char ***commands;
    int i;
    int j;
    
    i = 0; // 0 = a.out et 1 = infile
    commands = malloc(sizeof(char **) * (argc + 1));
    if (!commands)
        return (NULL);
    while (i < argc)
    {
        commands[i] = ft_split(argv[i], ' '); // command is null-terminated
        if (!commands[i])
            return (perror("Error (tried to get the right path) "), NULL);
       /* j = 0;
        while (command[j])
            j++;
        commands[i] = malloc(sizeof(char *) * (j + 1));
        if (!commands[i])
            return (NULL); */
        j = 0;
        commands[i][0] = get_path(commands[i][0], envp);
        /*
        if (!commands[i][j])
            return (free_commands(&commands), free_tab(&command), NULL);
        while (command[++j])
        {
            commands[i][j] = ft_strdup(command[j]); // rajoute les options de la commande i
            if (!commands[i][j])
                return (free_commands(&commands), free_tab(&command), NULL);
        }*/
        //free_tab(&command);
        i++;
        printf("hello\n");
    }
    commands[i] = NULL;
    return (commands);
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

/*
void    manage_here_doc()
{
    char *line;
    
    line = get_next_line(STDIN_FILENO);
    while (line) // && condition sur la présence de "LIMITER" dans line
    {
        free(line);
        line = get_next_line(STDIN_FILENO);
    }
    
}
*/

int main(int argc, char **argv, char **envp)
{
    char ***commands;
    
    int pipes_nb;
    int processes_nb;
    int pipefd[argc - 2][2];
    int pid[argc - 3];

    int in_out[2];
    int i;
    int j;
    int wstatus;
    //int here_doc;

    //here_doc = strsearch(argv[1], "here_doc");
    commands = set_commands(argc - 3, argv + 2, envp);
    if (!commands)
        return (4);
    
    pipes_nb = argc - 2;
    processes_nb = argc - 3; 
    i = 0;
   // if (access(argv[1], F_OK | R_OK) == -1) // l'infile n'existe pas, ou ne peut pas etre lu
     //   return(4);
    in_out[0] = open(argv[1], O_RDONLY);
    if (in_out[0] == -1)
        return(perror("open "), 4);
    in_out[1] = open(argv[argc - 1], O_WRONLY, O_CREAT);
    if (in_out[1] == -1)
    {
        close(in_out[0]);
        return(perror("open "), 4);
    }
    
    while (i < pipes_nb)
    {
        if (pipe(pipefd[i]) == -1)
            return (perror("pipe "), 1);
        i++;
    }
    i = 0;
    perror("deb\n");
    while (i < processes_nb && commands[i])
    {
        pid[i] = fork();
        if (pid[i] == -1)
            return (perror("fork "), 2);
        if (pid[i] == 0) // child process
        {
            
            j = 0;
            
            while (j < pipes_nb)
            {
                //printf("ici %d\n", i);
                int fd;
                if (j == i)
                {
                    if (j == 0) // doit alors lire depuis d'infile donné en argument
                    {
                        close(pipefd[j][0]);
                        fd  = in_out[0];
                    }
                    else
                    {
                        close(in_out[0]);
                        fd = pipefd[j][0];
                    }
                    close(pipefd[j][1]);
                    dup2(fd , STDIN_FILENO);
                    close(fd);
                }
                else if (j == i + 1)
                {
                    if (j == pipes_nb - 1) // doit alors écrire sur l'outfile donné en argument
                    {
                        close(pipefd[j][1]);
                        fd = in_out[1];
                    }
                    else
                    {
                        close(in_out[1]);
                        fd = pipefd[j][1];
                    }
                    close(pipefd[j][0]);
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
                /* else
                {
                    close()
                } */
              /*   if (close(pipefd[j][0]) == -1)
                    perror("close ");
                if (close(pipefd[j][1]) == -1)
                    perror("close "); */
                j++;
            }
            printf("i = %d \n my pid is %d\n", i, pid[i]);
            if (execve(commands[i][0], commands[i], envp) == -1)
                return (perror("execve "), 3);
            exit(0); // we only want to fork in the parent process ; we therefore terminate each child process after they executed their assignated command, or else they would stay in the while loop and create child processes of their own as well
            
        }
        //printf("parent \n i = %d \n my pid is %ld\n", i, pid[i]);
        i++;
    }
    if (close(in_out[0]) == -1)
        perror("close ");
    if (close(in_out[1]) == -1)
        perror("close ");
    i = 0;
    while (i < processes_nb)
    {
        waitpid(pid[i], &wstatus, 0);
        /*
        if (waitpid(pid[i], &wstatus, 0) == -1)
            perror("waitpid ");
        if (! WIFEXITED(wstatus))
            perror("child process terminated anormally ");
        */
        i++;
    }
    perror("e ici\n");
    return (0); 
}

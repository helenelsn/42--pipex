/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 02:31:34 by Helene            #+#    #+#             */
/*   Updated: 2023/04/08 17:38:14 by Helene           ###   ########.fr       */
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

size_t get_path(char *command, char **envp, char ***commands, int j)
{
    int i;
    char *path;
    char **paths;

    i = 0;
    while (envp[i] && !strsearch(envp[i], "PATH="))
        i++;
    path = ft_strjoin(".:", envp[i] + 5); // ./ est un chemin relatif : est-ce qu'access() gere ce genre de chemin ?
    if (!path)
        return (0);
    paths = ft_split(path, ':');
    
    i = 0;
    while (paths[i])
    {
        free(path);
        path = add_command(paths[i], command);
        //printf("path = %s\n", path);
        if (!access(path, X_OK)) // on success, 0 is returned
        {
            *commands[j] = ft_strdup(path);
            free(path);
            return (ft_strlen(*commands[j])); // free_tab(&paths);
        }
        i++;
    }
    
    
    return (free_tab(&paths), free(path), 0); // si aucun path n'est valide
}

// get the commands' binary's absolute paths (env | grep PATH)
char ***set_commands(int argc, char **argv, char **envp) // chaque argv[i] correspond a une commande
{
    char ***commands;
    char **command;
    int i;
    int j;
    
    i = 2; // 0 = a.out et 1 = infile
    commands = malloc(sizeof(char **) * (argc - 1));
    if (!commands)
        return (NULL);
    while (i < argc)
    {
        command = ft_split(argv[i], ' '); // command is null-terminated
        if (!command)
            return (perror("Error (tried to get the right path) "), NULL);
        j = 0;
        while (command[j])
            j++;
        commands[i] = malloc(sizeof(char *) * (j + 1));
        if (!commands[i])
            return (NULL);
        j = 0;
        get_path(command[j], envp, &commands[i], j);
        if (!commands[i][j])
            return (free_commands(&commands), free_tab(&command), NULL);
        while (command[++j])
        {
            commands[i][j] = ft_strdup(command[j]); // rajoute les options de la commande i
            if (!commands[i][j])
                return (free_commands(&commands), free_tab(&command), NULL);
        }
        //free_tab(&command);
        i++;
    }
    commands[i] = NULL;
    return (commands);
}

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

    commands = set_commands(argc - 1, argv, envp);
    if (!commands)
        return (4);
    
    pipes_nb = argc - 2;
    processes_nb = argc - 3; 
    i = 0;
    if (access(argv[1], F_OK | R_OK) == -1) // l'infile n'existe pas, ou ne peut pas etre lu
        return(4);
    in_out[0] = open(argv[1], O_RDONLY);
    if (in_out[0] == -1)
        return(perror("open "), 4);
    in_out[1] = open(argv[argc - 1], O_WRONLY, O_CREAT);
    if (in_out[1] == -1)
        return(perror("open "), 4);
    
    while (i < pipes_nb)
    {
        if (pipe(pipefd[i]) == -1)
            return (perror("pipe "), 1);
        i++;
    }
    i = 0;
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
                printf("ici\n");
                if (j == i)
                {
                    if (j == 0) // doit alors lire depuis d'infile donné en argument
                        dup2(in_out[0], STDIN_FILENO);
                    else
                        dup2(pipefd[j][0], STDIN_FILENO);
                }
                if (j == i + 1)
                {
                    if (j == pipes_nb - 1) // doit alors écrire sur l'outfile donné en argument
                        dup2(in_out[1], STDOUT_FILENO);
                    else
                        dup2(pipefd[j][1], STDOUT_FILENO);
                }
                if (close(pipefd[j][0]) == -1)
                    perror("close ");
                if (close(pipefd[j][1]) == -1)
                    perror("close ");
                j++;
            }
            if (execve(commands[i][0], commands[i], envp) == -1)
                return (perror("execve "), 3);
            return (0); // we only want to fork in the parent process ; we therefore terminate each child process after they executed their assignated command, or else they would stay in the while loop and create child processes of their own as well
        }
        i++;
    }
    if (close(in_out[0]) == -1)
        perror("close ");
    if (close(in_out[1]) == -1)
        perror("close ");
    i = 0;
    while (i < processes_nb)
    {
        if (waitpid(pid[i], &wstatus, 0) == -1)
            perror("waitpid ");
        if (! WIFEXITED(wstatus))
            perror("child process terminated anormally ");
        i++;
    }
    return (0); 
}

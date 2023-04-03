/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 02:31:34 by Helene            #+#    #+#             */
/*   Updated: 2023/04/03 20:04:03 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*join_strings(char *string1, char *string2, int free1)
{
	int		len1;
    int     len2;
	char	*tmp;
	int		i;

	
	i = -1;
	len1 = ft_strlen(string1);
	len2 = ft_strlen(string2);
	tmp = malloc(sizeof(char) * (len1 + len2 + 1));
	if (!tmp)
		return (NULL);
	while (++i < len1)
		tmp[i] = string1[i];
	i = -1;
	while (++i < len2)
		tmp[len1 + i] = string2[i];
	tmp[len1 + i] = '\0';
    if (!free1)
        free(string2);
    else
	    free(string1);
	return (tmp);
}

char *strsearch(char *string, char *to_find)
{
    int i;

    i = 0;
    while (string[i] && to_find[i])
    {
        if (string[i] != to_find[i])
            return (NULL);
        i++;
    }
    return ();
}

int 

t_command *new_elem()
{
    
}

void    get_path(char *command, char **envp)
{
    int i;
    int j;
    char *path;
    char **paths;

    i = 0;
    while (envp[i])
    {
        path = strsearch(envp[i], "PATH=");
        if (path)
            break;
    }
    path = join_strings("./:", path, 1); // ./ est un chemin relatif : est-ce qu'access() gere ce genre de chemin ?
    if (!path)
        return ;
    paths = ft_split(path, ':');
    i = 0;
    while (paths[i])
    {
        free(path);
        path = join_strings(paths[i], command);
        if (!access(path, X_OK)) // on success, 0 is returned
        {
            
        }
    }   
}

// get the commands' binary's absolute paths (env | grep PATH)
t_command    *set_commands(int argc, char **argv, char **envp) // chaque argv[i] correspond a une commande
{
    t_command *current;
    char **command;
    char **args;
    int i;
    int j;
    
    i = 2;
    current = new_elem;
    while (i < argc)
    {
        command = ft_split(argv[i], ' ');
        if (!command)
            return (strerror("Error : split\n"), 1);
        current->path = get_path(command[0], envp);
        j = 1;
        while (command[j])
        {
            
        }
        
    }
}

int main(int argc, char **argv, char **envp)
{
    t_command *current;
    
    int pipes_nb;
    int processes_nb;
    int pipefd[argc - 2][2];
    int pid[argc - 3];

    int in_out[2];
    int i;
    int j;
    int wstatus;

    //current = NULL;
    current = set_commands(argc - 1, argv, envp);
    if (!current)
        return (4);
    
    pipes_nb = argc - 2;
    processes_nb = argc - 3; 
    i = 0;
    in_out[0] = open(argv[1], O_RDONLY);
    in_out[1] = open(argv[argc - 1], O_WRONLY);
    
    while (i < pipes_nb)
    {
        if (pipe(pipefd[i]) == -1)
            return (perror("pipe : "), 1);
        i++;
    }
    i = 0;
    while (i < processes_nb && current)
    {
        pid[i] = fork();
        if (pid[i] == -1)
            return (perror("fork : "), 2);
        if (pid[i] == 0) // child process
        {
            j = 0;
            while (j < pipes_nb)
            {
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
                    perror("close : ");
                if (close(pipefd[j][1]) == -1)
                    perror("close : ");
                j++;
            }
            if (execve(current->path, current->args, envp) == -1)
                return (perror("execve : "), 3);
            return (0); // we only want to fork in the parent process ; we therefore terminate each child process after they executed their assignated command, or else they would stay in the while loop and create child processes of their own as well
        }
        current = current->next;
        i++;
    }
    if (close(in_out[0]) == -1)
        perror("close : ");
    if (close(in_out[1]) == -1)
        perror("close : ");
    i = 0;
    while (i < processes_nb)
    {
        if (waitpid(pid[i], &wstatus, 0) == -1)
            perror("waitpid : ");
        if (! WIFEXITED(wstatus))
            strerror("Child process terminated anormally\n");
        i++;
    }
    return (0); 
}

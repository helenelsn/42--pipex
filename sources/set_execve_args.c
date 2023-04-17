/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_execve_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 17:08:18 by hlesny            #+#    #+#             */
/*   Updated: 2023/04/17 17:19:31 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
        if (!access(path, F_OK) && !access(path, X_OK)) // on success, 0 is returned
            return (free_tab(paths), free(command), path);
        i++;
    }
    return (free_tab(paths), free(path), command); // si aucun path n'est valide
}

// get the commands' binary's absolute paths (env | grep PATH)
char ***set_commands(int cmds_count, char **cmds, char **envp) // chaque cmds[i] correspond a une commande
{
    char ***commands;
    int i;
    int j;
    
    i = 0;
    commands = malloc(sizeof(char **) * (cmds_count + 1));
    if (!commands)
        return (NULL);
    while (i < cmds_count)
    {
        if (cmds[i])
        {  
            commands[i] = ft_split(cmds[i], ' '); // command is null-terminated
            if (!commands[i])
                return (perror("ft_split "), free_commands(commands), NULL);
            j = 0;
            commands[i][0] = get_path(commands[i][0], envp);
        }
        else
            commands[i] = NULL;
        // if (!commands[i][0]) // ie si le binaire n'existe pas ou n'est pas executable
        //     return(free_commands(commands), NULL);
        i++;
    }
    commands[i] = NULL;
    return (commands);
}

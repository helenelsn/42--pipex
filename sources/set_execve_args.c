/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_execve_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 17:08:18 by hlesny            #+#    #+#             */
/*   Updated: 2023/04/24 01:47:10 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// add the command string to the path string
char	*add_command(char *path, char *command)
{
	int		len1;
	int		len2;
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

int	strsearch(char *string, char *to_find)
{
	int	i;

	i = 0;
	while (string[i] && to_find[i])
	{
		if (string[i] != to_find[i])
			return (0);
		i++;
	}
	if (to_find[i])
		return (0);
	return (1);
}

char	*get_path(char *command, char **envp)
{
	int		i;
	char	*path;
	char	**paths;

	i = 0;
	while (envp[i] && !strsearch(envp[i], "PATH="))
		i++;
	if (!envp[i])
		return (ft_putstr_fd("Error : No PATH variable\n", STDERR_FILENO),
			command);
	path = ft_strjoin(".:", envp[i] + 5);
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	i = 0;
	while (paths[i])
	{
		free(path);
		path = add_command(paths[i], command);
		if (!access(path, F_OK) && !access(path, X_OK))
			return (free_tab(paths), free(command), path);
		i++;
	}
	return (free_tab(paths), free(path), command);
}

// get the commands' binary's absolute paths (env | grep PATH)
char	***set_commands(int cmds_count, char **cmds, char **envp)
	// chaque cmds[i] correspond a une commande
{
	char	***commands;
	int		i;

	i = 0;
	commands = malloc(sizeof(char **) * (cmds_count + 1));
	if (!commands)
		return (NULL);
	while (i < cmds_count)
	{
		if (cmds[i])
		{
			commands[i] = ft_split(cmds[i], ' ');
			if (!commands[i])
				return (perror("ft_split "), free_commands(commands), NULL);
			commands[i][0] = get_path(commands[i][0], envp);
		}
		else
			commands[i] = NULL;
		i++;
	}
	commands[i] = NULL;
	return (commands);
}

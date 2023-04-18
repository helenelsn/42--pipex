/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 23:04:25 by Helene            #+#    #+#             */
/*   Updated: 2023/04/18 23:05:33 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void     f_close(int fd)
{
    if (fd < 0)
        exit(6);
    if (close(fd) == -1)
    {
        perror("close ");
        exit(7); // exit maintenant ? 
    }
}

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

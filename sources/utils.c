/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 23:04:25 by Helene            #+#    #+#             */
/*   Updated: 2023/04/20 23:14:56 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void     f_close(int fd)
{
    if (fd < 0)
        exit(EXIT_FAILURE);
    if (close(fd) == -1)
    {
        perror("close ");
        exit(EXIT_FAILURE); // exit maintenant ? 
    }
}

int     f2_close(int fd)
{
    if (fd < 0)
        return(EXIT_FAILURE);
    if (close(fd) == -1)
    {
        perror("close ");
        return(EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

void    f_dup2(int fd1, int fd2)
{
    if (fd1 < 0 || fd2 < 0)
        exit(EXIT_FAILURE);
    if (dup2(fd1, fd2) == -1)
    {
        perror("dup2 ");
        exit(EXIT_FAILURE);
    }
}

// void    close_c(int fd) // i
// {
//     if (fd < 0)
//         return (EXIT_FAILURE);
//     if (close(fd) == -1)
//     {
//         perror("close ");
//         return (EXIT_FAILURE);
//     }
// }
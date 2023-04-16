/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 17:07:03 by hlesny            #+#    #+#             */
/*   Updated: 2023/04/16 17:05:42 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void    read_input(int fd);

int contains_limiter(char *line, char *limiter) // returns -1 if line doesn't contain it, and the index of the limiter in line if it does
{
    int i;
    int j;
    int k;

    i = 0;
    while (line[i])
    {
        j = 0;
        while (line[i] && line[i] != limiter[j])
            i++;
        k = i;
        while (line[k] && line[k] == limiter[j])
            (k++, j++);
        if (!limiter[j]) // ie si est sorti du while en ayant entièrement parcouru limiter
            return (i);
        if (line[i])
            i++;
        //fprintf(stderr, "i = %d, line[i] = %c\n", i, line[i]);
    }
    return (-1);
}

// returns >= 0 (the infile fd) in case of success, -1 in case of failure or nonexistence of here_doc argument
int test_here_doc(char *arg1, char *limiter)
{
    char *line;
    int infile;
    int i;
    int j;
    
    if (!strsearch(arg1, "here_doc"))
        return (-1);
    
    infile = open("pipex_infile", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR); 
    if (infile == -1)
    {
        perror("open");
        exit(-1); // ? 
    }
    line = get_next_line(STDIN_FILENO);
    while (line && contains_limiter(line, limiter) == -1)
    {
        //fprintf(stderr, "contains_limiter = %d\n", contains_limiter(line, limiter));
        ft_putstr_fd(line, infile);
        free(line);
        line = get_next_line(STDIN_FILENO);
    }
    i = -1;
    j = contains_limiter(line, limiter);
    //fprintf(stderr, "contains_limiter = %d\n", j);
    if (line && j >= 0) // ie est sorti du while car est tombé sur le limiter
    {
        while (++i < j)
            ft_putchar_fd(line[i], infile);
        free(line);
        read_input(infile);
        dup2(infile, STDIN_FILENO);
        close(infile);
        return (infile);
    }
    close(infile);
    return (-1); // si a aucun moment 
}
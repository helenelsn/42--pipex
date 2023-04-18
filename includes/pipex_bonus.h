/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 16:45:41 by Helene            #+#    #+#             */
/*   Updated: 2023/04/18 16:47:56 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

#include "pipex.h"

int     test_here_doc(char *arg1, char *limiter, int *in);
void    here_doc_case(t_fork_data *data, char **argv, int last);
void    no_here_doc_case(t_fork_data *data, char **argv, int last);

# endif
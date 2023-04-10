/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 02:31:34 by Helene            #+#    #+#             */
/*   Updated: 2023/04/10 21:00:10 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_next_line(int fd);

void    free_tab(char **command)
{
    int i;

    i = 0;
    while (command[i])
    {
        free(command[i]);
        i++;
    }
}

void    free_commands(char ***commands) // char *** ou char **** ?
{
    int i;

    i = 0;
    while (commands[i])
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
    return (free_tab(paths), free(path), NULL); // si aucun path n'est valide
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
            return (perror("set_command() "), NULL);
        j = 0;
        commands[i][0] = get_path(commands[i][0], envp);
        if (!commands[i][0]) // ie si le binaire n'existe pas ou n'est pas executable
            return(free_commands(commands), NULL);
        i++;
        //printf("setting command %d...\n", i);
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


void    read_input(int fd)
{
    char *line;

    printf("fd = %d\n", fd);
    line = get_next_line(fd);
    while (line)
    {
        printf("%s", line);
        free(line);
        line = get_next_line(fd);
    }
    printf("\n");
    free(line);
}

int contains_limiter(char *string, char *limiter)
{
    return (0);
}

// returns >= 0 (the infile fd) in case of success, -1 in case of failure or nonexistence of here_doc argument
int test_here_doc(char *arg1, char *limiter)
{
    char *line;
    int infile;
    
    if (!strsearch(arg1, "here_doc"))
        return (-1);
    
    infile = open("pipex_infile", O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR); 
    if (infile == -1)
    {
        perror("open");
        exit(-1); // ? 
    }
    line = get_next_line(STDIN_FILENO);
    while (line && !contains_limiter(line, limiter)) // && condition sur la présence de "LIMITER" dans line
    {
        ft_putstr_fd(line, infile);
        free(line);
        line = get_next_line(STDIN_FILENO);
    }
    if (line) // ie est sorti du while car est tombé sur le limiter
    {
        
        free(line);
    }
    return (infile);
}

int main(int argc, char **argv, char **envp)
{
    char ***commands;
    
    int processes_nb;
    int pipefd[2];
    int pid[argc - 3];
    int wstatus;

    int i;
    int j;
    int in_out[2];
    int here_doc;

    if (argc < 5)
    {
        ft_putstr_fd("Not enough arguments in command line!\n", STDERR_FILENO);
        return (5);
    }
    
    here_doc = test_here_doc(argv[1], argv[2]);
    
    if (here_doc == -1)
    {
        if (access(argv[1], F_OK | R_OK) == -1) // l'infile n'existe pas, ou ne peut pas etre lu
            return(perror("access"), 4);
        in_out[0] = open(argv[1], O_RDONLY);
        if (in_out[0] == -1)
            return(perror("open "), 4);   
        in_out[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
        if (in_out[1] == -1)
        {
            close(in_out[0]);
            return(perror("open "), 4);
        }
    }
    else
    {
        in_out[0] = here_doc; // re vérifier si est bien >= 0 ? 
        in_out[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
        if (in_out[1] == -1)
        {
            close(in_out[0]);
            return(perror("open "), 4);
        }
    }
    commands = set_commands(argc - 3, argv + 2, envp);
    if (!commands)
        return (perror("Error while setting commands' paths"), 4);
    processes_nb = argc - 3; 
    i = 0;
    dup2(in_out[0], STDIN_FILENO);
    dup2(in_out[1], STDOUT_FILENO);
    close(in_out[0]);
    close(in_out[1]);
    
    while (i < processes_nb && commands[i])
    {
        if (i < processes_nb - 1)
        {
            if (i)
            {
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]);
                close(pipefd[1]); // ?
            }
            if (pipe(pipefd) == -1) // pipefd[0] -- read // pipefd[1] : write
                return (perror("pipe"), 2);   
        }
        //printf("ok, i = %d\n", i);
        pid[i] = fork();
        if (pid[i] == -1)
            return (perror("fork "), 2);
        if (pid[i] == 0) // child process
        {
            //sleep(1);
            //fprintf(stderr, "ok, child %d\n", i);
            if (i == 0) // in that case, we want to read from the infile rather than from pipefd[0]
            {
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }
            else if (i == processes_nb - 1) // we want to write in the outfile, rather than in pipefd[1]
            {
                close(pipefd[1]);
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]);
            }
            else
            {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
                close(pipefd[0]); //?
            }
            fprintf(stderr, "i = %d, my pid is %d, my parent is %d\n", i, getpid(), getppid());
            if (execve(commands[i][0], commands[i], envp) == -1)
                return (perror("execve "), 3);
            exit(0); // we only want to fork in the parent process ; we therefore terminate each child process after they executed their assignated command, or else they would stay in the while loop and create child processes of their own as well
            
        }
        // else
        // {
        //     //fprintf(stderr, "i = %d, my pid is %d, my child's pid is %d\n", i, getpid(), pid[i]);
        //     //prev_pipe = pipefd[0];
        //     if (close(pipefd[0]) == -1)
        //         perror("close cc1");
        //     if (close(pipefd[1]) == -1)
        //         perror("close cc2");
        // }
        //printf("parent \n i = %d \n my pid is %ld\n", i, pid[i]);
        i++;
    }
    i = 0;
    //close(pipefd[0]);
    //close(pipefd[1]);
    /*
    if (close(in_out[0]) == -1)
        perror("close");
    if (close(in_out[1]) == -1)
        perror("close");
    */
   if (close(pipefd[0]) == -1)
        perror("close");
    if (close(pipefd[1]) == -1)
        perror("close");
    i = 0;
    while (i < processes_nb)
    {
        //waitpid(pid[i], &wstatus, 0);
        
        if (waitpid(pid[i], &wstatus, 0) == -1)
            perror("waitpid ");
        //if (!WIFEXITED(wstatus))
        //    perror("wifexited ");
        fprintf(stderr, "done waiting for child %d\n", i);
        i++;
    }
    sleep(10);
    return (WIFEXITED(wstatus) & WEXITSTATUS(wstatus)); 
}

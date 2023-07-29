/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarks <jmarks@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 14:34:44 by jmarks            #+#    #+#             */
/*   Updated: 2023/07/28 22:03:46 by jmarks           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern	t_program	g_program;

char	**realloc_back(char **arr, char *delim)
{
	char	**ret;
	int		i;
	int		len;

	if (!delim || !ft_strcmp(arr[0], delim))
		return (arr);
	i = 0;
	while (arr[i] && ft_strcmp(arr[i], delim))
		i++;
	len = i;
	while (arr[len] && ft_strcmp(arr[len], ""))
		len++;
	ret = malloc(sizeof(*ret) * (len - i + 1));
	len = 0;
	while (arr[i] && ft_strcmp(arr[i], ""))
		ret[len++] = ft_strdup(arr[i++]);
	ret[len] = NULL;
	ft_free_array(arr);
	if (!ret[1] && !ft_strcmp(ret[0], ")"))
	{
		ft_free_array(ret);
		return (NULL);
	}
	return (ret);
}


void last_command(void)
{
	int pid;

	if (!g_program.pid)
			execmd(&g_program);
		pid = fork();
		if (!pid)
			execmd(&g_program);
		waitpid(g_program.pid, &g_program.exit_status, 0);
		waitpid(pid, &g_program.exit_status, 0);
		ft_free_array(g_program.token);
}

void	set_commands(void)
{
    debugFunctionName("SET_CMDS");
    char **tokens;
    int i;
	int j;

    tokens = g_program.token;
	i = 0; 
	
	while (tokens[i] && ft_strcmp("|", tokens[i]))
		i++;
	if (!tokens[i])
    {
        g_program.commands = tokens;
    }
	else
	g_program.commands = malloc(sizeof(*g_program.commands) * (i + 1));
	j = 0;
	while (j < i)
	{
		g_program.commands[j] = tokens[j];
		j++;
	}
    g_program.commands[i] = NULL;
}

void	execute_commands(void)
{
	int	i;

	set_commands();
	do_pipe();
	i = -1;
	while (g_program.token[i] && ft_strcmp("|", g_program.token[i]))
	{
		i++;
	}
	if (!ft_strcmp("|", g_program.token[i]))
	{
		g_program.token = realloc_back(g_program.token, "|");
		g_program.token = realloc_back(g_program.token,
				g_program.token[0]);
	}
	i = -1;
	free(g_program.commands);
}

void handle_pipe(void) 
{
    debugFunctionName("HANDLE_PIPE");
    // t_token_list *curr = root;
    int i = 0;
	if (!g_program.token[i])
	exit (0);
    while (g_program.token[i])
    {
        if (!ft_strcmp("|", g_program.token[i])) 
        {
            execute_commands();
			i = -1;
		}
		i++;
	}
	g_program.commands = g_program.token;
	last_command();
}

void do_pipe(void)
{
    debugFunctionName("DO_PIPE");

    int pipe_fd[2];
	int pid;
    pipe(pipe_fd);
    pid = fork();
    if (pid < 0)
    {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    if (pid)
    {
        // Child process
        if (!g_program.pid)
        {
			g_program.pid = pid;
			close(g_program.pipe_fd[1]);
            if (dup2(g_program.pipe_fd[0], STDIN_FILENO) < 0)
            {
                perror("Error");
                exit(EXIT_FAILURE);
            }
            close(g_program.pipe_fd[0]);
        }
        else
        {
            close(pipe_fd[0]);
            if (dup2(pipe_fd[1], STDOUT_FILENO) < 0)
            {
                perror("Error");
                exit(EXIT_FAILURE);
            }
            close(pipe_fd[1]);
			execmd(&g_program);
        }
    }
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarks <jmarks@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 15:30:25 by jmarks            #+#    #+#             */
/*   Updated: 2023/06/20 18:26:35 by jmarks           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_program g_program;

char    *join_cmd(char *path, char *cmd)
{
    char    *tmp;
    char    *cmd_path;

    //append "/" to the directory
    tmp = ft_strjoin(path, "/");
    //append the command name 
    cmd_path = ft_strjoin(tmp, cmd);
    return (cmd_path);
}

char	*get_command(char *path)
{
	char	*tmp;
	int		i;

	i = 0;
	while (g_program.path[i])
	{
        tmp = join_cmd(g_program.path[i], path);
        //check if the command exists and if it does return it
		if (access(tmp, F_OK) == 0)
		{
			tmp = path;
            return (path);
		}
		free(tmp);
		i++;
	}
	return (NULL);
}

void execmd(char **cmds)
{
    debugFunctionName("EXECMD");
    char *path;
    pid_t pid;

    pid = fork();
    if (pid < 0)
    {
        perror("Error: fork failed\n");
    }
    else if (pid == 0)
    {
    path = getenv("PATH");
    g_program.path = ft_split(path, ':');
    // free(path);
    rebuild_envp();
    // path = g_program.token[0];
    path = get_command(path);
    if (path)
    {
        execve(path, cmds, g_program.envp);
    }
    printf("%s: command not found\n", path);
        g_program.exit_status = 1;
        exit(1);
    }
    else
    {
            wait(NULL);
            return;
    }
}

void	pipex(void)
{
	int		fd[2];
	pid_t 	pid;

	pipe(fd);
	pid = fork();
	if (pid < 0)
		printf("Error: did not fork\n");
	if (pid)
	{
		close(fd[1]);
		dup2(fd[1], 0);
		close(fd[0]);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], 1);
		close(fd[0]);
        execmd(g_program->token);
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarks <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 15:00:49 by jmarks            #+#    #+#             */
/*   Updated: 2023/06/27 15:00:51 by jmarks           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_program g_program;

int std_input(char *file)
{
	int fd;

	fd = open(file, O_RDWR, | O_CREAT | O_TRUNC, 0644)
	if (fd < 0)
	{
		perror("Error: \n");
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	remove_redirect("<");
	execve();
	return (-1);
}

int std_output(char *file)
{
	int	fd;

	error_redir("<" check what this does??)
	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror("Error: \n");
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	remove_redirect(">");
	execve();
	g_program.exit_status = 0;
}

int output_append(char *file)
{
	int fd;

	fd = open(file, O_CREAT | O_APPEND | O_WRONLY, 0644)
	if (fd < 0)
	{
		perror("Error: \n");
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	remove_redirect(">>");
	return ()
}

int	get_heredoc(char *end_of_input)
{
	char	*line;
	int		fd[2];

	if (pipe(fd) < 0)
		return (-1);
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
			return (-1);
		if (ft_strcmp(line, end_of_input) == 0)
			break ;
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
	}
	close(fd[1]);
	return (fd[0]);
}

void	remove_redirect(char *redirector)
{
	char	**new;
	int		i;
	int		j;

	i = 0;
	while (g_program.token[i])
		i++;
	new = malloc(sizeof(*new) * (i - 1));
	i = 0;
	j = 0;
	while (g_program.token[i])
	{
		if (!ft_strcmp(g_program.token[i], redirector))
			i += 2;
		if (!g_program.token[i])
			break ;
		new[j++] = ft_strdup(g_program.token[i++]);
	}
	new[j] = NULL;
	free_array(g_program.token);
	g_program.token = new;
}

int	input_heredoc(char *file, char *delim)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("Error: \n");
	}
	get_heredoc(delim);
	fd = open(file, O_RDWR, 0644);
	dup2(fd, STDIN_FILENO);
	close(fd);
	remove_redir("<<"); 
	return (-1);
}

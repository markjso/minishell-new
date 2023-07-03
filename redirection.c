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
	ft_free_array(g_program.token);
	g_program.token = new;
}

int std_output(t_program *program)
{
	debugFunctionName("STD_OUT");
	int fd;
	int stdout_fd;
	char *file;

	file = g_program.redirect_file;

	printf("Redirect File: %s\n", file);
    printf("Token 1: %s\n", program->token[1]);

	if (program->token[1][0] == '\0') // Check if only ">" is present
	{
		printf("Invalid command\n");
		return -1;
	}
	fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("Error: \n");
	}
	stdout_fd = dup(STDOUT_FILENO); 
	dup2(fd, STDOUT_FILENO);
	close(fd);
	dup2(stdout_fd, STDOUT_FILENO);
	close(stdout_fd);
	remove_redirect(">");
	// execmd(program);
	return (0);
	exit(g_program.exit_status);
}

// int std_input(char *file)
// {
// 	int	fd;

// 	fd = open(file, O_RDONLY);
// 	if (fd < 0)
// 	{
// 		perror("Error: \n");
// 	}
// 	dup2(fd, STDIN_FILENO);
// 	close(fd);
// 	remove_redirect("<");
// 	return(-1);
// }

// int output_append(char **tokens)
// {
// 	int fd;

// 	fd = open(file, O_CREAT | O_APPEND | O_WRONLY, 0644)
// 	if (fd < 0)
// 	{
// 		perror("Error: \n");
// 	}
// 	dup2(fd, STDIN_FILENO);
// 	close(fd);
// 	remove_redirect(">>");
// 	return ()
// }

// int	get_heredoc(char *end_of_input)
// {
// 	char	*line;
// 	int		fd[2];

// 	if (pipe(fd) < 0)
// 		return (-1);
// 	while (1)
// 	{
// 		line = readline("heredoc> ");
// 		if (!line)
// 			return (-1);
// 		if (ft_strcmp(line, end_of_input) == 0)
// 			break ;
// 		write(fd[1], line, ft_strlen(line));
// 		write(fd[1], "\n", 1);
// 	}
// 	close(fd[1]);
// 	return (fd[0]);
// }

// int	input_heredoc(char *file, char *delim)
// {
// 	int	fd;

// 	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd < 0)
// 	{
// 		perror("Error: \n");
// 	}
// 	get_heredoc(delim);
// 	fd = open(file, O_RDWR, 0644);
// 	dup2(fd, STDIN_FILENO);
// 	close(fd);
// 	remove_redir("<<"); 
// 	return (-1);
// }

int check_for_redirect(t_program *program)
{
    debugFunctionName("CHECK_REDIR");
    int i = 0;

    while (program->token[i])
    {
        if (!ft_strcmp(">", program->token[i]))
        {
            program->is_redirect = 1;
            return i; // Return the index of the redirect symbol
        }
        i++;
    }
    return -1; // No redirect symbol found
}

void do_redirect(t_program *program)
{
    debugFunctionName("DO_REDIR");
    int redirect_index = check_for_redirect(program);

    if (redirect_index >= 0)
    {
        if (program->token[redirect_index + 1] != NULL)
        {
            program->redirect_file = ft_strdup(program->token[redirect_index + 1]);
            std_output(program);
        }
        else
        {
            error_message("No file specified", 1);
        }
    }
    else
    {
        error_message("Not a redirect", 1);
    }
}
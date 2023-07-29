/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarks <jmarks@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 18:01:04 by jmarks            #+#    #+#             */
/*   Updated: 2023/07/28 20:22:54 by jmarks           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_program	g_program;

/*
If either STDOUT_FILENO or STDIN_FILENO were modified:
Reset to defualt. 
*/
void remove_redirect()
{
    debugFunctionName("REMOVE_REDIRECT");
	if (g_program.redir_out_flag == 1)
    {
        printf("REMOVE_REDIRECT OUT\n");
        close(g_program.out_file);
        dup2(g_program.out_backup, STDOUT_FILENO);
        printf("STDOUT_FILENO: %d\n", STDOUT_FILENO);
        close(g_program.out_backup);
        g_program.out_backup = -1;
        free(g_program.redirect_out);
        g_program.redir_out_flag = 0;
    }

    if (g_program.redir_in_flag == 1)
    {
        printf("REMOVE_REDIRECT IN\n");
        close(g_program.in_file);
        dup2(g_program.in_backup, STDIN_FILENO);
        printf("STDIN_FILENO: %d\n", STDIN_FILENO);
        close(g_program.in_backup);
        g_program.in_backup = -1;
        free(g_program.redirect_in);
        g_program.redir_in_flag = 0;
    }
	if (g_program.pipe_fd[0] != -1)
    {
        close(g_program.pipe_fd[0]);
        g_program.pipe_fd[0] = -1;
    }
    if (g_program.pipe_fd[1] != -1)
    {
        close(g_program.pipe_fd[1]);
        g_program.pipe_fd[1] = -1;
    }
}

/*
If a matching close quote is found: 
	Index will be moved to 1 char after the close quote. 

If no matching close quote is found: 
	Index will be moved to 1 char after the current loose quote. 
*/
void	locate_second_quote(char *str)
{
	debugFunctionName("LOCATE_SECOND_QUOTE");
	int		second;

	second = g_program.redirect_index + 1; // One after the quote
	while (str[second] != '\0') // For the length of the string
	{
		if (str[second] == str[g_program.redirect_index]) // If matching quote found
		{
			g_program.redirect_index = second + 1; // Make first be the index of the 2nd quote plus one next outside of the quote. 
			break ; // First will now be one after the matching quote. 
		}
		second++;
	}
	if (str[second] == '\0') // At end and therefore no matching quote was found.
	{
		g_program.redirect_index++; // One after the current quote. 
        return ; // Can remove this return if norm error. 
	}
}

char	*get_file_name(char *str)
{
	debugFunctionName("GET_FILE_NAME");
	int 	end_of_name;
	char	*file_name;

	g_program.redirect_index++;
	while (ft_is_white_space(str[g_program.redirect_index]))
		g_program.redirect_index++;
	end_of_name = g_program.redirect_index;
	while (ft_is_not_white_space(str[end_of_name]) == 1)
		end_of_name++;	
	file_name = ft_substr(str, g_program.redirect_index, end_of_name - g_program.redirect_index);
	if (file_name)
		return (file_name); // MALLOC
	else
		return (NULL);
}
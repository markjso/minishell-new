/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialise.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarks <jmarks@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 15:44:30 by jmarks            #+#    #+#             */
/*   Updated: 2023/06/20 18:08:12 by jmarks           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	init_free(char *input)
// {
// 	if (input)
// 		free(input);
// 	// if (temp)
// 	// 	free(temp);
	
// }

void	get_user_prompt(void)
{
	char	*prompt;
	char	*username;

	username = getenv("USER");
	prompt = ft_strjoin(username, "@>>$ ");
	g_program.prompt = prompt;
	free (prompt);
}

int	take_input(void)
{
	char	*input;

	get_user_prompt();
	input = readline(g_program.prompt); // Return is MALLOCED
	if (!input) // Input is username and '$'.  IF username doesn't exist or Ctrl-d, exit program in error. 
	{
		printf("%s exit\n", input);
		free(input);
		exit(1);
	}
	if (ft_strlen(input) != 0) // If user inputs text, even nonsense, this is called. 
	{
		add_history(input);
		// ft_strlcpy(str, input, ft_strlen(input) + 1);
		free(input);
		return (0);
	}
	else // Else the user inputed nothing. 
	{
		free(input);
		return (1);
	}
}

/* Initialse PWD and OLDPWD to be used in the "cd" butilin
This ensures that they are properly set when the shell
starts up*/

void init_env_vars(void)
{
	char cwd[256];
    t_envar *pwd;
    t_envar *oldpwd;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        printf("Failed to get current directory\n");
        exit(1);
    }
	pwd = init_env(ft_strdup("PWD"), ft_strdup(cwd));
    oldpwd = init_env(ft_strdup("OLDPWD"), ft_strdup(cwd));
    
    add_env_var(pwd);
    add_env_var(oldpwd);
}

/* sets up the g_program global structure
allocates memory to token to hold an array
of char pointers and sets each element to NULL.
This ensures that array is empty and ready to 
store data.*/

void init_global(void)
{
    g_program.token = (char **)malloc((MAXLIST + 1) * sizeof(char *));
    for (int i = 0; i < MAXLIST + 1; i++) {
        g_program.token[i] = NULL;
	g_program.envp = NULL;
	g_program.exit_status = 0;
	init_env_vars();
    }
}
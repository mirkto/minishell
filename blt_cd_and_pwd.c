/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_cd_and_pwd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 20:37:57 by ngonzo            #+#    #+#             */
/*   Updated: 2020/12/30 20:37:59 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		blt_pwd(t_param *all)
{
	if (check_options(all) == TRUE)
		return (put_error("Enter without any options!", NULL));
	all->tmp = getcwd(NULL, 0);
	ft_putendl(all->tmp);
	free(all->tmp);
	return (0);
}

int		blt_cd_replace(t_param *all, char *str)
{
	int		i;
	int		len;

	len = ft_strlen(str);
	i = -1;
	while (all->env[++i])
		if (ft_strncmp(all->env[i], str, len) == 0)
		{
			if (all->env[i][len] == '=' || all->env[i][len] == '\0')
			{
				free(all->env[i]);
				all->env[i] = ft_strstrjoin(str, "=", all->tmp);
			}
		}
	return (0);
}

int		blt_cd_home(t_param *all)
{
	if (all->cmd[1] == NULL ||
		(all->cmd[1][0] == '~' && all->cmd[1][1] == '\0'))
	{
		all->tmp = get_value_env(all, "HOME");
		if (chdir(all->tmp))
			return (put_error("HOME not set", "cd"));
		free(all->tmp);
		return (1);
	}
	return (0);
}

int		blt_cd_return(t_param *all)
{
	if (all->cmd[1][0] == '-' && all->cmd[1][1] == '\0')
	{
		if (!(all->tmp = get_value_env(all, "OLDPWD")))
			return (put_error("OLDPWD not set", "cd"));
		ft_putendl(all->tmp);
		free(all->cmd[1]);
		all->cmd[1] = all->tmp;
	}
	return (0);
}

int		blt_cd(t_param *all)
{
	int	ret;

	if ((ret = blt_cd_home(all)) != 0)
		return (ret);
	if ((ret = blt_cd_return(all)) == ERROR)
		return (ERROR);
	all->tmp = getcwd(NULL, 0);
	if (chdir(all->cmd[1]) == 0)
	{
		blt_cd_replace(all, "OLDPWD");
		free(all->tmp);
		all->tmp = getcwd(NULL, 0);
		blt_cd_replace(all, "PWD");
	}
	else
	{
		free(all->tmp);
		return (put_error("No such file or directory", all->cmd[1]));
	}
	free(all->tmp);
	return (0);
}

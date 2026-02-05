/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <elfranco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 13:12:49 by elfranco          #+#    #+#             */
/*   Updated: 2026/01/13 13:12:51 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

char	*ft_strdup(const char *src)
{
	char		*newstr;
	size_t		counter;

	if (!src)
		return (NULL);
	newstr = (char *)malloc((ft_strlen(src) + 1) * sizeof(char));
	if (!newstr)
		return (NULL);
	counter = 0;
	while (src[counter] != '\0')
	{
		newstr[counter] = src[counter];
		counter++;
	}
	newstr[counter] = '\0';
	return (newstr);
}

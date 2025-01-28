/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encrypt_password.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 11:57:42 by jesuserr          #+#    #+#             */
/*   Updated: 2025/01/28 12:45:28 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/ft_ssl.h"

// Read and verify password from user and store it in 'args->pass'.
void	read_password(t_encrypt_args *args)
{
	char	buffer[1024];

	if (readpassphrase("enter encryption password:", buffer, sizeof(buffer), \
	RPP_ECHO_OFF) == NULL)
		print_encrypt_strerror_and_exit("readpassphrase", args);
	args->pass = ft_strdup(buffer);
	if (readpassphrase("Verifying - enter encryption password:", buffer, \
	sizeof(buffer), RPP_ECHO_OFF) == NULL)
		print_encrypt_strerror_and_exit("readpassphrase", args);
	if (ft_strncmp(args->pass, buffer, ft_strlen(args->pass)) != 0)
	{
		ft_printf("Passwords don't match\n");
		errno = EINVAL;
		print_encrypt_strerror_and_exit("DES", args);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encrypt_password.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 11:57:42 by jesuserr          #+#    #+#             */
/*   Updated: 2025/02/04 15:33:48 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_ssl.h"

// Read and verify password from user and store it in 'args->pass'.
void	read_password_from_console(t_encrypt_args *args)
{
	char	buffer[1024];

	if (readpassphrase("enter encryption password:", buffer, sizeof(buffer), \
	RPP_ECHO_OFF) == NULL)
		print_encrypt_strerror_and_exit("readpassphrase", args);
	args->pass = ft_strdup(buffer);
	if (readpassphrase("Verifying - enter encryption password:", buffer, \
	sizeof(buffer), RPP_ECHO_OFF) == NULL)
		print_encrypt_strerror_and_exit("readpassphrase", args);
	if (ft_strlen(args->pass) != ft_strlen(buffer) || \
	(ft_strncmp(args->pass, buffer, ft_strlen(args->pass)) != 0))
	{
		errno = EINVAL;
		print_encrypt_strerror_and_exit("Password verification error", args);
	}
}

// Reading from /dev/urandom is the most secure way to generate a random salt.
// Better than using 'rand' or 'srand' functions.
void	generate_salt(uint8_t *salt, t_encrypt_args *args)
{
	int	fd;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		print_encrypt_strerror_and_exit("/dev/urandom", args);
	if (read(fd, salt, KEY_LENGTH) < 0)
		print_encrypt_strerror_and_exit("/dev/urandom", args);
	close(fd);
}

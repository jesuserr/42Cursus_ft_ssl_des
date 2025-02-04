/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encrypt_pbkdf2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:03:09 by jesuserr          #+#    #+#             */
/*   Updated: 2025/02/04 12:12:12 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_ssl.h"

static void	derive_key_pbkdf2(const char *password, const unsigned char *salt, \
		int salt_len, int iterations, int key_len, unsigned char *output_key)
{
	if (PKCS5_PBKDF2_HMAC(password, strlen(password), salt, salt_len, \
	iterations, EVP_sha256(), key_len, output_key) == 0)
		fprintf(stderr, "Error generating key with PBKDF2\n");
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

void	generate_derived_key(t_encrypt_args *args)
{
	if (args->salt_provided)
		convert_str_to_hex(args->salt, args->hex_salt);
	else
		generate_salt(args->hex_salt, args);
	derive_key_pbkdf2(args->pass, args->hex_salt, KEY_LENGTH, ITERATIONS, \
	KEY_LENGTH, args->hex_key);
	//ft_printf("Hex salt: ");
	//print_hex_bytes(args->hex_salt, 0, KEY_LENGTH - 1);
	//ft_printf("\nDerived Key: ");
	//print_hex_bytes(args->hex_key, 0, KEY_LENGTH - 1);
}

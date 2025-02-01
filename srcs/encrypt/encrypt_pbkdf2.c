/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encrypt_pbkdf2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:03:09 by jesuserr          #+#    #+#             */
/*   Updated: 2025/02/01 14:21:23 by jesuserr         ###   ########.fr       */
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

void	generate_derived_key(t_encrypt_args *args)
{
	ft_printf("Generating key\n");
	ft_printf("Password: %s\n", args->pass);
	ft_printf("Salt: %s\n", args->salt);
	if (args->salt_provided)
	{
		convert_str_to_hex(args->salt, args->hex_salt);
		derive_key_pbkdf2(args->pass, args->hex_salt, KEY_LENGTH, ITERATIONS, \
		KEY_LENGTH, args->hex_key);
	}
	else
		derive_key_pbkdf2(args->pass, NULL, 0, ITERATIONS, KEY_LENGTH, \
		args->hex_key);
	ft_printf("Hex salt: ");
	print_hex_bytes(args->hex_salt, 0, KEY_LENGTH - 1);
	ft_printf("\nDerived Key: ");
	print_hex_bytes(args->hex_key, 0, KEY_LENGTH - 1);
	ft_printf("\n");
	ft_hex_dump(&args->hex_salt, KEY_LENGTH, 8);
}

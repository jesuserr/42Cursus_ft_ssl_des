/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encrypt_pbkdf2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:03:09 by jesuserr          #+#    #+#             */
/*   Updated: 2025/02/06 19:02:10 by jesuserr         ###   ########.fr       */
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
	derive_key_pbkdf2(args->pass, args->hex_salt, BLOCK_LENGTH, ITERATIONS, \
	BLOCK_LENGTH, args->hex_key);
	//ft_printf("Hex salt: ");
	//print_hex_bytes(args->hex_salt, 0, BLOCK_LENGTH - 1);
	//ft_printf("\nDerived Key: ");
	//print_hex_bytes(args->hex_key, 0, BLOCK_LENGTH - 1);
}

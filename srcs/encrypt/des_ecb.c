/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_ecb.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 11:18:14 by jesuserr          #+#    #+#             */
/*   Updated: 2025/02/01 13:45:21 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_ssl.h"

// Main function for des-ecb encryption/decryption.
//ft_hex_dump(args, sizeof(t_encrypt_args), 8);
void	des_ecb(t_encrypt_args *args)
{
	if (args->encrypt_mode)
	{
		ft_printf("Encrypting with DES-ECB\n");
		if (!args->key_provided)
			generate_derived_key(args);
		else
			convert_str_to_hex(args->key, args->hex_key);
	}
	else if (args->decrypt_mode)
		ft_printf("Decrypting with DES-ECB\n");
}

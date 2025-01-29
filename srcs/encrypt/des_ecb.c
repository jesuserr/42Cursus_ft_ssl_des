/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_ecb.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 11:18:14 by jesuserr          #+#    #+#             */
/*   Updated: 2025/01/29 11:29:18 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_ssl.h"

// Main function for des-ecb encryption/decryption.
void	des_ecb(t_encrypt_args *args)
{
	if (args->encrypt_mode)
	{
		ft_printf("Encrypting with DES-ECB\n");
		ft_hex_dump(args, sizeof(t_encrypt_args), 8);
	}
	else if (args->decrypt_mode)
	{
		ft_printf("Decrypting with DES-ECB\n");
		ft_hex_dump(args, sizeof(t_encrypt_args), 8);
	}
}

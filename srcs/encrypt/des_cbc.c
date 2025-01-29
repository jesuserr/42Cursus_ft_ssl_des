/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_cbc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 11:42:33 by jesuserr          #+#    #+#             */
/*   Updated: 2025/01/29 11:45:45 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_ssl.h"

// Main function for des-cbc encryption/decryption.
void	des_cbc(t_encrypt_args *args)
{
	if (args->encrypt_mode)
	{
		ft_printf("Encrypting with DES-CBC\n");
		ft_hex_dump(args, sizeof(t_encrypt_args), 8);
	}
	else if (args->decrypt_mode)
	{
		ft_printf("Decrypting with DES-CBC\n");
		ft_hex_dump(args, sizeof(t_encrypt_args), 8);
	}
}

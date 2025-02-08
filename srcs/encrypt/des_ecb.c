/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_ecb.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 11:18:14 by jesuserr          #+#    #+#             */
/*   Updated: 2025/02/08 18:58:34 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_ssl.h"

// Cyphered text is limited to 4096 bytes. Should be dynamically allocated
// and freed. Size changes depending on salt too.
void	des_ecb_encrypt(t_encrypt_args *args)
{
	EVP_CIPHER_CTX	*ctx;
	int				len;
	int				ciphertext_len;
	unsigned char	ciphertext[4096];
	OSSL_PROVIDER	*legacy_provider;

	legacy_provider = OSSL_PROVIDER_load(NULL, "legacy");
	OSSL_PROVIDER_load(NULL, "legacy");
	ctx = EVP_CIPHER_CTX_new();
	EVP_EncryptInit_ex(ctx, EVP_des_ecb(), NULL, args->hex_key, NULL);
	EVP_EncryptUpdate(ctx, ciphertext, &len, (unsigned char *)args->message, \
	args->message_length);
	ciphertext_len = len;
	EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
	ciphertext_len += len;
	EVP_CIPHER_CTX_free(ctx);
	EVP_cleanup();
	OSSL_PROVIDER_unload(legacy_provider);
	if (!args->salt_provided && !args->key_provided)
	{
		ft_memmove(ciphertext + SALT_TOTAL_LEN, ciphertext, ciphertext_len);
		ft_memcpy(ciphertext, SALT_STR, SALT_LENGTH);
		ft_memcpy(ciphertext + SALT_LENGTH, args->hex_salt, SALT_LENGTH);
		ciphertext_len += SALT_TOTAL_LEN;
	}
	if (args->base64_mode)
		encode_encrypted_message(args, ciphertext, ciphertext_len);
	else
		for (int i = 0; i < ciphertext_len; i++)
			ft_putchar_fd(ciphertext[i], args->output_fd);
	//ft_printf("Ciphertext: \n");
	//ft_hex_dump(ciphertext, ciphertext_len, 16);
}

// Plain text is limited to 4096 bytes. Should be dynamically allocated
// and freed.
void	des_ecb_decrypt(t_encrypt_args *args)
{
	EVP_CIPHER_CTX	*ctx;
	int				len;
	int				plaintext_len;
	unsigned char	plaintext[4096];
	OSSL_PROVIDER	*legacy_provider;

	legacy_provider = OSSL_PROVIDER_load(NULL, "legacy");
	OSSL_PROVIDER_load(NULL, "legacy");
	ctx = EVP_CIPHER_CTX_new();
	EVP_DecryptInit_ex(ctx, EVP_des_ecb(), NULL, args->hex_key, NULL);
	EVP_DecryptUpdate(ctx, plaintext, &len, (unsigned char *)args->message, \
	args->message_length);
	plaintext_len = len;
	EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
	plaintext_len += len;
	EVP_CIPHER_CTX_free(ctx);
	EVP_cleanup();
	OSSL_PROVIDER_unload(legacy_provider);
	for (int i = 0; i < plaintext_len; i++)
		ft_putchar_fd(plaintext[i], args->output_fd);
}

void	process_block_cipher(t_encrypt_args *args)
{
	uint8_t	permuted_msg[BLOCK_LENGTH];
	uint8_t	right_half[BLOCK_LENGTH / 2];
	uint8_t	left_half[BLOCK_LENGTH / 2];
	uint8_t	right_half_copy[BLOCK_LENGTH / 2];
	uint8_t	round;

	ft_bzero(permuted_msg, BLOCK_LENGTH);
	bitwise_permutation(args->plain_block, permuted_msg, g_ip_table, 64);
	ft_memcpy(right_half, permuted_msg + BLOCK_LENGTH / 2, BLOCK_LENGTH / 2);
	ft_memcpy(left_half, permuted_msg, BLOCK_LENGTH / 2);
	round = 0;
	while (round < ROUNDS)
	{
		ft_memcpy(right_half_copy, right_half, BLOCK_LENGTH / 2);
		mangler(right_half, round, args);
		right_half[0] ^= left_half[0];
		right_half[1] ^= left_half[1];
		right_half[2] ^= left_half[2];
		right_half[3] ^= left_half[3];
		ft_memcpy(left_half, right_half_copy, BLOCK_LENGTH / 2);
		round++;
	}
	ft_memcpy(permuted_msg, right_half, BLOCK_LENGTH / 2);
	ft_memcpy(permuted_msg + BLOCK_LENGTH / 2, left_half, BLOCK_LENGTH / 2);
	bitwise_permutation(permuted_msg, args->cipher_block, g_fp_table, 64);
}

// Main function for des-ecb encryption/decryption.
void	des_ecb(t_encrypt_args *args)
{
	obtain_main_key(args);
	generate_subkeys(args);
	ft_memcpy(args->plain_block, args->message, BLOCK_LENGTH);
	ft_hex_dump(args->plain_block, BLOCK_LENGTH, 16);
	process_block_cipher(args);
	ft_hex_dump(args->cipher_block, BLOCK_LENGTH, 16);
	if (args->encrypt_mode)
		des_ecb_encrypt(args);
	else if (args->decrypt_mode)
	{
		if (args->base64_mode)
			decode_encrypted_message(args);
		if (!ft_strncmp(args->message, SALT_STR, SALT_LENGTH))
			extract_salt(args);
		des_ecb_decrypt(args);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_ecb.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 11:18:14 by jesuserr          #+#    #+#             */
/*   Updated: 2025/02/05 13:45:49 by jesuserr         ###   ########.fr       */
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

// Main function for des-ecb encryption/decryption.
void	des_ecb(t_encrypt_args *args)
{
	if (args->key_provided)
		convert_str_to_hex(args->key, args->hex_key);
	else if (args->pass)
	{
		if (args->salt_provided)
			convert_str_to_hex(args->salt, args->hex_salt);
		else
			generate_salt(args->hex_salt, args);
		generate_derived_key(args);
	}
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_ecb.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 11:18:14 by jesuserr          #+#    #+#             */
/*   Updated: 2025/02/03 11:08:18 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_ssl.h"

// Encode encrypted message to base64 format and send it to the output file
// descriptor (terminal or output file).
static void	encode_encrypted_message(t_encrypt_args *args, \
unsigned char *ciphertext, int ciphertext_len)
{
	t_encode_args	encode_args;

	encode_args.message = (char *)ciphertext;
	encode_args.message_length = ciphertext_len;
	encode_args.output_fd = args->output_fd;
	encode_message(&encode_args);
}

// Cyphered text is limited to 4096 bytes. Should be dynamically allocated
// and freed.
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
	if (args->base64_mode)
		encode_encrypted_message(args, ciphertext, ciphertext_len);
	else
		for (int i = 0; i < ciphertext_len; i++)
			ft_putchar_fd(ciphertext[i], args->output_fd);
}

// base64() is not called directly in order to handle properly when there is
// an error and print_encrypt_strerror_and_exit() is called. If base64() were
// called directly, the program would exit without freeing the allocated memory.
static void	decode_encrypted_message(t_encrypt_args *args)
{
	t_encode_args	decode_args;

	decode_args.message = args->message;
	decode_args.message_length = args->message_length;
	remove_message_whitespaces_and_newlines(&decode_args);
	if (!proper_encoded_message(&decode_args))
	{
		errno = EBADMSG;
		print_encrypt_strerror_and_exit("Invalid base64 message", args);
	}
	decode_base64_message(&decode_args, args->message, args->message);
	args->message_length = decode_args.message_length;
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

	if (args->base64_mode)
		decode_encrypted_message(args);
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
//ft_hex_dump(args, sizeof(t_encrypt_args), 8);
void	des_ecb(t_encrypt_args *args)
{
	if (!args->key_provided)
		generate_derived_key(args);
	else
		convert_str_to_hex(args->key, args->hex_key);
	if (args->encrypt_mode)
		des_ecb_encrypt(args);
	else if (args->decrypt_mode)
		des_ecb_decrypt(args);
}

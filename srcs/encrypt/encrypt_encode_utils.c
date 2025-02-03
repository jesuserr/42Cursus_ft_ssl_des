/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encrypt_encode_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 12:33:47 by jesuserr          #+#    #+#             */
/*   Updated: 2025/02/03 12:35:36 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_ssl.h"

// Similar to 'decode_message()' in base64.c, except that in this case the 
// output is not sent to the file descriptor, but stored in args->message in
// order to be decrypted later (args->message_length is updated).
void	decode_base64_message(t_encode_args *args, char *msg, char *copy)
{
	uint8_t		quartet[BASE64_DEC_BLOCKS];
	uint8_t		output[BASE64_ENC_BLOCKS];
	uint8_t		reverse_table[4];
	uint64_t	i;

	i = 0;
	while (i < (args->message_length / BASE64_DEC_BLOCKS) * 4)
	{
		ft_memcpy(quartet, args->message + i, BASE64_DEC_BLOCKS);
		reverse_table[0] = g_base64_reverse_table[quartet[0]];
		reverse_table[1] = g_base64_reverse_table[quartet[1]];
		reverse_table[2] = g_base64_reverse_table[quartet[2]];
		reverse_table[3] = g_base64_reverse_table[quartet[3]];
		output[0] = (reverse_table[0] << 2) | (reverse_table[1] >> 4);
		output[1] = (reverse_table[1] << 4) | (reverse_table[2] >> 2);
		output[2] = (reverse_table[2] << 6) | reverse_table[3];
		*msg++ = (char)output[0];
		if (quartet[2] != '=')
			*msg++ = (char)output[1];
		if (quartet[3] != '=')
			*msg++ = (char)output[2];
		i += BASE64_DEC_BLOCKS;
	}
	args->message_length = msg - copy;
}

// Encode encrypted message to base64 format and send it to the output file
// descriptor (terminal or output file).
void	encode_encrypted_message(t_encrypt_args *args, \
unsigned char *ciphertext, int ciphertext_len)
{
	t_encode_args	encode_args;

	encode_args.message = (char *)ciphertext;
	encode_args.message_length = ciphertext_len;
	encode_args.output_fd = args->output_fd;
	encode_message(&encode_args);
}

// base64() is not called directly in order to handle properly when there is
// an error and print_encrypt_strerror_and_exit() is called. If base64() were
// called directly, the program would exit without freeing the allocated memory.
void	decode_encrypted_message(t_encrypt_args *args)
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

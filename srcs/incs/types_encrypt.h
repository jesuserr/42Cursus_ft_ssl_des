/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types_encrypt.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:48:16 by jesuserr          #+#    #+#             */
/*   Updated: 2025/02/05 14:50:32 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_ENCRYPT_H
# define TYPES_ENCRYPT_H

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              DEFINES
*/
# define ITERATIONS 	10000U			// Number of iterations for PBKDF2
# define KEY_LENGTH		8U				// Key length in bytes (64 bits)
# define SALT_STR		"Salted__"		// Salt string for DES encryption
# define SALT_LENGTH	8U				// Salt length in bytes (64 bits)
# define SALT_TOTAL_LEN	16U				// (Salt string + Salt) length in bytes

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              STRUCTS
*/
typedef struct s_encrypt_args
{
	char		*input_pipe;
	char		*input_file;
	char		*message;
	char		*input_file_name;
	uint64_t	input_file_size;
	uint64_t	pipe_size;
	uint64_t	message_length;
	int			output_fd;
	bool		decrypt_mode;
	bool		encrypt_mode;
	bool		input_from_file;
	bool		output_to_file;
	bool		base64_mode;
	bool		key_provided;
	bool		pass_provided;
	bool		salt_provided;
	bool		iv_provided;
	char		*key;
	char		*pass;
	char		*salt;
	char		*iv;
	uint8_t		encrypt_function;
	uint8_t		hex_key[KEY_LENGTH];
	uint8_t		hex_iv[KEY_LENGTH];
	uint8_t		hex_salt[KEY_LENGTH];
}	t_encrypt_args;

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types_encrypt.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:48:16 by jesuserr          #+#    #+#             */
/*   Updated: 2025/01/29 11:55:00 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_ENCRYPT_H
# define TYPES_ENCRYPT_H

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
}	t_encrypt_args;

#endif

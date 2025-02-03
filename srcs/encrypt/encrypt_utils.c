/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encrypt_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 19:23:26 by jesuserr          #+#    #+#             */
/*   Updated: 2025/02/03 11:00:37 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_ssl.h"

// At the contrary of the hashing functions, the encrypt function is just 
// called once, since the input can only come from one source. File has
// priority over pipe, so if both are provided, the pipe will be ignored.
void	calls_to_encrypt_function(t_encrypt_args *args)
{
	void	(*encrpyt_functions[])(t_encrypt_args *) = {des_ecb, des_cbc};

	if (args->input_pipe)
	{
		args->message = args->input_pipe;
		args->message_length = args->pipe_size;
		encrpyt_functions[args->encrypt_function](args);
		free(args->input_pipe);
	}
	if (args->input_file)
	{
		args->message = args->input_file;
		args->message_length = args->input_file_size;
		encrpyt_functions[args->encrypt_function](args);
		if (args->input_file_size > 0)
			if (munmap(args->input_file, args->input_file_size) < 0)
				print_encrypt_strerror_and_exit("munmap", args);
	}
	if (args->output_to_file && args->output_fd != STDOUT_FILENO)
		if (close(args->output_fd) < 0)
			print_encrypt_strerror_and_exit("close", args);
	if (!args->pass_provided && args->pass)
		free(args->pass);
}

void	print_encrypt_usage(void)
{
	ft_printf("Usage\n"
		"  ./ft_ssl <command> [flags] [file]\n\n"
		"Cipher options:\n"
		"  command     des, des-ecb or des-cbc\n"
		"  -h          print help and exit\n"
		"  -a          decode/encode the input/output in base64\n"
		"  -d          decrypt mode\n"
		"  -e          encrypt mode (default)\n"
		"  -i <file>   input file\n"
		"  -k <key>    key in hexadecimal\n"
		"  -o <file>   output file\n"
		"  -p          password in ASCII\n"
		"  -s <salt>   salt in hexadecimal\n"
		"  -v          initialization vector in hexadecimal\n");
	exit(EXIT_SUCCESS);
}

// Prints system error message, releases allocated memory and file descriptor
// and exits with EXIT_FAILURE status.
void	print_encrypt_strerror_and_exit(char *msg, t_encrypt_args *args)
{
	ft_printf("%s: %s\n", msg, strerror(errno));
	if (args->input_pipe)
		free(args->input_pipe);
	if (args->input_file)
		munmap(args->input_file, args->input_file_size);
	if (args->output_to_file && args->output_fd != STDOUT_FILENO)
		close(args->output_fd);
	if (!args->pass_provided && args->pass)
		free(args->pass);
	exit(EXIT_FAILURE);
}

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

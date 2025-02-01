/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encrypt_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 19:23:26 by jesuserr          #+#    #+#             */
/*   Updated: 2025/02/01 13:27:58 by jesuserr         ###   ########.fr       */
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
		ft_printf("Encrypting pipe\n");
		encrpyt_functions[args->encrypt_function](args);
		free(args->input_pipe);
	}
	if (args->input_file)
	{
		args->message = args->input_file;
		args->message_length = args->input_file_size;
		ft_printf("Encrypting file\n");
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encrypt_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 19:23:26 by jesuserr          #+#    #+#             */
/*   Updated: 2025/01/28 13:40:14 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/ft_ssl.h"

// At the contrary of the hashing functions, the encrypt function is just 
// called once, since the input can only come from one source.
void	calls_to_encrypt_function(t_encrypt_args *args)
{
	ft_hex_dump(args, sizeof(t_encrypt_args), 8);
	if (args->input_pipe)
	{
		args->message = args->input_pipe;
		args->message_length = args->pipe_size;
		ft_printf("Encrypting pipe\n");
		//base64(args);
		free(args->input_pipe);
	}
	if (args->input_file)
	{
		args->message = args->input_file;
		args->message_length = args->input_file_size;
		ft_printf("Encrypting file\n");
		//base64(args);
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

// Returns true if the string is a valid hexadecimal string.
bool	str_is_hex(char *str, t_encrypt_args *args)
{
	while (*str)
	{
		if (!ft_isdigit(*str) && !ft_strchr("abcdefABCDEF", *str))
		{
			if (args->output_to_file && args->output_fd != STDOUT_FILENO)
				close(args->output_fd);
			print_error_and_exit("Invalid hexadecimal string");
		}
		str++;
	}
	return (true);
}

// Returns true if the string is a valid printable ascii string.
bool	str_is_ascii(char *str, t_encrypt_args *args)
{
	while (*str)
	{
		if (!ft_isprint(*str))
		{
			if (args->output_to_file && args->output_fd != STDOUT_FILENO)
				close(args->output_fd);
			print_error_and_exit("Invalid ascii string");
		}
		str++;
	}
	return (true);
}

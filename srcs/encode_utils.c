/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encode_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 13:12:56 by jesuserr          #+#    #+#             */
/*   Updated: 2024/12/27 21:54:19 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/ft_ssl.h"

void	calls_to_decoding_function(t_encode_args *args)
{
	if (args->input_pipe)
	{
		args->msg_origin = IS_PIPE;
		args->message = args->input_pipe;
		args->message_length = args->pipe_size;
		base64(args);
		free(args->input_pipe);
	}
	if (args->input_file)
	{
		args->msg_origin = IS_FILE;
		args->message = args->input_file;
		args->message_length = args->input_file_size;
		base64(args);
		if (args->input_file_size > 0)
			if (munmap(args->input_file, args->input_file_size) < 0)
				print_encode_strerror_and_exit("munmap", args);
	}
}

void	print_encode_usage(void)
{
	ft_printf("Usage\n"
		"  ./ft_ssl <command> [flags] [file]\n\n"
		"Encode options:\n"
		"  command     base64\n"
		"  -h          print help and exit\n"
		"  -d          decode mode\n"
		"  -e          encode mode (default)\n"
		"  -i <file>   input file\n"
		"  -o <file>   output file\n");
	exit(EXIT_SUCCESS);
}

// Prints system error message, releases allocated memory and exits with 
// EXIT_FAILURE status.
void	print_encode_strerror_and_exit(char *msg, t_encode_args *args)
{
	ft_printf("%s: %s\n", msg, strerror(errno));
	if (args->input_pipe)
		free(args->input_pipe);
	if (args->input_file)
		munmap(args->input_file, args->input_file_size);
	exit(EXIT_FAILURE);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 19:47:43 by jesuserr          #+#    #+#             */
/*   Updated: 2024/12/12 20:14:37 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/ft_ssl.h"

// Uses an array of function pointers to call the hashing function selected by
// the user. Performs up to three calls to the hashing function, one for each
// possible input source (pipe, string or file) that has some content.
void	calls_to_hashing_function(t_hash_args *args)
{
	void		(*hash_functions[])(t_hash_args *) = \
				{md5_sum, sha224_sum, sha256_sum, sha384_sum, sha512_sum};
	char		*msg[3];
	uint8_t		origin;

	msg[IS_PIPE] = args->input_pipe;
	msg[IS_STRING] = args->input_str;
	msg[IS_FILE] = args->input_file;
	origin = IS_PIPE;
	while (origin <= IS_FILE)
	{
		if (msg[origin])
		{
			args->msg_origin = origin;
			args->message = msg[origin];
			hash_functions[args->hash_function](args);
		}
		origin++;
	}
	if (args->input_pipe)
		free(args->input_pipe);
	if (args->input_file && args->file_size > 0)
		if (munmap(args->input_file, args->file_size) < 0)
			print_hash_strerror_and_exit("munmap", args);
}

void	print_hash_usage(void)
{
	ft_printf("Usage\n"
		"  ./ft_ssl <command> [flags] [file]\n\n"
		"Options:\n"
		"  command     md5, sha224, sha256, sha384 or sha512\n"
		"  -h          print help and exit\n"
		"  -p          echo STDIN to STDOUT and append the checksum to STDOUT\n"
		"  -q          quiet mode\n"
		"  -r          reverse the format of the output\n"
		"  -s <string> print the sum of the given string\n");
	exit(EXIT_SUCCESS);
}

// Prints system error message, releases allocated memory and exits with 
// EXIT_FAILURE status.
void	print_hash_strerror_and_exit(char *msg, t_hash_args *args)
{
	ft_printf("%s: %s\n", msg, strerror(errno));
	if (args->input_pipe)
		free(args->input_pipe);
	if (args->input_file)
		munmap(args->input_file, args->file_size);
	exit(EXIT_FAILURE);
}

// Auxilary function for print_xxx_digest that is common to all hash functions.
void	print_prehash_output(char *algorithm, t_hash_args *args)
{
	if (args->msg_origin == IS_PIPE && !args->echo_stdin)
		ft_printf("(stdin)= ");
	else if (args->msg_origin == IS_PIPE && args->echo_stdin)
		ft_printf("(\"%s\")= ", args->message);
	else if (args->msg_origin == IS_STRING && !args->reverse_output)
		ft_printf("%s (\"%s\") = ", algorithm, args->message);
	else if (args->msg_origin == IS_FILE && !args->reverse_output)
		ft_printf("%s (%s) = ", algorithm, args->file_name);
}

// Removes the newline character from the end of the message if it has been 
// introduced by the 'echo' command when reading from stdin (pipe).
// Modified only for printing purposes, for hashing purposes the message with
// the newline character is used.
void	remove_newline_character(char *msg, uint64_t len)
{
	if (len > 0 && msg[len - 1] == '\n')
		msg[len - 1] = '\0';
}

// Reads from standard input and stores the string in the 'args->input_pipe'.
// Uses 'gnl_strjoin' to concatenate the input lines and using the flag 'erase'
// set to 1,  frees the previous content of the 'input_pipe' field.
void	read_interactive_mode(t_hash_args *args)
{
	char	*input;

	input = get_next_line(STDIN_FILENO);
	args->input_pipe = ft_strdup("");
	while (input)
	{
		args->input_pipe = gnl_strjoin(args->input_pipe, input, 1);
		free(input);
		input = get_next_line(STDIN_FILENO);
	}
	args->pipe_size = ft_strlen(args->input_pipe);
	free(input);
}
